//****************************************************************************/
//  File:	JMLNode.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	04.04.2003
//****************************************************************************/
#include "stdafx.h"
#include "Stream.h"
#include "JMLNode.h"
#include "JMLParser.h"

//****************************************************************************/
/*	JMLNode implementation
//****************************************************************************/
int	JMLNode::s_Indent = 0;

JMLNode::JMLNode( InStream& is )
{
    m_pChild		= NULL;	
    m_pAttr			= NULL;
    m_pParent		= NULL;	
    m_pNext			= NULL;	
    m_bComplete     = true;
    Read( is );
}

JMLNode::JMLNode()
{
    m_pChild		= NULL;	
    m_pAttr			= NULL;
    m_pParent		= NULL;	
    m_pNext			= NULL;	
    m_bComplete     = true;
}

JMLNode::JMLNode( char* buf )
{
    m_pChild		= NULL;	
    m_pAttr			= NULL;
    m_pParent		= NULL;	
    m_pNext			= NULL;	
    m_bComplete     = true;
    JMLParser parser( this );
    parser.ParseBuffer( buf );
}

JMLNode::~JMLNode()
{
    while (m_pChild)
    {
        JMLNode* pChild = m_pChild->NextSibling(); 
        delete m_pChild;
        m_pChild = pChild;
    }

    while (m_pAttr)
    {
        JMLNode* pAttr = m_pAttr->NextSibling(); 
        delete m_pAttr;
        m_pAttr = pAttr;
    }
} // JMLNode::~JMLNode

bool JMLNode::Read( InStream& is )
{
    if (!is) return false;
    bool allOK = false;
    int size = is.GetTotalSize();
    JMLParser parser( this );
    parser.SetFileName( is.GetName() );
    char* pData = new char[size + 1];
    pData[size] = 0;
    is.Read( pData, size );
    allOK = parser.ParseBuffer( pData );
    delete []pData;

    return allOK;
} // JMLNode::Read

const int c_IndentSize = 4;
char* Indent( char* buf, int nChars )
{
    for (int i = 0; i < nChars; i++) buf[i] = ' ';
    buf[nChars] = 0;
    return buf + nChars;
} // Indent

const int c_SaveBufSize = 1024*1024*4;
Buffer JMLNode::s_SaveBuffer;
void JMLNode::Write( OutStream& os, int level )
{
    s_SaveBuffer.Resize( c_SaveBufSize );
    char* pBuf = (char*)s_SaveBuffer.GetData();
    *pBuf = 0;
    Write( pBuf, level );
    os.Write( (BYTE*)pBuf, strlen( pBuf ) );
    s_SaveBuffer.Resize( 0 );
} // JMLNode::Write

char* JMLNode::Write( char* buf, int level )
{
    char* pBuf = buf; 

    pBuf = Indent( pBuf, level*c_IndentSize );
    const char* className = GetClass();
    if (className && className[0]) pBuf += sprintf( pBuf, "%s ", className );
    const char* nodeName = GetName();
    if (nodeName && nodeName[0]) pBuf += sprintf( pBuf, "\"%s\"", nodeName );

    if (!IsComplete())
    {
        pBuf += sprintf( pBuf, ";\n" );
        return pBuf;
    }

    pBuf += sprintf( pBuf, "\n" );
    pBuf = Indent( pBuf, level*c_IndentSize );
    pBuf += sprintf( pBuf, "{\n" );
    
    //  count max attribute string length 
    JMLNode* pAttr = m_pAttr;
    int maxSize = 0;
    while (pAttr)
    {
        int len = strlen( pAttr->GetName() );
        if (maxSize < len) maxSize = len;
        pAttr = pAttr->NextSibling();
    }

    pAttr = m_pAttr;
    maxSize += maxSize%c_IndentSize + c_IndentSize;
    while (pAttr)
    {
        int len = strlen( pAttr->GetName() );
        pBuf = Indent( pBuf, (level + 1)*c_IndentSize );
        pBuf += sprintf( pBuf, "%s", pAttr->GetName() );
        pBuf = Indent( pBuf, maxSize - len );
        pBuf += sprintf( pBuf, "= %s;\n", pAttr->GetValue() );
        pAttr = pAttr->NextSibling();
    }
    
    JMLNode* pChild = m_pChild;
    while (pChild)
    {
        pBuf = pChild->Write( pBuf, level + 1 );
        pChild = pChild->NextSibling();
    }

    pBuf = Indent( pBuf, level*c_IndentSize );
    pBuf += sprintf( pBuf, "}\n" );
    return pBuf;
} // JMLNode::Write

JMLNode* JMLNode::FindChild( const char* tag ) const
{
    JMLNode* cNode = m_pChild;
    while (cNode)
    {
        if (cNode->HasName( tag )) return cNode;
        cNode = cNode->m_pNext;
    }
    return NULL;
} // JMLNode::FindChild

JMLNode* JMLNode::FindAttr( const char* name ) const
{
    JMLNode* cNode = m_pAttr;
    while (cNode)
    {
        if (cNode->HasName( name )) return cNode;
        cNode = cNode->m_pNext;
    }
    return NULL;
} // JMLNode::FindAttr

JMLNode* JMLNode::AddChild( const char* className, const char* name )
{
    JMLNode* newNode = new JMLNode();
    newNode->m_pParent = this;
    newNode->SetClass( className );
    newNode->SetName( name );
    if (!m_pChild) { m_pChild = newNode; return newNode; }
    JMLNode* pHead = m_pChild;
    while (pHead->m_pNext) pHead = pHead->m_pNext;
    pHead->m_pNext = newNode;
    return newNode;
} // JMLNode::AddChild

JMLNode* JMLNode::AddChild( JMLNode* pChild )
{
    if (!m_pChild) { m_pChild = pChild; return pChild; }
    JMLNode* pHead = m_pChild;
    while (pHead->m_pNext) pHead = pHead->m_pNext;
    pHead->m_pNext = pChild;
    return pChild;
}

int	JMLNode::GetNChildren() const
{
    int nCh = 0;
    JMLNode* pHead = m_pChild;
    while (pHead){ pHead = pHead->m_pNext; nCh++; }
    return nCh;
}

int	JMLNode::GetNAttr() const
{
    int nCh = 0;
    JMLNode* pHead = m_pAttr;
    while (pHead){ pHead = pHead->m_pNext; nCh++; }
    return nCh;
}

JMLNode* JMLNode::GetChild( int idx )
{
    JMLNode* pChild = FirstChild();
    while (idx-- > 0 && pChild) pChild = pChild->NextSibling();
    return pChild;
} // JMLNode::GetChild

JMLNode* JMLNode::AddAttr( const char* name )
{
    JMLNode* newAttr = new JMLNode();
    newAttr->SetName( name );
    newAttr->m_pParent = this;
    if (!m_pAttr) { m_pAttr = newAttr; return newAttr; }
    JMLNode* pHead = m_pAttr;
    while (pHead->m_pNext) pHead = pHead->m_pNext;
    pHead->m_pNext = newAttr;
    return newAttr;
} // JMLNode::AddAttr

void JMLNode::AddValue( const char* val )
{
    if (m_Value.size() > 0) m_Value += ", ";
    m_Value += val;
} // JMLNode::AddValue





