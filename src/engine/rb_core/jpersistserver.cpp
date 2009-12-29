//****************************************************************************/
//  File:  JPersistServer.cpp
//****************************************************************************/
#include "stdafx.h"
#include "JPersistServer.h"

#include "Path.h"
#include "Buffer.h"
#include "CountStream.h"
#include "Hash.h"
#include "tinyxml.h"
#include "JMLNode.h"
#include "JSignalServer.h"
#include "JObjectIterator.h"
#include "IFileServer.h"
#include "IStringServer.h"

//****************************************************************************/
/*  JPersistServer implementation
//****************************************************************************/
decl_class(JPersistServer);
JPersistServer::JPersistServer()
{
    g_pPersistServer = this;
    SetName( "persist" );
} // JPersistServer::JPersistServer

JPersistServer::~JPersistServer()
{
} // JPersistServer::JPersistServer

ScriptResource* JPersistServer::FindScriptResource( const char* name )
{
    int nRes = m_ScriptResReg.size();
    for (int i = 0; i < nRes; i++)
    {
        if (!stricmp( m_ScriptResReg[i].m_Name, name ))
        {
            return &m_ScriptResReg[i];
        }
    }
    return NULL;
}

ScriptResource* JPersistServer::FindScriptResource( const JObject* pObject )
{
    // FIXME: linear search most probably is an overkill here
    int nRes = m_ScriptResReg.size();
    for (int i = 0; i < nRes; i++)
    {
        if (m_ScriptResReg[i].m_pRoot == pObject)
        {
            return &m_ScriptResReg[i];
        }
    }
    return NULL;
}

JPersistFormat GetFormatByExt( const char* fileExt )
{
    JPersistFormat fmt = PersistFmt_Unknown;
    if (!stricmp( fileExt, "rxml" ))
    {
        fmt = PersistFmt_XML;
    }

    if (!stricmp( fileExt, "rjml" ))
    {
        fmt = PersistFmt_JML;
    }

    if (!stricmp( fileExt, "rmdl" ) ||
        !stricmp( fileExt, "rbm"  ))
    {
        fmt = PersistFmt_Model;
    }

    if (!stricmp( fileExt, "ranm" ) ||
        !stricmp( fileExt, "rba"  ))
    {
        fmt = PersistFmt_Anim;
    }

    if (!stricmp( fileExt, "rbin" ))
    {
        fmt = PersistFmt_Bin;
    }
    return fmt;
}

bool JPersistServer::SaveObject( JObject* pObject, const char* fname, JPersistFormat fmt )
{
    FOutStream os( fname );
    return Save( pObject, os, fmt );
}

JObject* JPersistServer::LoadObject( const char* fname, JObject* pSrc )
{
    if (!fname || fname[0] == 0) return NULL;

    Path path( fname );
    if (path.GetExt()[0] == 0)
    {
        //  try to pick the first suitable, if no extension provided
             if (g_pFileServer->FindMedia( fname, "rbin" )) path.SetExt( "rbin" );
        else if (g_pFileServer->FindMedia( fname, "rxml" )) path.SetExt( "rxml" );
        else if (g_pFileServer->FindMedia( fname, "rjml" )) path.SetExt( "rjml" );
        else if (g_pFileServer->FindMedia( fname, "rmdl" )) path.SetExt( "rmdl" );
        else if (g_pFileServer->FindMedia( fname, "ranm" )) path.SetExt( "ranm" );
    }

    JPersistFormat fmt = GetFormatByExt( path.GetExt() );

    FInStream is;
    JString fullPath = path.GetFullPath();
    if (!is.OpenFile( fname ))
    {
        if (g_pFileServer->FindMedia( path.GetFile(), path.GetExt(), &fullPath ))
        {
            is.OpenFile( fullPath );
        }
        else
        {
            rlog.warn( "Could not open script file: %s", path.GetFileExt() );
            return NULL;
        }
    }

    uint32_t hash = 0;
    JObject* pObj = Load( is, fmt, pSrc, &hash );

    //  register loaded script resource
    m_ScriptResReg.push_back( ScriptResource() );
    ScriptResource* pRes = &m_ScriptResReg.back();

    pRes->m_Hash    = hash;
    pRes->m_Name    = fname;
    pRes->m_pRoot   = pObj;
    pRes->m_Format  = fmt;
    pRes->m_Path    = fullPath;

    //  FIXME: convert text resources
    JObjectIterator it( pObj );
    JString val;
    while (it)
    {
        JObject* pObj = *it;
        if (pObj->GetProperty( "text", val ) && val.size() > 0 && val[0] == '#')
        {
            pObj->SetProperty( "text", g_pStringServer->GetString( val.c_str() ) );
        }
        ++it;
    }

    return pObj;
}

