//****************************************************************************/
//  File:  JClassAttr.cpp
//****************************************************************************/
#include "precompile.h"
#include "JClassAttr.h"

void JClassAttr::SetName( const char* name )
{
    m_Name = name;
    const char* pSep = strchr( name, ':' );
    if (pSep)
    {
        m_Name = JString( name, pSep - name );
        pSep++;
        while (*pSep)
        {
            char c = *pSep;
            if (c == 'd' || c == 'D')
            {
                m_bDerived = true;
            }
            pSep++;
        }
    }
}