//****************************************************************************/
//  File:	JMLParser.h
//  Desc:	Simplified parser of the jml files
//****************************************************************************/
#ifndef __JMLPARSER_H__
#define __JMLPARSER_H__

#include <stack>
#include <string>

class JMLNode;
//****************************************************************************/
//  Class:	JMLParser
//  Desc:	jml file parsing handler
//****************************************************************************/
class JMLParser
{
public:
					JMLParser		();
                    JMLParser		( JMLNode* pRoot );
	virtual			~JMLParser		(){}

	bool			ParseFile		( const char* fName			);
	bool			ParseBuffer		( char* buffer				);
	int				yyInput			( char* buf, int max_size	);
	void			Error			( char* msg, int curLine	);

    void            SetFileName     ( const char* fname         );

	void			SetErrorTokenPtr( char* pErrToken )	{ m_pErrToken = pErrToken; }
	void			SetCurLine		( int line		  )	{ m_CurLine = line; }
	const char*		GetCurLocation	() const;


	virtual void	StartObject		( const char* className, const char* objName = NULL );
	virtual void	EndObject       ( bool bComplete = true );
	virtual void	StartAttribute  ( const char* name );
    virtual void	AddAttrValue    ( const char* val );

    virtual void	Init()			{ if (!m_bInited) OnInit(); m_bInited = true; }

protected:
	virtual void	OnInit()		{}

	bool			m_bInited;

private:
    JMLNode*                m_pRoot;
    JMLNode*				m_pCurNode;
    JMLNode*				m_pCurAttr;
    std::stack<JMLNode*>	m_NodeStack;

    char*			        m_Buffer;
	char*			        m_BufPtr;
	char*			        m_pErrToken;
	int				        m_CurLine;
	char			        m_FileName[_MAX_PATH];

}; // class JMLParser

const char* GetPooledString( uint32_t id );
uint32_t       CreatePooledString( const char* pStr, int len );
void        ClearStringPool();

#endif // __JMLPARSER_H__
