/*****************************************************************************/
/*	File:	xgrammar.l
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.04.2005
/*****************************************************************************/

%{
#include <math.h>
#define YY_INPUT(buf,result,max_size) {assert(g_pParser); result = g_pParser->yyInput(buf, max_size);}
%}

%option caseless
%option noyywrap
%option yylineno
%option never-interactive

ID				[a-zA-Z_.][a-zA-Z0-9_.]*
DIGIT			[0-9]
HEXDIGIT        [0-9a-fA-F]
QUOTEDSTRING	"L"?\"[^\"]*\"

%s InsideTag
%%

"/*"[^*/]+"*/"		{}
"//"[^\n]+"\n"		{}

"="						{	return tAssignment;			}
"{"						{	return tLeftBracket;		}
"}"						{	return tRightBracket;		}
";"                     {   return tSemicolon;          }
","                     {   return tComma;              }
">>"                    {   return tRightArrow;         }
"<<"                    {   return tLeftArrow;          }

{QUOTEDSTRING}			{	
							const char* pText = yytext + 1;
							int len = yyleng - 2;
							if (*pText == '"') { pText++; len--; }
							yylval.strID = CreatePooledString( pText, len );
							return tQuotedString;		
						}
[ \t\r\n]+				{	/*  skip blank symbols	*/	}
<InsideTag>[^<]+		{ 
							yylval.strID = CreatePooledString( yytext, yyleng );
							BEGIN(0); 
							return tInsideTag; 
						}
"-"?{DIGIT}+"."{DIGIT}+"f"?	{	
							yylval.strID = CreatePooledString( yytext, yyleng );
							return tNumber;					
						}
						
"-"?{DIGIT}+			{	
							yylval.strID = CreatePooledString( yytext, yyleng );
							return tNumber;					
						}
						
"0x"{HEXDIGIT}+			{	
							yylval.strID = CreatePooledString( yytext, yyleng );
							return tHexNumber;					
						}
						
{ID}					{	
							yylval.strID = CreatePooledString( yytext, yyleng );
							return tID;					
						}
"<<"{ID}			    {	
							yylval.strID = CreatePooledString( yytext, yyleng );
							return tID;					
						}
">>"{ID}			    {	
							yylval.strID = CreatePooledString( yytext, yyleng );
							return tID;					
						}
"<>"{ID}			    {	
							yylval.strID = CreatePooledString( yytext, yyleng );
							return tID;					
						}
						
"$"{ID}			        {	
							yylval.strID = CreatePooledString( yytext, yyleng );
							return tID;					
						}
						
.						{	/*  skip anything else	*/	}

%%

int isatty( int )
{
	return 1;
}

void	_yyundo()
{
	yyless(0);
	BEGIN(InsideTag);
	yychar = yylex();
}

const char*	_getcurpos()
{
	return yytext;
}


bool JMLParser::ParseBuffer( char* buffer )
{
	assert( buffer );
	Init();
	
	m_Buffer			= buffer;
	m_BufPtr			= buffer;
	g_pParser	= this;
	yyparse();
	return true;
} // JMLParser::ParseBuffer



