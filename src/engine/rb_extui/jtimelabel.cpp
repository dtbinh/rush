//****************************************************************************/
//  File:   JTimeLabel.cpp
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "JLabel.h"
#include "JTimeLabel.h"

//****************************************************************************/
/*  JTimeLabel implementation
//****************************************************************************/
decl_class(JTimeLabel);
JTimeLabel::JTimeLabel()
{
    m_bBackward     = false;
    m_TimeLimit     = 0.0f;
    m_bShowHours    = false;
    m_bShowMinutes  = true;
    m_bShowSeconds  = true;

    m_Seconds       = 0.0f;
} // JTimeLabel::JTimeLabel 

void JTimeLabel::SetSeconds( float val )
{ 
    float cTime = val;
    if (m_bBackward) cTime = m_TimeLimit - val;
    if (cTime < 0.0f) cTime = 0.0f;

    int nSecTotal   = int( cTime );
    int nMinTotal   = nSecTotal/60;
    int nSec        = nSecTotal%60;
    int nHours      = nMinTotal/60;
    int nMin        = nMinTotal%60;
    char timeStr[256];
    char* pStr = timeStr;
    if (m_bShowHours)
    {
        sprintf( pStr, "%02d", nHours );
        pStr += strlen( pStr );
    }
    if (m_bShowMinutes || m_bShowSeconds)
    {
        if (m_bShowHours)
        {
            *pStr = ':';
            pStr++;
            *pStr = 0;
        }
        sprintf( pStr, "%02d", nMin );
        pStr += strlen( pStr );
    }

    if (m_bShowSeconds)
    {
        if (m_bShowMinutes)
        {
            *pStr = ':';
            pStr++;
            *pStr = 0;
        }
        sprintf( pStr, "%02d", nSec );
        pStr += strlen( pStr );
    }
    
    SetText( timeStr );

    m_Seconds = val; 
} // JTimeLabel::SetSeconds
