/***********************************************************************************/
//  File:   JToolTip.h
//  Date:   13.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JTOOLTIP_H__
#define __JTOOLTIP_H__

#include "JWidget.h"
#include "Timer.h"

/***********************************************************************************/
//  Class:  JToolTip
//  Desc:   Popup help tooltip
/***********************************************************************************/
class JToolTip : public JWidget
{
    Timer           m_ActiveTimer;      
    float           m_Timeout;      //  tooltip autohide after time

public:
                    JToolTip    () : m_Timeout( 5.0f ) {}
    float           GetTimeout  () const { return m_Timeout; }
    void            SetTimeout  ( float t ) { m_Timeout = t; }

    expose(JToolTip)
    {
        parent(JWidget);
        field( "Timeout", m_Timeout );
    }

}; // class JToolTip

#endif //__JTOOLTIP_H__