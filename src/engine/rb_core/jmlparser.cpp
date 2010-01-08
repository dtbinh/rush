//****************************************************************************/
//  File:	JMLParser.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	04.04.2003
//****************************************************************************/
#include "precompile.h"
#include "JMLNode.h"
#include "JMLParser.h"

//****************************************************************************/
/*	JMLParser implementation
//****************************************************************************/
JMLParser::JMLParser()
{
	m_Buffer		= NULL;
	m_pErrToken		= 0;
	m_FileName[0]	= 0;
	m_bInited		= false;
    m_pCurNode      = NULL;
    m_pCurAttr      = NULL;
} // JMLParser::JMLParser

JMLParser::JMLParser( JMLNode* pRoot )
{
    m_Buffer		= NULL;
    m_pErrToken		= 0;
    m_FileName[0]	= 0;
    m_bInited		= false;
    m_pRoot         = pRoot;
    m_pCurAttr      = NULL;
} // JMLParser::JMLParser

bool JMLParser::ParseFile( const char* fName )
{
	strcpy( m_FileName, fName );
	FILE* fp = fopen( m_FileName, "rb" );
	if (!fp) return false;
	fseek( fp, 0, SEEK_END );
	int fileSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );
	char* buf = new char[fileSize + 1];
	fread( buf, fileSize, 1, fp );
	fclose( fp );
	buf[fileSize] = 0;

    ClearStringPool();

	ParseBuffer( buf );

    ClearStringPool();
	delete []buf;
	return true;
} // JMLParser::ParseFile

const char* JMLParser::GetCurLocation() const
{
	static const int c_MaxLocationStr = 256;
	static char buf[c_MaxLocationStr];
	buf[0] = 0;
	return buf;
}

int	JMLParser::yyInput( char* buf, int max_size )
{
	assert( m_Buffer );
	int nSym = 0;
	while (*m_BufPtr && *m_BufPtr != '\n')
	{
		buf[nSym++] = *(m_BufPtr++);
		if (nSym >= max_size)
        {
            rlog.err( "Too long JML input buffer." );
            return nSym;
        }
	}
	if (*m_BufPtr == '\n')
	{
		buf[nSym++] = '\n';
		m_BufPtr++;
	}

	return nSym;
} // JMLParser::yyInput

void JMLParser::StartObject( const char* className, const char* objName )
{
    JMLNode* pNode = NULL;
    if (m_NodeStack.size() > 0)
    {
        JMLNode* pTop = m_NodeStack.top();
        pNode =  pTop->AddChild( className, objName );
    }
    else
    {
        m_pRoot->SetClass( className );
        m_pRoot->SetName ( objName );
        pNode = m_pRoot;
    }
    m_NodeStack.push( pNode );
    m_pCurNode = pNode;
} // JMLParser::StartObject

void JMLParser::EndObject( bool bComplete )
{
    if (m_pCurNode) m_pCurNode->SetComplete( bComplete );
    if (m_NodeStack.size() == 0) { m_pCurNode = NULL; return; }
    m_NodeStack.pop();
    m_pCurNode = (m_NodeStack.size() > 0) ? m_NodeStack.top() : NULL;
} // JMLParser::EndObject

void JMLParser::StartAttribute( const char* name )
{
    if (m_pCurNode) m_pCurAttr = m_pCurNode->AddAttr( name );
} // JMLParser::OnAttribute

void JMLParser::AddAttrValue( const char* val )
{
    if (m_pCurAttr) m_pCurAttr->AddValue( val );
} // JMLParser::AddAttrValue

void JMLParser::Error( char* msg, int curLine )
{
    rlog.err( "%s(%d): Error in script - %s", m_FileName, curLine, msg );
} // JMLParser::Error

void JMLParser::SetFileName( const char* fname )
{
    strcpy( m_FileName, fname );
} // JMLParser::SetFileName

//****************************************************************************/
/*	String pooling
//****************************************************************************/

static std::vector<const char*>    s_StrPool;
const char* GetPooledString( uint32_t id )
{
    if (id >= s_StrPool.size()) return "";
    return s_StrPool[id];
}

uint32_t CreatePooledString( const char* pStr, int len )
{
    if (len == -1)
    {
        uint32_t id = s_StrPool.size();
        s_StrPool.push_back( "" );
        return id;
    }
    char* pAllocStr = new char[len + 1];
    strncpy( pAllocStr, pStr, len );
    pAllocStr[len] = 0;
    uint32_t id = s_StrPool.size();
    s_StrPool.push_back( pAllocStr );
    return id;
}

void ClearStringPool()
{
    int nStr = s_StrPool.size();
    for (int i = 0; i < nStr; i++) delete []s_StrPool[i];
    s_StrPool.clear();
}






