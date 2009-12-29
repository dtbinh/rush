//****************************************************************************/
//  File:  CommandLine.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "CommandLine.h"

//****************************************************************************/
/*  CommandLine implementation
//****************************************************************************/
CommandLine::CommandLine( const char* cmdLine )
{
    Parse( cmdLine );
} // CommandLine::CommandLine

void CommandLine::Parse( const char* cmdLine )
{
    if (!cmdLine) return;
    m_Params.clear();
    const char* pCh = cmdLine;
    while (true)
    {
        pCh = strstr( pCh, "--" );
        if (!pCh) break;
        m_Params.push_back( Parameter() );
        Parameter& param = m_Params.back();
        pCh += 2;
        const char* pChEnd = pCh + strcspn( pCh, "= " );
        param.m_Name = JString( pCh, pChEnd - pCh );
        if (*pChEnd == '=')
        {
            // parameter has value
            pChEnd++;
            bool bQuoted = false;
            if (*pChEnd == '"')
            {
                pChEnd++;
                bQuoted = true;
            }
            pCh = pChEnd;
            pChEnd = pCh + strcspn( pCh, bQuoted ? "\"" : " " );
            param.m_Value = JString( pCh, pChEnd - pCh );
            pChEnd++;
        }
        pCh = pChEnd;
    }
}

const char* CommandLine::GetValue( const char* name ) const
{
    int nParam = m_Params.size();
    for (int i = 0; i < nParam; i++)
    {
        if (!strcmp( m_Params[i].m_Name.c_str(), name ))
        {
            return m_Params[i].m_Value.c_str();
        }
    }
    return NULL;
}