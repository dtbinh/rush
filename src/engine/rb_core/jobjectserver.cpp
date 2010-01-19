//****************************************************************************/
//  File:  JObjectServer.cpp
//****************************************************************************/
#include "precompile.h"
#include "path.h"
#include "jmlnode.h"
#include "ipersistserver.h"
#include "jobject.h"
#include "jcore.h"
#include "jobjectiterator.h"
#include "jupobjectiterator.h"
#include "jobjectserver.h"

JObjectServer* g_pObjectServer = &JObjectServer::inst();
//****************************************************************************/
/*  JObjectServer implementation
//****************************************************************************/
JObjectServer& JObjectServer::inst()
{
    static JObjectServer me;
    return me;
}

JObjectServer::JObjectServer()
{
    m_NumObjects = 0;
    m_NumDeleted = 0;
    g_pObjectServer = this;
} // JObjectServer::JObjectServer

JObjectServer::~JObjectServer()
{
    //  release metaclass templates
    JClassMap::iterator it = m_ClassMap.begin();
    while (it != m_ClassMap.end())
    {
        JMetaClass* pMeta = (*it).second;
        if (pMeta->GetTemplate())
        {
            pMeta->GetTemplate()->Release();
        }
        ++it;
    }
}

JMetaClass* JObjectServer::GetClass( int idx )
{
    JClassMap::iterator it = m_ClassMap.begin();
    for (int i = 0; i < idx; i++)
    {
        ++it;
    }
    return (*it).second;
}

JMetaClass* JObjectServer::FindClass( const char* cname )
{
    if (!cname || cname[0] == 0) return NULL;
    if (cname[0] == 'J' || cname[0] == 'j')
    {
        cname++;
    }
    JString s_Key;
    s_Key.set( cname );
    s_Key.to_lower();
    JClassMap::iterator it = m_ClassMap.find( s_Key );
    if (it != m_ClassMap.end())
    {
        return (*it).second;
    }
    return NULL;
}

JClassAttr* JObjectServer::FindClassAttr( const char* cname, const char* attr )
{
    if (!cname || cname[0] == 0)
    {
        return NULL;
    }

    if (cname[0] == 'J' || cname[0] == 'j')
    {
        cname++;
    }

    static JString s_Key;
    s_Key.set( cname );
    s_Key += attr;
    s_Key.to_lower();
    JAttrMap::iterator it = m_AttrMap.find( s_Key );
    if (it == m_AttrMap.end())
    {
         JMetaClass* pClass = FindClass( cname );
         if (!pClass)
         {
             return NULL;
         }
         const char* parentName = pClass->GetParentName();
         return FindClassAttr( parentName, attr );
    }
    return (*it).second;
}

JMetaClass* JObjectServer::RegClass( const char* cname, ObjCreator creator )
{
    static JString key;
    if (cname[0] == 'J' || cname[0] == 'j')
    {
        cname++;
    }
    key = cname;
    key.to_lower();

    JMetaClass* pMeta = FindClass( key.c_str() );
    if (pMeta)
    {
        rlog.warn( "Class <%s> was already registered!", cname );
        return pMeta;
    }
    key.to_lower();
    pMeta = m_ClassList.Allocate();
    m_ClassMap[key] = pMeta;
    pMeta->SetName( key.c_str() );

    if (creator)
    {
        pMeta->SetCreator( creator );
    }

    //  assign parent of the newly-added class
    JMetaClass* pParent = FindClass( pMeta->GetParentName() );
    if (pParent)
    {
        pMeta->SetParent( pParent );
    }

    //  check whether some of the already register classes refer to this class as to parent
    int nClasses = GetNClasses();
    for (int i = 0; i < nClasses; i++)
    {
        JMetaClass* pClass = GetClass( i );
        if (!stricmp( pClass->GetParentName(), cname ))
        {
            pClass->SetParent( pMeta );
        }
    }
    return pMeta;
}

JObject* JObjectServer::Create( const char* cname )
{
    static JString key;
    if (cname[0] == 'J' || cname[0] == 'j')
    {
        cname++;
    }
    key = cname;
    key.to_lower();

    JMetaClass* cd = FindClass( key.c_str() );
    JObject* pRes = NULL;
    if (cd) pRes = cd->Create();
    return pRes;
}

