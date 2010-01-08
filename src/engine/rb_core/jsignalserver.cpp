//****************************************************************************/
//  File:  JSignalServer.cpp
//****************************************************************************/
#include "precompile.h"
#include "JMLNode.h"
#include "JSignalServer.h"

//****************************************************************************/
/*  JSignalServer implementation
//****************************************************************************/
decl_class(JSignalServer);
JSignalServer* JSignalServer::s_pInstance = NULL;
JSignalServer::JSignalServer() 
    : m_CurToken( 1 ), 
      m_pCurSrcObj( NULL ) 
{
    SetName( "signal" );
    s_pInstance     = this;
    g_pSignalServer = this;
    m_bLogSignals   = false;
}

JSignalServer::~JSignalServer()
{
    if (s_pInstance == this) s_pInstance = NULL;
}

void JSignalServer::Render()
{
    
}

bool JSignalServer::Connect( JObject* pSrcObj, JClassAttr* pSrcAttr, 
                              JObject* pDstObj, JClassAttr* pDstAttr )
{
    JSignal* pSig = m_SignalPool.Allocate();
    pSig->m_pSrcObj    = pSrcObj;
    pSig->m_pDstObj    = pDstObj;
    pSig->m_pSrcAttr   = pSrcAttr;
    pSig->m_pDstAttr   = pDstAttr;
    pSig->CreateSymbolicNames();

    //  look if there already is such signal
    if (FindSignal( pSig ))
    {
        m_SignalPool.Free( pSig );
        return false;
    }

    RegSignal( pSig, true  );
    RegSignal( pSig, false );
    return true;
} // JSignalServer::Connect

bool JSignalServer::ConnectSrc( JObject* pSrcObj, JClassAttr* pSrcAttr, const char* dst )
{
    JSignal* pSig = m_SignalPool.Allocate();
    pSig->m_pDstObj        = NULL;
    pSig->m_pDstAttr       = NULL;
    pSig->m_DstPath        = dst;
    pSig->m_pSrcObj        = pSrcObj;
    pSig->m_pSrcAttr       = pSrcAttr;
    pSig->m_Type           = Signal_Out;
    pSig->m_bSrcInit       = true;
    pSig->CreateSymbolicNames();

    //  look if there already is such signal
    if (FindSignal( pSig ))
    {
        m_SignalPool.Free( pSig );
        return false;
    }

    RegSignal( pSig, true  );

    bool bRes = Connect( pSig );
    return bRes;
} // JSignalServer::ConnectSrc

bool JSignalServer::ConnectDst( JObject* pDstObj, JClassAttr* pDstAttr, const char* src )
{
    JSignal* pSig = m_SignalPool.Allocate();
    pSig->m_pSrcObj        = NULL;
    pSig->m_pSrcAttr       = NULL;
    pSig->m_pDstObj        = pDstObj;
    pSig->m_pDstAttr       = pDstAttr;
    pSig->m_SrcPath        = src;
    pSig->m_Type           = Signal_In;
    pSig->m_bSrcInit       = false;
    pSig->CreateSymbolicNames();

    //  look if there already is such signal
    if (FindSignal( pSig ))
    {
        m_SignalPool.Free( pSig );
        return false;
    }

    RegSignal( pSig, false );
    
    bool bRes = Connect( pSig );
    return bRes;
} // JSignalServer::ConnectDst