const char* c_BinDataFOURCC = "RAWD";

JObject* JPersistServer::Load( InStream& is, JPersistFormat fmt, JObject* pSrc, uint32_t* pHash )
{
    if (is.GetTotalSize() <= 0)
    {
        return NULL;
    }

    MemInStream mis;
    Buffer buf;
    InStream* pBinStream = NULL;
    JObject* pObj = NULL;

    if (fmt == PersistFmt_Bin)
    {
        pObj = LoadBin( is, pSrc );
        pBinStream = &is;
    }
    else
    {
        int size = is.GetTotalSize() + 1;
        buf.Resize( size );
        is.Read( buf.GetData(), size );
        buf.GetData()[size - 1] = 0;
        char* pBuf = (char*)buf.GetData();

        if (pHash)
        {
            *pHash = PHHash( buf.GetData(), size - 1 );
        }

        if (fmt == PersistFmt_XML)
        {
            pObj = LoadXML( pBuf, pSrc );
        }
        if (fmt == PersistFmt_JML   ||
            fmt == PersistFmt_Model ||
            fmt == PersistFmt_Anim )
        {
            pObj = LoadJML( pBuf, pSrc );
        }

        int cChar = 0;
        while (pBuf[cChar] && cChar < size)
        {
            cChar++;
        }
        cChar++;

        mis = MemInStream( buf.GetData() + cChar, size - cChar - 1 );
        pBinStream = &mis;
    }

    if (pObj == NULL)
    {
        return pObj;
    }

    //  read binary data
    uint32_t id = 0;
    (*pBinStream) >> id;
    const char* pCharID = (const char*)(&id);
    if (strncmp( c_BinDataFOURCC, pCharID, 4 ) || !(*pBinStream))
    {
        return pObj;
    }

    JString name;
    while ((*pBinStream))
    {
        (*pBinStream) >> name;
        if (name.size() == 0)
        {
            break;
        }
        int nBytes = 0;
        (*pBinStream) >> nBytes;
        if (nBytes == 0)
        {
            break;
        }
        JObject* pCurObj = g_pObjectServer->FindObject( name.c_str(), pObj );
        if (pCurObj == NULL)
        {
            rlog.warn( "Could not find object %s while unserializing binary data in subtree: %s",
                name.c_str(), pObj->GetName() );
            (*pBinStream).Rewind( nBytes );
        }
        else
        {
            pCurObj->Unserialize( *pBinStream );
        }
    }

    return pObj;
} // JPersistServer::Load

bool JPersistServer::Save( JObject* pObject, OutStream& os, JPersistFormat fmt )
{
    bool bRes = false;
    if (fmt == PersistFmt_XML)
    {
        bRes = SaveXML( os, pObject );
    }
    if (fmt == PersistFmt_JML)
    {
        bRes = SaveJML( os, pObject );
    }
    if (fmt == PersistFmt_Bin)
    {
        bRes = SaveBin( os, pObject );
    }
    if (!bRes)
    {
        return false;
    }

    //  save binary data
    JObjectIterator it( pObject );
    int nSaved = 0;
    while (it)
    {
        JObject* pObj = *it;
        ++it;

        CountStream cs;
        pObj->Serialize( cs );
        cs.Close();
        int nBytes = cs.GetTotalSize();
        if (nBytes == 0) continue;
        if (nSaved == 0)
        {
            os << BYTE( 0 );
            os.Write( (const BYTE*)c_BinDataFOURCC, 4 );
        }
        nSaved++;
        JString name( pObj->GetName() );
        if (name.size() == 0)
        {
            rlog.warn( "Trying to serialize binary data for unnamed object, while saving object tree: %s",
                pObject->GetName() );
            continue;
        }
        os << name;
        os << nBytes;
        pObj->Serialize( os );
    }
    return true;
} // JPersistServer::Save