JObject* JObjectServer::CreateFromScript( const char* text, JObject* pSrc )
{
    JPersistFormat fmt = PersistFmt_JML;
    //  not really robust way to check whether it's XML
    const char* pstart = text + strcspn( text, " \n\r\t" );
    if (!pstart || *pstart == 0)
    {
        return NULL;
    }
    if (*pstart == '<')
    {
        fmt = PersistFmt_XML;
    }
    MemInStream is( (void*)text, strlen( text ) );
    JObject* pObj = g_pPersistServer->Load( is, fmt, pSrc );
    return pObj;
}

JObject* JObjectServer::CloneObject( const JObject* pObj, JObject* pParent, const char* name,
                                     bool bCloneSignals, bool bCloneChildren )
{
    MemOutStream os;
    g_pPersistServer->SaveBin( os, pObj );
    os.Close();

    MemInStream is( os.GetBuffer() );
    JObject* pNewObj = g_pPersistServer->LoadBin( is );

    //  FIXME:
    pNewObj->SetParent( pParent );
    if (name)
    {
        pNewObj->SetName( name );
    }

    return pNewObj;
}

bool JObjectServer::AreObjectsEqual( const JObject* pObj1, const JObject* pObj2, bool bWithChildren )
{
    if (pObj1 == pObj2) return true;
    if (pObj1 == NULL || pObj2 == NULL) return false;
    if (strcmp( pObj1->ClassName(), pObj2->ClassName() )) return false;
    if (strcmp( pObj1->GetName(), pObj2->GetName() )) return false;

    MemOutStream os1;
    MemOutStream os2;

    g_pPersistServer->SaveBin( os1, pObj1, bWithChildren, false );
    g_pPersistServer->SaveBin( os2, pObj2, bWithChildren, false );

    if (os1.GetTotalSize() != os2.GetTotalSize())
    {
        return false;
    }
    int res = memcmp( os1.GetBuffer(), os2.GetBuffer(), os1.GetTotalSize() );
    return (res == 0);
}

void JObjectServer::AutoNameObject( JObject* pObject )
{
    JString baseName = pObject->ClassName();
    baseName.to_lower();
    JString name = baseName;
    name += "00";
    int curIdx = 0;
    while (FindObject( name.c_str() ))
    {
        char buf[64];
        curIdx++;
        sprintf( buf, "%02d", curIdx );
        name = baseName;
        name += buf;
    }
    pObject->m_Name = name;
}

int JObjectServer::Register( JObject* pObject )
{
    if (!pObject)
    {
        return -1;
    }
    if (pObject->GetName()[0] == 0)
    {
        AutoNameObject( pObject );
    }
    static JString s_Key;
    s_Key.set( pObject->GetName() );
    s_Key.to_lower();
    m_ObjectMap.insert( JObjectMap::value_type( s_Key, pObject ) );
    m_NumObjects++;
    return m_NumObjects - 1;
}

bool JObjectServer::Unregister( JObject* pObject )
{
    //if (!pObject || pObject->GetName()[0] == 0) return false;
    static JString s_Key;
    s_Key.set( pObject->GetName() );
    s_Key.to_lower();
    JObjectMap::iterator it = m_ObjectMap.find( s_Key );
    JObject* pCandidate = NULL;
    while (it != m_ObjectMap.end())
    {
        pCandidate = (*it).second;
        if (!pCandidate->HasName( pObject->GetName() ))
        {
            return false;
        }
        if (pCandidate == pObject) break;
        ++it;
    }
    if (pCandidate != pObject)
    {
        return false;
    }
    m_ObjectMap.erase( it );
    m_NumObjects--;
    m_NumDeleted++;

    return true;
}

const JObject* FindCommonParent( const JObject* pObj1, const JObject* pObj2, int& len1, int& len2 )
{
    len1 = 0;
    const JObject* pParent1 = pObj1;
    while (pParent1)
    {
        const JObject* pParent2 = pObj2;
        len2 = 0;
        while (pParent2)
        {
            if (pParent1 == pParent2)
            {
                return pParent1;
            }
            pParent2 = pParent2->GetParent();
            len2++;
        }
        pParent1 = pParent1->GetParent();
        len1++;
    }
    return NULL;
}

bool HasPath( JObject* pObj, const char* path )
{
    if (pObj == NULL)
    {
        return (!path || path[0] == 0);
    }
    const char* pObjName = strrchr( path, '.' );
    if (!pObjName)
    {
        return pObj->HasName( path );
    }
    pObjName++;
    if (!pObj->HasName( pObjName ))
    {
        return false;
    }
    JString parentPath( path, pObjName - path - 1 );
    return HasPath( pObj->GetParent(), parentPath.c_str() );
}

