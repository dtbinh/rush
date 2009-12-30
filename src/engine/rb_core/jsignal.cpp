//****************************************************************************/
//  File:  JSignal.cpp
//****************************************************************************/
#include "stdafx.h"
#include "JSignal.h"

//****************************************************************************/
/*  JSignal implementation
//****************************************************************************/
void JSignal::CreateSymbolicNames()
{
    if (m_pSrcObj && m_pSrcAttr)
    {
        m_SrcPath = CreateSymbolicName( m_pSrcObj, m_pSrcAttr );
    }

    if (m_pDstObj && m_pDstAttr)
    {
        m_DstPath = CreateSymbolicName( m_pDstObj, m_pDstAttr );
    }
} // JSignal::CreateSymbolicNames

JString JSignal::CreateSymbolicName( JObject* pObj, JClassAttr* pAttr )
{
    assert( pObj && pAttr );
    JString res;
    pObj->GetPath( res );
    res += ".";
    res += pAttr->GetName();
    return res;
} // JSignal::CreateSymbolicName

bool JSignal::operator ==( const JSignal& c ) const
{
    if (m_pSrcObj && c.m_pSrcObj && m_pSrcObj != c.m_pSrcObj)     return false;
    if (m_pDstObj && c.m_pDstObj && m_pDstObj != c.m_pDstObj)     return false;
    if (m_pDstAttr && c.m_pDstAttr && m_pDstAttr != c.m_pDstAttr) return false;
    if (m_pSrcAttr && c.m_pSrcAttr && m_pSrcAttr != c.m_pSrcAttr) return false;

    if (m_pSrcObj && m_pSrcAttr &&
        m_pSrcObj == m_pDstObj && 
        m_pSrcAttr == m_pDstAttr)
    {
        return true;
    }
    
    if (!m_pSrcObj || !m_pDstObj)
    {
        if (m_SrcPath.size() != 0 && c.m_SrcPath.size() != 0 &&
            stricmp( m_SrcPath.c_str(), c.m_SrcPath.c_str() )) return false; 
        if (m_DstPath.size() != 0 && c.m_DstPath.size() != 0 &&
            stricmp( m_DstPath.c_str(), c.m_DstPath.c_str() )) return false; 
    }
    return true;
} // JSignal::operator ==

bool JSignal::Fire()
{
    if (!m_pSrcAttr || !m_pDstAttr) return false;

    int srcTypeID = m_pSrcAttr->GetTypeID();
    int dstTypeID = m_pDstAttr->GetTypeID();

    static JString val;
    if (m_pDstAttr->GetTypeID() == -1) 
    {
        //  destination is method
        m_pDstAttr->Call( m_pDstObj, m_pSrcObj->GetTag() );
    }
    else if (m_pSrcAttr->GetTypeID() == -1)
    {
        //  source is method, but destination is not - assign tag of the sender
        cast( val, m_pSrcObj->GetTag() );
        m_pDstAttr->Set( m_pDstObj, val, m_pSrcObj->GetTag() );
    }
    else if (srcTypeID == dstTypeID)
    {
        const int c_CloneAttrBufferSize = 1024;
        uint8_t buf[c_CloneAttrBufferSize];
        if (m_pSrcAttr->GetSizeOf() < c_CloneAttrBufferSize)
        {
            m_pSrcAttr->Get( m_pSrcObj, (void*)buf, m_pDstObj->GetTag() );
            m_pDstAttr->Set( m_pDstObj, (void*)buf, m_pSrcObj->GetTag() );
        }
    }
    else
    {
        m_pSrcAttr->Get( m_pSrcObj, val, m_pDstObj->GetTag() );
        m_pDstAttr->Set( m_pDstObj, val, m_pSrcObj->GetTag() );
    }

    return true;
} // JSignal::Fire