void JSignalServer::CloneSignals( JObject* pSrc, JObject* pDst )
{
    if (pSrc == NULL || pDst == NULL || pDst == pSrc)
    {
        return;
    }

    //  gather all signals to clone
    JSignalReg::const_iterator it = m_SignalReg.find( pSrc );
    static std::vector<JSignal*> s_CloneSig;
    s_CloneSig.clear();
    while (it != m_SignalReg.end() && (*it).first == pSrc)
    {
        JSignal* pCurSig = (*it).second;
        if (!pCurSig->m_bConnected) Connect( pCurSig );
        s_CloneSig.push_back( pCurSig );
    }

    int nSig = s_CloneSig.size();
    for (int i = 0; i < nSig; i++)
    {
        JSignal* pCurSig = s_CloneSig[i];
        JSignal ncn( *pCurSig );

        if (pCurSig->m_pSrcObj == pSrc)
        {
            ncn.m_pSrcObj       = pDst;
            ncn.m_pDstObj       = NULL;
        }
        if (pCurSig->m_pDstObj == pSrc)
        {
            ncn.m_pDstObj       = pDst;
            ncn.m_pSrcObj       = NULL;
        }

        ncn.m_bConnected    = false;
        ncn.CreateSymbolicNames();

        if (FindSignal( &ncn ) == NULL)
        {
            JSignal* pNewSig = m_SignalPool.Allocate();
            *pNewSig = ncn;
            RegSignal( pNewSig, ncn.m_pSrcObj ? true : false );
        }
    }
} // JSignalServer::CloneSignals

bool JSignalServer::Connect( JObject* pSrcObj, const char* pSrcAttr, 
                              JObject* pDstObj, const char* pDstAttr )
{
    if (!pSrcObj || !pDstObj) return false;
    JMetaClass* pSrcMeta = g_pObjectServer->FindClass( pSrcObj->ClassName() );
    JMetaClass* pDstMeta = g_pObjectServer->FindClass( pDstObj->ClassName() );

    if (!pSrcMeta || !pDstMeta) return false;
    JClassAttr* pSAttr = pSrcMeta->FindAttr( pSrcAttr );
    JClassAttr* pDAttr = pDstMeta->FindAttr( pDstAttr );
    if (!pSAttr || !pDAttr) return false;
    
    return Connect( pSrcObj, pSAttr, pDstObj, pDAttr );
} // JSignalServer::Connect

bool JSignalServer::SendSignal( JObject* pSrcObj, const char* pSrcAttr )
{
    JSignalReg::iterator it = m_SignalReg.find( pSrcObj );
    if (it == m_SignalReg.end())
    {
        return false;
    }
    JObject* pCurObj = (*it).first;
    while (pCurObj == pSrcObj)
    {
        JSignal& cn = *((*it).second);
        ++it;
        pCurObj = (it == m_SignalReg.end()) ? NULL : (*it).first;
        if (cn.m_pSrcObj != pSrcObj)
        {
            continue;
        }
        if (pSrcAttr && cn.m_pSrcAttr && !cn.m_pSrcAttr->HasName( pSrcAttr )) 
        {
            continue;
        }
        if (cn.m_Token == m_CurToken) 
        {
            continue;
        }
        if (!cn.m_bConnected && !Connect( &cn ))
        {
            continue;
        }
        if (cn.m_pDstAttr == NULL)
        {
            assert( cn.m_pDstObj && cn.m_pSrcObj && cn.m_pSrcAttr );
            const char* pAttrName = strrchr( cn.m_DstPath.c_str(), '.' );
            if (*pAttrName == '.') 
            {
                pAttrName++;
            }
            cn.m_pDstObj->OnSignal( cn.m_pSrcObj, cn.m_pSrcAttr->GetName(), pAttrName );
        }
        cn.m_Token = m_CurToken;
        m_pCurSrcObj = pSrcObj;
        m_CurSrcAttr = pSrcAttr;
        cn.Fire();
        if (m_bLogSignals)
        {
            rlog.msg( "Signal: %s -> %s", cn.m_SrcPath.c_str(), cn.m_DstPath.c_str() );
        }
    }
    m_CurToken++;
    return true;
} // JSignalServer::SendSignal