JObject* JPersistServer::FromJML( const JMLNode& jml, JObject* pSrc, JObject* pParent )
{
    JObject* pObj = pSrc;
    if (!jml.IsComplete())
    {
        pObj = g_pPersistServer->LoadObject( jml.GetName(), pSrc );
        if (pObj)
        {
            pObj->SetName( jml.GetName() );
        }
        return pObj;
    }

    const char* name = jml.GetName();
    const char* className = jml.GetClass();
    if (className[0] == '$')
    {
        //  template declaration
        className++;
    }

    if (pObj)
    {
        pObj->RemoveChildren();
    }
    else
    {
        pObj = g_pObjectServer->Create( className );
    }
    if (!pObj)
    {
        //  try to create from template
        JObject* pTemplate = g_pObjectServer->FindObject( className, NULL, pParent );
        if (pTemplate)
        {
            pObj = pTemplate->Clone( pParent, name, true );
        }
    }
    if (!pObj)
    {
        rlog.err( "Could not create object of class <%s>", className );
        return NULL;
    }

    pObj->SetName( name );
    className = pObj->ClassName();

    //  create child nodes
    const JMLNode* pChild = jml.FirstChild();
    while (pChild)
    {
        JObject* pChildObj = FromJML( *pChild, NULL, pObj );
        pObj->AddChild( pChildObj );
        pChild = pChild->NextSibling();
    }

    //  assign attribute values
    const JMLNode*  pAttr = jml.FirstAttr();
    JMetaClass*     pMeta = g_pObjectServer->FindClass( className );
    if (!pMeta) return NULL;
    JString strAttr;
    while (pAttr)
    {
        const char* attrName = pAttr->GetName();
        if (attrName[0] == '$')
        {
            //  template parameter declaration
            attrName++;
        }
        JClassAttr* pCA = g_pObjectServer->FindClassAttr( className, attrName );
        if (pCA)
        {
            const char* attrVal = pAttr->GetValue();
            //  check if a signal link
            if (!strncmp( attrVal, "<<", 2 ))
            {
                JSignalServer::s_pInstance->ConnectDst( pObj, pCA, attrVal + 2 );
            }
            else if (!strncmp( attrVal, ">>", 2 ))
            {
                JSignalServer::s_pInstance->ConnectSrc( pObj, pCA, attrVal + 2 );
            }
            else if (!strncmp( attrVal, "<>", 2 ))
            {
                JSignalServer::s_pInstance->ConnectSrc( pObj, pCA, attrVal + 2 );
                JSignalServer::s_pInstance->ConnectDst( pObj, pCA, attrVal + 2 );
            }
            else
            {
                strAttr = attrVal;
                pCA->Set( pObj, strAttr );
            }
        }
        pAttr = pAttr->NextSibling();
    }
    return pObj;
}

JObject* JPersistServer::LoadJML( char* text, JObject* pSrc )
{
    JMLNode jml( text );
    return FromJML( jml, pSrc );
}

JMLNode* JPersistServer::ToJML( const JObject* pObject, bool bRoot )
{
    JMetaClass* pMeta = g_pObjectServer->FindClass( pObject->ClassName() );
    if (!pMeta) return NULL;

    JMLNode* pNode = new JMLNode();

    JString className = pObject->ClassName();
    _strlwr( (char*)className.c_str() );
    pNode->SetClass( className[0] == 'j' ? className.c_str() + 1 : className.c_str() );
    const char* name = pObject->GetName();
    pNode->SetName( name );

    //  if node is a "soft-reference" - do not save sub-tree
    if (!bRoot && FindScriptResource( pObject ) != NULL)
    {
        pNode->SetComplete( false );
        return pNode;
    }

    //  assign attributes
    JObject* pDefault = pMeta->GetTemplate();
    JString val, defVal;
    JAttrIterator it( pMeta );
    while (it)
    {
        JClassAttr* pAttr = *it;
        ++it;
        if (pAttr->HasName( "name" ) || pAttr->IsDerived()) continue;
        pAttr->Get( (void*)pObject, val );
        pAttr->Get( (void*)pDefault, defVal );
        if (!strcmp( val.c_str(), defVal.c_str() )) continue;
        JString attrName( pAttr->GetName() );
        _strlwr( (char*)attrName.c_str() );
        JMLNode* pNodeAttr = pNode->AddAttr( attrName.c_str() );

        if (pAttr->GetTypeID() == ::GetTypeID<const char*>() ||
            pAttr->GetTypeID() == ::GetTypeID<JString>() ||
            pAttr->GetTypeID() == ::GetTypeID<std::string>())
        {
            val.insert( 0, "\"" );
            val += "\"";
        }
        pNodeAttr->SetValue( val.c_str() );

    }

    //  add signals
    SaveSignalsJML( pObject, pNode );

    //  create children
    int nChildren = pObject->GetNChildren();
    for (int i = 0; i < nChildren; i++)
    {
        pNode->AddChild( ToJML( pObject->GetChild( i ) ) );
    }
    return pNode;
} // JPersistServer::ToJML

