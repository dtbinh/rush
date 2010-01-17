/*****************************************************************************/
/*	File:	xgrammar.y
/*	Desc:	.x grammar for BISON input
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.04.2005
/*****************************************************************************/

%{
#include "rb_common.h"
#include <math.h>
#include <malloc.h>
#include <string.h>
#include "JMLParser.h"

//  disable some MSVC7 pain in the arse...
#pragma runtime_checks( "", off )
#pragma warning ( disable : 4102 )
#pragma warning ( disable : 4060 )

int		yylex();
int		yyerror(char *s);
void	_yyundo();
const	char*	_getcurpos();
void	_setcurpos( char* pos );

#define YYDEBUG 1
#define YYERROR_VERBOSE
static JMLParser* g_pParser = NULL;
unsigned int curTag;

%}

/* BISON Declarations */

%union{
	unsigned int	strID;
}

%token				tAssignment
%token				tLeftBracket
%token 				tQuotedString
%token				tRightBracket
%token				tInsideTag
%token              tSemicolon
%token				tNumber
%token				tHexNumber
%token				tComma
%token				tID
%token				tRightArrow
%token				tLeftArrow

/* jml file grammar */
%%

rFile:			    rBlockList
;
rBlockList:			|
					| rBlock
					| rBlock rBlockList
;
rBlock:				rBlockHeader rBlockBody
					| rBlockHeader rBlockBody tSemicolon
;
rBlockHeader:       tID tQuotedString
					{
						g_pParser->StartObject( GetPooledString( $1.strID ), GetPooledString( $2.strID ) );
					}
					| tID
					{
						g_pParser->StartObject( GetPooledString( $1.strID ), "" );
					}
;

rBlockBody:         {
						g_pParser->EndObject( false );
					}
                    | tLeftBracket rAttrList tRightBracket
					{
						g_pParser->EndObject( true );
					}
;
rAttrList:          | rAttribute
                    | rAttribute rAttrList
;
rAttribute:         rTermAttr
                    | rComplexAttr
                    | rTermAttr tSemicolon
;
rTermAttr:          tID
					{
						g_pParser->StartAttribute( GetPooledString( $1.strID ) );
					}
					tAssignment rAttrValue
;
rComplexAttr:       rBlock
;
rAttrValue:         | tID
					{
						g_pParser->AddAttrValue( GetPooledString( $1.strID ) );
					}
                    | tQuotedString
                    {
						g_pParser->AddAttrValue( GetPooledString( $1.strID ) );
					}
                    | tNumber
                    {
						g_pParser->AddAttrValue( GetPooledString( $1.strID ) );
					}
                    | tHexNumber
                    {
						g_pParser->AddAttrValue( GetPooledString( $1.strID ) );
					}
                    | rAttrArray
;
rAttrArray:         tLeftBracket  rAttrArrayList tRightBracket
;
rAttrArrayList:     | rAttrValue
                    | rAttrArrayList tComma rAttrValue
;
%%

extern int yylineno;
extern char *yytext;
int yyerror(char *s)
{
	assert( g_pParser );
	char errb[1024];
	sprintf( errb, "%s. Stopped at token (%s)", s, yytext );
	g_pParser->Error( errb, yylineno );
	return 0;
}