bool JSignalServer::PollSlot( JObject* pDstObj, const char* pDstAttr )
{
    JSignalReg::iterator it = m_SignalReg.find( pDstObj );
    while (it != m_SignalReg.end() && (*it).first == pDstObj)
    {
        JSignal& cn = *((*it).second);
        ++it;
        if (cn.m_pDstObj != pDstObj) continue;
        if (pDstAttr && cn.m_pDstAttr && !cn.m_pDstAttr->HasName( pDstAttr )) continue;
        if (cn.m_Token == m_CurToken || (!cn.m_bConnected && !Connect( &cn ))) continue;
        cn.m_Token = m_CurToken;
        m_pCurSrcObj = cn.m_pSrcObj;
        m_CurSrcAttr = cn.m_pSrcAttr->GetName();
        if (!cn.m_bConnected)
        {
            Connect( &cn );
        }
        cn.Fire();
        if (m_bLogSignals)
        {
            rlog.msg( "Signal: %s <- %s", cn.m_DstPath.c_str(), cn.m_SrcPath.c_str() );
        }
        return true;
    }
    m_CurToken++;
    return true;
} // JSignalServer::PollSlot

int JSignalServer::Connect( JObject* pObj )
{
    int nFound = 0;
    JSignalReg::iterator it = m_SignalReg.find( pObj );
    while (it != m_SignalReg.end() && (*it).first == pObj)
    {
        JSignal& cn = *((*it).second);
        ++it;
        if (cn.m_pDstObj != pObj && cn.m_pSrcObj != pObj) continue;
        nFound++;
        if (!cn.m_bConnected)
        {
            Connect( &cn );
        }
    }
    return nFound;
} // JSignalServer::Connect

void JSignalServer::GetSignalList( const JObject* pObj, std::vector<JSignal*>& sigList ) const
{
    sigList.clear();
    if (pObj == NULL) return;

    JSignalReg::const_iterator it = m_SignalReg.find( (JObject*)pObj );
    while (it != m_SignalReg.end() && (*it).first == pObj)
    {
        sigList.push_back( (*it).second );
        ++it;
    }
} // JSignalServer::GetSignalList

int JSignalServer::ClearSignals( JObject* pObj )
{
    int nFound = 0;
    //  remove all registry entries for this object
    JSignalReg::iterator it = m_SignalReg.find( pObj );
    while (it != m_SignalReg.end() && (*it).first == pObj)
    {
        JSignal* pCn = (*it).second;
        JObject* pOtherObj = NULL;
        if (pCn->m_pSrcObj == pObj) 
        {
            pOtherObj = pCn->m_pDstObj;
        }
        else if (pCn->m_pDstObj == pObj) 
        {
            pOtherObj = pCn->m_pSrcObj;
        }
        else
        {
            assert( false );
        }
        m_SignalReg.erase( it );
        if (pOtherObj != NULL)
        {
            //  find reference to the other end of the connection, and remoe it from the registry
            JSignalReg::iterator nit = m_SignalReg.find( pOtherObj );
            while (nit != m_SignalReg.end() && (*nit).first == pOtherObj)
            {
                JSignal* pCurCn = (*nit).second;
                if (pCurCn == pCn)
                {
                    m_SignalReg.erase( nit );
                    break;
                }
                ++nit;
            }
        }

        m_SignalPool.Free( pCn );
        nFound++;

        it = m_SignalReg.find( pObj );
    }
    return nFound;
} // JSignalServer::ClearSignals

JSignal* JSignalServer::FindSignal( const JSignal* pSig )
{
    JSignalReg::iterator it = m_SignalReg.begin();
    while (it != m_SignalReg.end())
    {
        JSignal* pCurSig = (*it).second;
        if (*pSig == *pCurSig) return pCurSig;
        ++it;
    }
    return NULL;
} // JSignalServer::FindSignal

bool JSignalServer::RegSignal( JSignal* pSignal, bool bSource )
{
    JObject* pObj = bSource ? pSignal->m_pSrcObj : pSignal->m_pDstObj;
    if (!pObj) return false;
    m_SignalReg.insert( JSignalReg::value_type( pObj, pSignal ) );
    return true;
} // JSignalServer::RegSignal