bool JPersistServer::SaveJML( OutStream& os, const JObject* pObject )
{
    JMLNode* pNode = ToJML( pObject, true );
    if (!pNode) return false;
    pNode->Write( os );
    delete pNode;
    return true;
} // JPersistServer::SaveJML

JObject* JPersistServer::FromXML( const TiXmlElement* pNode, JObject* pSrc )
{
    if (pNode == NULL)
    {
        return NULL;
    }
    JObject* pObj = pSrc;
    const char* className = pNode->Value();

    if (!pObj) pObj = g_pObjectServer->Create( className );
    if (!pObj)
    {
        rlog.err( "Could not create object of class <%s>", className );
        return NULL;
    }
    pObj->RemoveChildren();
    const char* name = pNode->Attribute( "name" );
    if (name && name[0])
    {
        pObj->SetName( name );
    }
    else
    {
        pObj->SetName( className );
    }

    //  create child nodes
    const TiXmlElement* pChild = pNode->FirstChildElement();
    while (pChild)
    {
        if (!stricmp( pChild->Value(), "signal" ))
        {
            //  this is signal description
            const char* type = pChild->Attribute( "dir" );
            if (!type) type = "in";
            const char* attr = pChild->Attribute( "attr" );
            const char* path = pChild->Attribute( "path" );
            JClassAttr* pCA = g_pObjectServer->FindClassAttr( className, attr );
            if (!stricmp( type, "in" ))
            {
                JSignalServer::s_pInstance->ConnectDst( pObj, pCA, path );
            }
            else if(!stricmp( type, "out" ))
            {
                JSignalServer::s_pInstance->ConnectSrc( pObj, pCA, path );
            }
            else if(!stricmp( type, "inout" ))
            {
                JSignalServer::s_pInstance->ConnectSrc( pObj, pCA, path );
                JSignalServer::s_pInstance->ConnectDst( pObj, pCA, path );
            }
        }
        else
        {
            JObject* pChildObj = FromXML( pChild, NULL );
            pObj->AddChild( pChildObj );
        }
        pChild = pChild->NextSiblingElement();
    }

    //  assign attribute values
    const TiXmlAttribute* pAttr = pNode->FirstAttribute();
    JMetaClass* pMeta = g_pObjectServer->FindClass( className );
    if (!pMeta) return NULL;
    JString strAttr;
    while (pAttr)
    {
        JClassAttr* pCA = g_pObjectServer->FindClassAttr( className, pAttr->Name() );
        if (pCA)
        {
            const char* attrVal = pAttr->Value();
            strAttr = attrVal;
            pCA->Set( pObj, strAttr );
        }
        pAttr = pAttr->Next();
    }
    return pObj;
} // JPersistServer::FromXML

JObject* JPersistServer::LoadXML( char* text, JObject* pSrc )
{
    TiXmlDocument doc;
    doc.Parse( text );
	if (doc.Error())
	{
        rlog.err( "Error parsing XML in %s: %s\n", doc.Value(), doc.ErrorDesc() );
		return NULL;
	}

    return FromXML( doc.FirstChildElement(), pSrc );
} // JPersistServer::LoadXML

