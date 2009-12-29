//****************************************************************************/
//  File:   JTimeLabel.h
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JTIMELABEL_H__
#define __JTIMELABEL_H__

#include "JLabel.h"

//****************************************************************************/
//  Class:  JTimeLabel
//  Desc:   Label which shows time value
//****************************************************************************/
class JTimeLabel : public JLabel
{
    bool            m_bBackward;
    float           m_TimeLimit;
    bool            m_bShowHours;
    bool            m_bShowMinutes;
    bool            m_bShowSeconds;

    float           m_Seconds;
    
public:
                    JTimeLabel();

    float           GetSeconds() const { return m_Seconds; }
    void            SetSeconds( float val );

    expose( JTimeLabel )
    {
        parent(JLabel);
        field( "Backward",      m_bBackward             );
        field( "TimeLimit",     m_TimeLimit             );
        field( "ShowHours",     m_bShowHours            );
        field( "ShowMinutes",   m_bShowMinutes          );
        field( "ShowSeconds",   m_bShowSeconds          );
        prop ( "Seconds",       GetSeconds, SetSeconds  );
    }

private:
}; // class JTimeLabel

#endif // __JTIMELABEL_H__