bool JSignalServer::Connect( JSignal* pSignal )
{
    if (!pSignal) return false;

    if (!pSignal->m_pSrcObj || !pSignal->m_pSrcAttr)
    {
        //  find source
        if (!LocateSlot( pSignal->m_SrcPath.c_str(), pSignal->m_pSrcObj, pSignal->m_pSrcAttr, 
            pSignal->m_pDstObj )) 
        {
            return false;
        }
        RegSignal( pSignal, true );
    }

    if (!pSignal->m_pDstObj || !pSignal->m_pDstAttr)
    {
        //  find destination
        if (!LocateSlot( pSignal->m_DstPath.c_str(), pSignal->m_pDstObj, pSignal->m_pDstAttr, 
            pSignal->m_pSrcObj )) 
        {
            return false;
        }
        RegSignal( pSignal, false );
    }

    pSignal->m_bConnected = true;
    return true;
} // JSignalServer::Connect

bool JSignalServer::LocateSlot( const char* symPath, JObject* &pObj, JClassAttr* &pAttr, JObject* pContext )
{
    const char* pAttrName = strrchr( symPath, '.' );
    if (!pAttrName) pAttrName = symPath;
    int nObjChars = pAttrName - symPath;
    if (*pAttrName == '.') pAttrName++;
    JString objName( symPath, nObjChars );
    pObj = g_pObjectServer->FindObject( objName.c_str(), NULL, pContext );
    if (!pObj)
    {
        //  search for property only (for all parents as well)
        pObj = pContext;
        while (pObj)
        {
            JMetaClass* pMeta = g_pObjectServer->FindClass( pObj->ClassName() );
            if (!pMeta) 
            {
                return false;
            }
            pAttr = pMeta->FindAttr( pAttrName );

            if (pAttr)
            {
                return true;
            }
            pObj = pObj->GetParent();
        }
        return false;
    }
    pAttr = g_pObjectServer->FindClassAttr( pObj->ClassName(), pAttrName );
    return true;
}  

JStringList JSignalServer::GetSignalList() const
{
    JStringList res;
    JSignalReg::const_iterator it = m_SignalReg.begin();
    JString srcPath, dstPath, sig;
    while (it != m_SignalReg.end())
    {
        JSignal& cn = *((*it).second);
        srcPath = "?";
        dstPath = "?";
        if (cn.m_pSrcObj) 
        {
            cn.m_pSrcObj->GetPath( srcPath );
        }

        if (cn.m_pDstObj) 
        {
            cn.m_pDstObj->GetPath( dstPath );
        }
        if (cn.m_bSrcInit)
        {
            sig = srcPath;
            sig += ":";
            sig += cn.m_pSrcAttr ? cn.m_pSrcAttr->GetName() : "?";
            sig += " -> ";
            sig += dstPath;
            sig += ":";
            sig += cn.m_pDstAttr ? cn.m_pDstAttr->GetName() : "?";
        }
        else
        {
            sig = dstPath;
            sig += ":";
            sig += cn.m_pDstAttr ? cn.m_pDstAttr->GetName() : "?";
            sig += " <- ";
            sig += srcPath;
            sig += ":";
            sig += cn.m_pSrcAttr ? cn.m_pSrcAttr->GetName() : "?";
        }
        res.push_back( sig );
        ++it;
    }
    return res;
} // JSignalServer::GetSignalList

void JSignalServer::DumpConnections() 
{
    JStringList sigList = GetSignalList();
    rlog.msg( "Dumping connections: %d entries, %d connections", sigList.size(), m_SignalPool.GetNElem() );
    for (int i = 0; i < sigList.size(); i++)
    {
        rlog.msg( "%d: %s", i, sigList[i].c_str() );
    }
} // JSignalServer::DumpConnections