TiXmlElement* JPersistServer::ToXML( const JObject* pObject, bool bRoot )
{
    JMetaClass* pMeta = g_pObjectServer->FindClass( pObject->ClassName() );
    if (!pMeta) return NULL;

    JString className = pObject->ClassName();
    _strlwr( (char*)className.c_str() );

    TiXmlElement* pNode = new TiXmlElement( className[0] == 'j' ? className.c_str() + 1 : className.c_str() );

    //  assign attributes
    JObject* pDefault = pMeta->GetTemplate();
    JString val, defVal;
    JAttrIterator it( pMeta );
    while (it)
    {
        JClassAttr* pAttr = *it;
        ++it;
        if (pAttr->IsDerived())
        {
            continue;
        }
        pAttr->Get( (void*)pObject, val );
        pAttr->Get( (void*)pDefault, defVal );
        if (!strcmp( val.c_str(), defVal.c_str() )) continue;
        JString attrName( pAttr->GetName() );
        _strlwr( (char*)attrName.c_str() );
        pNode->SetAttribute( attrName.c_str(), val.c_str() );
    }

    //  add signals
    SaveSignalsXML( pObject, pNode );

    //  create children
    int nChildren = pObject->GetNChildren();
    for (int i = 0; i < nChildren; i++)
    {
        TiXmlElement* pChild = ToXML( pObject->GetChild( i ) );
        pNode->LinkEndChild( pChild );
    }
    return pNode;
} // JPersistServer::ToXML

bool JPersistServer::SaveXML( OutStream& os, const JObject* pObject )
{
    TiXmlElement* pNode = ToXML( pObject, true );
    if (!pNode) return false;
    TiXmlPrinter printer;
	bool bRes = pNode->Accept( &printer );
    if (!bRes)
    {
        return false;
    }
    os.Write( (const BYTE*)printer.CStr(), strlen( printer.CStr() ) );
    delete pNode;
    return true;
} // JPersistServer::SaveXML

bool JPersistServer::SaveBin( OutStream& os, const JObject* pObject, bool bWithChildren, bool bSaveSignals )
{
    const char* className = pObject->ClassName();
    JMetaClass* pMeta = g_pObjectServer->FindClass( className );
    if (!pMeta) return false;

    //  serialize class name
    os << className;

    //  serialize children
    int nCh = pObject->GetNChildren();
    if (bWithChildren)
    {
        os << nCh;
        for (int i = 0; i < nCh; i++)
        {
            SaveBin( os, pObject->GetChild( i ), true, bSaveSignals );
        }
    }
    else
    {
        nCh = 0;
        os << nCh;
    }

    //  serialize attributes
    int nA = pMeta->GetNAttr();

    BYTE saveMode = SaveMode_BYTEIdx;
    if (nA >= 255) saveMode = SaveMode_WORDIdx;

    os << saveMode;

    JString val, defVal;
    JAttrIterator it( pMeta );
    if (saveMode == SaveMode_SaveAll)
    {
        while (it)
        {
            JClassAttr* attr = *it;
            ++it;
            if (attr->IsDerived())
            {
                continue;
            }
            attr->Serialize( (void*)pObject, os );
        }
    }
    else if (saveMode == SaveMode_BYTEIdx)
    {
        BYTE idx = 0;
        JObject* pDefault = pMeta->GetTemplate();
        while (it)
        {
            JClassAttr* attr = *it;
            ++it;
            if (attr->IsDerived())
            {
                idx++;
                continue;
            }
            attr->Get( (void*)pObject, val );
            attr->Get( (void*)pDefault, defVal );
            if (strcmp( val.c_str(), defVal.c_str() ))
            {
                os << idx;
                attr->Serialize( (void*)pObject, os );
            }
            idx++;
        }
        idx = 0xFF;
        os << idx;
    }
    else if (saveMode == SaveMode_WORDIdx)
    {
        WORD idx = 0;
        JObject* pDefault = pMeta->GetTemplate();
        while (it)
        {
            JClassAttr* attr = *it;
            ++it;
            if (attr->IsDerived())
            {
                idx++;
                continue;
            }
            attr->Get( (void*)pObject, val );
            attr->Get( (void*)pDefault, defVal );
            if (strcmp( val.c_str(), defVal.c_str() ))
            {
                os << idx;
                attr->Serialize( (void*)pObject, os );
            }
            idx++;
        }
        idx = 0xFFFF;
        os << idx;
    }

    //  serialize custom data
    pObject->Serialize( os );

    //  serialize signals
    if (bSaveSignals && JSignalServer::s_pInstance)
    {
        SaveSignalsBin( pObject, os );
    }

    return true;
} // JPersistServer::SaveBin

