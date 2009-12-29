/***********************************************************************************/
//  File:   JAnimServer.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANIMSERVER_H__
#define __JANIMSERVER_H__

#include "Timer.h"

class JAnimation;
/***********************************************************************************/
//  Class:  JAnimServer
//  Desc:   
/***********************************************************************************/
class JAnimServer : public JObject
{
    Timer               m_Timer;
    double              m_PrevTime;
    double              m_TimeDeltaMax;
    bool                m_bPaused;

public:
                        JAnimServer ();
    virtual void        Render      ();
    double              GetCurTime  () const { return m_Timer.Seconds(); }
    void                SetCurTime  ( double cTime ) { m_Timer.Set( cTime ); }
    
    void                Pause       ( bool bPause = true );
    bool                IsPaused    () const { return m_bPaused; }


    static JAnimServer* s_pInstance;

    expose(JAnimServer)
    {
        parent(JObject);
        prop( "paused", IsPaused, Pause );
    }
}; // class JAnimServer

#endif //__JANIMSERVER_H__