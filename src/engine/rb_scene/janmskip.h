/***********************************************************************************/
//  File:   JAnmSkip.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANMSKIP_H__
#define __JANMSKIP_H__

class JAnimation;
/***********************************************************************************/
//  Class:  JAnmSkip
//  Desc:   
/***********************************************************************************/
class JAnmSkip : public JWidget
{
    JString         m_Animation;
    JAnimation*     m_pAnimation;
    float           m_TimeDelta;

    bool            m_bRewinding;

public:
                    JAnmSkip    ();
    void            Skip        ();
    void            SkipAll     ();
    virtual void    Init        ();
    virtual void    OnKey       ( JKeyEvent& e );

    void            StopRewind  () { m_bRewinding = false; }

    expose(JAnmSkip)
    {
        parent(JWidget);
        field( "Animation",     m_Animation );
        field( "TimeDelta",     m_TimeDelta );
        method( "Skip",         Skip );
        method( "SkipAll",      SkipAll );
    }
}; // class JAnmSkip

#endif //__JANMSKIP_H__