JObject* JPersistServer::LoadBin( InStream& is, JObject* pSrc )
{
    JObject* pObj = pSrc;

    //  unserialize class name
    JString className;
    is >> className;

    JMetaClass* pMeta = g_pObjectServer->FindClass( className.c_str() );
    if (!pMeta) return NULL;

    if (!pObj) pObj = g_pObjectServer->Create( className.c_str() );
    if (!pObj) return NULL;

    pObj->RemoveChildren();

    //  unserialize children
    int nCh = 0;
    is >> nCh;
    for (int i = 0; i < nCh; i++)
    {
        pObj->AddChild( LoadBin( is ) );
    }

    //  unserialize attributes
    BYTE saveMode = SaveMode_BYTEIdx;
    is >> saveMode;

    int nAttr = pMeta->GetNAttr();
    if (saveMode == SaveMode_SaveAll)
    {
        JAttrIterator it( pMeta );
        int idx = 0;
        while (it)
        {
            JClassAttr* attr = pMeta->GetAttr( idx );
            attr->Unserialize( (void*)pObj, is );
            idx++;
            ++it;
        }
    }
    else if (saveMode == SaveMode_BYTEIdx)
    {
        BYTE idx = 0;
        for (int i = 0; i < nAttr; i++)
        {
            is >> idx;
            if (idx == 0xFF) break;
            JClassAttr* attr = pMeta->GetAttr( idx );
            attr->Unserialize( (void*)pObj, is );
        }
    }
    else if (saveMode == SaveMode_WORDIdx)
    {
        WORD idx = 0;
        for (int i = 0; i < nAttr; i++)
        {
            is >> idx;
            if (idx == 0xFFFF) break;
            JClassAttr* attr = pMeta->GetAttr( idx );
            attr->Unserialize( (void*)pObj, is );
        }
    }

    //  unserialize custom data
    pObj->Unserialize( is );

    //  unserialize signals
    LoadSignalsBin( pObj, is );

    return pObj;
} // JPersistServer::LoadBin

int JPersistServer::LoadSignalsBin( JObject* pObj, InStream& is )
{
    if (!JSignalServer::s_pInstance) return NULL;
    int nSig = 0;
    is >> nSig;
    JString attrName;
    JString path;
    for (int i = 0; i < nSig; i++)
    {
        JSignal cn;
        is >> attrName;
        is >> cn.m_Type;
        is >> path;
        JClassAttr* pCA = g_pObjectServer->FindClassAttr( pObj->ClassName(), attrName.c_str() );
        if (!pCA) continue;
        if (cn.m_Type == Signal_In)
        {
            JSignalServer::s_pInstance->ConnectDst( pObj, pCA, path.c_str() );
        }
        else if (cn.m_Type == Signal_Out)
        {
            JSignalServer::s_pInstance->ConnectSrc( pObj, pCA, path.c_str() );
        }
        else if (cn.m_Type == Signal_InOut)
        {
            JSignalServer::s_pInstance->ConnectSrc( pObj, pCA, path.c_str() );
            JSignalServer::s_pInstance->ConnectDst( pObj, pCA, path.c_str() );
        }
    }
    return nSig;
} // JPersistServer::LoadSignalsBin

int JPersistServer::SaveSignalsBin( const JObject* pObj, OutStream& os )
{
    static std::vector<JSignal*> sigList;
    JSignalServer::s_pInstance->GetSignalList( pObj, sigList );
    int nSig = sigList.size();

    int nValidSig = nSig;
    for (int i = 0; i < nSig; i++)
    {
        JSignal& cn = *(sigList[i]);
        if ((cn.m_pSrcObj == pObj && cn.m_bSrcInit) ||
            (cn.m_pDstObj == pObj && !cn.m_bSrcInit))
        {
        }
        else
        {
            nValidSig--;
        }
    }
    os << nValidSig;
    for (int i = 0; i < nSig; i++)
    {
        JSignal& cn = *(sigList[i]);
        if (cn.m_pSrcObj == pObj && cn.m_bSrcInit)
        {
            os << cn.m_pSrcAttr->GetName();
            os << cn.m_Type;
            os << cn.m_DstPath;
        }
        else if (cn.m_pDstObj == pObj && !cn.m_bSrcInit)
        {
            os << cn.m_pDstAttr->GetName();
            os << cn.m_Type;
            os << cn.m_SrcPath;
        }
        else
        {
            //  probably invalid signal
        }
    }
    return nSig;
} // JPersistServer::SaveSignalsBin