JObject* JObjectServer::FindObject( const char* symPath, JObject* pRoot, JObject* pAnchor )
{
    static JString s_Path;
    static JString s_Type;
    static JString s_Key;

    //  extract type name
    JMetaClass* pTypeMeta = NULL;
    const char* pTypeDivider = strchr( symPath, ':' );
    if (pTypeDivider)
    {
        int nTypeSym = pTypeDivider - symPath;
        if (symPath[0] == 'j' || symPath[0] == 'J')
        {
            s_Type.set( symPath + 1, nTypeSym - 1 );
        }
        else
        {
            s_Type.set( symPath, nTypeSym );
        }
        s_Path.set( symPath + nTypeSym + 1 );
        pTypeMeta = FindClass( s_Type.c_str() );
    }
    else
    {
        s_Type = "*";
        s_Path = symPath;
    }
    bool bCheckType = (s_Type != "*");

    s_Path.to_lower();
    s_Type.to_lower();

    //  extract object name from the path
    bool bCheckPath = false;
    const char* pLastDot = strrchr( s_Path.c_str(), '.' );
    if (pLastDot)
    {
        s_Key = pLastDot + 1;
        bCheckPath = true;
    }
    else
    {
        s_Key = s_Path;
        bCheckPath = false;
    }

    if (!pAnchor)
    {
        pAnchor = pRoot;
    }

    JObject* pObject = NULL;
    if (s_Key == "*")
    {
        //  search for object with any name
        JObjectIterator nit( pAnchor );
        if (!bCheckType)
        {
            return pAnchor;
        }
        while (nit)
        {
            pObject = *nit;
            if (is_a( pObject, s_Type.c_str() ))
            {
                return pObject;
            }
            ++nit;
        }
        //  no object found so far, search upwards
        JUpObjectIterator upit( pAnchor );
        while (upit)
        {
            pObject = *upit;
            if (is_a( pObject, s_Type.c_str() ))
            {
                return pObject;
            }
            ++upit;
        }
        return NULL;
    }

    JObjectMap::const_iterator it = m_ObjectMap.find( s_Key );

    if (it == m_ObjectMap.end())
    {
        return NULL;
    }

    int ancLen = -1;
    int objLen = -1;

    //  pick object for which path to the first common parent is shortest
    int nCollisions = 0;
    while (it != m_ObjectMap.end())
    {
        JObject* pCandidate = (*it).second;
        ++it;
        if (!pCandidate->HasName( s_Key.c_str() ))
        {
            return pObject;
        }
        if (pRoot && !pCandidate->HasParent( pRoot ) && (pCandidate != pRoot))
        {
            continue;
        }
        if (bCheckType && !is_a( pCandidate, pTypeMeta ))
        {
            continue;
        }
        if (bCheckPath && !HasPath( pCandidate, s_Path.c_str()))
        {
            continue;
        }

        if (pAnchor)
        {
            int cAncLen = 0;
            int cObjLen = 0;
            FindCommonParent( pCandidate, pAnchor, cObjLen, cAncLen );
            if (cObjLen < objLen || (cObjLen == objLen && cAncLen < ancLen) || ancLen == -1)
            {
                objLen = cObjLen;
                ancLen = cAncLen;
                pObject = pCandidate;
            }
        }
        else
        {
            pObject = pCandidate;
        }
        nCollisions++;
    }
    return pObject;
}

void JObjectServer::DumpTree( const JObject* pObj, int indent )
{
    if (!pObj) return;
    static const int c_BufSize = 128;
    static char buf[c_BufSize];
    memset( buf, ' ', c_BufSize );
    sprintf( buf + indent, "%s[%d]\n", pObj->GetName(), pObj->GetNRef() );

    rlog.msg( buf );

    int nCh = pObj->GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        DumpTree( pObj->GetChild( i ), indent + 1 );
    }
}

void JObjectServer::DumpObjectMap()
{
    int nObj = 0;
    JObjectMap::iterator it = m_ObjectMap.begin();
    while (it != m_ObjectMap.end())
    {
        JObject* pObj = (*it).second;
        rlog.msg( "%s(%s):%X, nref:%d, nchildren:%d, parent:%X", pObj->GetName(), pObj->ClassName(),
            (uint32_t)pObj, pObj->GetNRef(), pObj->GetNChildren(), (uint32_t)pObj->GetParent() );
        nObj++;
        ++it;
    }
    rlog.msg( "Total objects in map: %d. Num alive: %d. Num deleted: %d.", nObj, m_NumObjects, m_NumDeleted );
}