int JPersistServer::SaveSignalsJML( const JObject* pObj, JMLNode* pNode )
{
    static std::vector<JSignal*> sigList;
    JSignalServer::s_pInstance->GetSignalList( pObj, sigList );
    int nSig = sigList.size();
    for (int i = 0; i < nSig; i++)
    {
        JSignal& cn = *(sigList[i]);
        if (cn.m_Type == Signal_None)
        {
            continue;
        }
        if (cn.m_pSrcObj == pObj && cn.m_bSrcInit)
        {
            JMLNode* pSignal = pNode->AddAttr( cn.m_pSrcAttr->GetName() );
            _strlwr( (char*)pSignal->GetName() );
            JString val;
            if (cn.m_Type == Signal_In) val = "<<";
            else if (cn.m_Type == Signal_Out) val = ">>";
            else if (cn.m_Type == Signal_InOut) val = "<>";
            val += cn.m_DstPath;
            pSignal->SetValue( val.c_str() );
        }
        if (cn.m_pDstObj == pObj && !cn.m_bSrcInit)
        {
            JMLNode* pSignal = pNode->AddAttr( cn.m_pDstAttr->GetName() );
            _strlwr( (char*)pSignal->GetName() );
            JString val;
            if (cn.m_Type == Signal_In) val = "<<";
            else if (cn.m_Type == Signal_Out) val = ">>";
            else if (cn.m_Type == Signal_InOut) val = "<>";
            val += cn.m_SrcPath;
            pSignal->SetValue( val.c_str() );
        }
    }
    return nSig;
} // JPersistServer::SaveSignalsJML

int JPersistServer::SaveSignalsXML( const JObject* pObj, TiXmlElement* pNode )
{
    static std::vector<JSignal*> sigList;
    JSignalServer::s_pInstance->GetSignalList( pObj, sigList );
    int nSig = sigList.size();
    if (nSig == 0)
    {
        return 0;
    }

    for (int i = 0; i < nSig; i++)
    {
        JSignal& cn = *(sigList[i]);
        if (cn.m_Type == Signal_None)
        {
            continue;
        }
        if (cn.m_pSrcObj == pObj && cn.m_bSrcInit)
        {
            TiXmlElement* pSigNode = new TiXmlElement( "signal" );
            pNode->LinkEndChild( pSigNode );

            pSigNode->SetAttribute( "attr", cn.m_pSrcAttr->GetName() );
            pSigNode->SetAttribute( "path", cn.m_DstPath.c_str() );

            if (cn.m_Type == Signal_In)
            {
                pSigNode->SetAttribute( "dir", "in" );
            }
            else if (cn.m_Type == Signal_Out)
            {
                pSigNode->SetAttribute( "dir", "out" );
            }
            else if (cn.m_Type == Signal_InOut)
            {
                pSigNode->SetAttribute( "dir", "inout" );
            }
        }
        if (cn.m_pDstObj == pObj && !cn.m_bSrcInit)
        {
            TiXmlElement* pSigNode = new TiXmlElement( "signal" );
            pNode->LinkEndChild( pSigNode );

            pSigNode->SetAttribute( "attr", cn.m_pDstAttr->GetName() );
            pSigNode->SetAttribute( "path", cn.m_SrcPath.c_str() );

            if (cn.m_Type == Signal_In)
            {
                pSigNode->SetAttribute( "dir", "in" );
            }
            else if (cn.m_Type == Signal_Out)
            {
                pSigNode->SetAttribute( "dir", "out" );
            }
            else if (cn.m_Type == Signal_InOut)
            {
                pSigNode->SetAttribute( "dir", "inout" );
            }
        }
    }
    return nSig;
} // JPersistServer::SaveSignalsXML

void JPersistServer::SaveMetaTable( OutStream& os ) const
{
    TiXmlElement* pNode = new TiXmlElement( "meta" );
    int nClasses = g_pObjectServer->GetNClasses();
    JString value;
    for (int i = 0; i < nClasses; i++)
    {
        JMetaClass* pClass = g_pObjectServer->GetClass( i );
        TiXmlElement* pClassNode = new TiXmlElement( "class" );
        pNode->LinkEndChild( pClassNode );
        pClassNode->SetAttribute( "name", pClass->GetName() );
        pClassNode->SetAttribute( "parent", pClass->GetParentName() );
        JObject* pDefaultObj = pClass->GetTemplate();
        for (int j = 0; j < pClass->GetNOwnAttr(); j++)
        {
            TiXmlElement* pAttrNode = new TiXmlElement( "attribute" );
            pClassNode->LinkEndChild( pAttrNode );
            JClassAttr* pAttr = pClass->GetAttr( j );
            pAttrNode->SetAttribute( "name", pAttr->GetName() );
            pAttrNode->SetAttribute( "type", pAttr->GetType() );
            pDefaultObj->GetProperty( pAttr->GetName(), value );
            pAttrNode->SetAttribute( "default", value.c_str() );
            if (pAttr->IsDerived())
            {
                pAttrNode->SetAttribute( "derived", "true" );
            }
        }
    }

    //  write xml tree
    TiXmlPrinter printer;
	bool bRes = pNode->Accept( &printer );
    if (!bRes)
    {
        return;
    }
    os.Write( (const BYTE*)printer.CStr(), strlen( printer.CStr() ) );
    delete pNode;
} // JPersistServer::SaveMetaTable

bool JPersistServer::Save( JObject* pObject )
{
    ScriptResource* pScript = NULL;
    int nRes = m_ScriptResReg.size();
    for (int i = 0; i < nRes; i++)
    {
        ScriptResource& res = m_ScriptResReg[i];
        if (res.m_pRoot == pObject)
        {
            pScript = &res;
            break;
        }
    }
    if (pScript == NULL)
    {
        JString objPath;
        pObject->GetPath( objPath );
        rlog.warn( "PersistServer: could not save object <%s> into existing script", objPath.c_str() );
        return false;
    }
    FOutStream os( pScript->m_Path.c_str() );
    return Save( pObject, os, pScript->m_Format );
} // JPersistServer::Save

void JPersistServer::ReloadScripts()
{
    //  scan through scripts and check for hashes
    int nRes = m_ScriptResReg.size();
    for (int i = 0; i < nRes; i++)
    {
        ScriptResource& res = m_ScriptResReg[i];
        FInStream is;
        Path path( res.m_Path.c_str() );
        if (!is.OpenFile( path.GetFullPath() ) &&
            !g_pFileServer->OpenMedia( path.GetFile(), path.GetExt(), is ))
        {
            rlog.warn( "Could not reload script file: %s", path.GetFileExt() );
            continue;
        }

        if (res.m_pRoot == NULL)
        {
            continue;
        }

        int size = is.GetTotalSize();
        Buffer buf( size );
        is.Read( buf.GetData(), size );
        uint32_t hash = PHHash( buf.GetData(), size );
        if (hash == res.m_Hash)
        {
            //  file did not change
            continue;
        }

        MemInStream mis( buf.GetData(), size );
        JObject* pRoot = res.m_pRoot;
        bool bVisible = pRoot ? pRoot->IsVisible() : true;
        JObject* pObj = Load( mis, res.m_Format, pRoot, &hash );
        pObj->SetVisible( bVisible );
        res.m_pRoot = pObj;
        res.m_Hash = hash;
    }
}

void JPersistServer::SaveScripts()
{
    int nRes = m_ScriptResReg.size();
    for (int i = 0; i < nRes; i++)
    {
        const ScriptResource& res = m_ScriptResReg[i];
        Path path( res.m_Path.c_str() );
        FOutStream os( path.GetFullPath() );
        if (!os)
        {
            rlog.warn( "Could not save script file: %s", path.GetFileExt() );
            continue;
        }
        Save( res.m_pRoot, os, res.m_Format );
    }
}

