/***********************************************************************************/
//  File:   JAnimation.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANIMATION_H__
#define __JANIMATION_H__

#include "janmcontext.h"
#include "staticarray.h"

enum JAnimMode
{
    AnimMode_PlayAll     = 0,
    AnimMode_PlayRandom  = 1,
    AnimMode_PlayOne     = 2
}; // enum JAnimMode

enum JAnimWrap
{
    AnimWrap_None        = 0,
    AnimWrap_Loop        = 1,   //  animation is looped
    AnimWrap_Pong        = 2,   //  animation is played in pong style (forwards-back-forwards-back...)
    AnimWrap_Freeze      = 3,   //  animation is played once and then is frozen at last frame
    AnimWrap_Reset       = 4    //  animation is played once and then is frozen at first frame
}; // enum JAnimWrap

enum_beg(JAnimMode)
    enum_val( "PlayAll",      AnimMode_PlayAll      ),
    enum_val( "PlayRandom",   AnimMode_PlayRandom   ),
    enum_val( "PlayOne",      AnimMode_PlayOne      )
enum_end(JAnimMode)

enum_beg(JAnimWrap)
    enum_val( "None",   AnimWrap_None   ),
    enum_val( "Loop",   AnimWrap_Loop   ),
    enum_val( "Pong",   AnimWrap_Pong   ),
    enum_val( "Freeze", AnimWrap_Freeze ),
    enum_val( "Reset",  AnimWrap_Reset  )
enum_end(JAnimWrap)

const int c_AnmStackSize = 128;
typedef static_array<JAnmContext, c_AnmStackSize> JContextStack;
/***********************************************************************************/
//  Class:  JAnimation
//  Desc:
/***********************************************************************************/
class JAnimation : public JObject
{
    bool            m_bReentrant;
    bool            m_bLooped;
    bool            m_bAutoPlay;
    bool            m_bParallel;
    bool            m_bPlaying;
    bool            m_bPaused;
    float           m_Tempo;
    JAnimMode       m_PlayMode;
    JAnimWrap       m_WrapMode;

    float           m_Duration;
    float           m_MinDuration;
    float           m_MaxDuration;
    float           m_StartTime;
    float           m_CurTime;
    int             m_NLoops;

    int             m_CurChild;
    int             m_CurLoop;

    JString         m_State;

public:
                    JAnimation      ();
    void            Play            ();
    void            Stop            ();
    void            SkipChild       ();

    virtual void    Render          ();
    virtual void    PostRender      ();

    bool            IsLooped        () const { return m_bLooped; }
    bool            IsParallel      () const { return m_bParallel; }
    bool            IsAutoPlayed    () const { return m_bAutoPlay; }
    bool            IsPaused        () const { return m_bPaused; }

    bool            IsPlaying       () const { return m_bPlaying; }
    float           GetDuration     () const { return m_Duration; }
    void            SetDuration     ( float amt ) { m_Duration = amt; }
    float           GetCurTime      () const;
    void            SetCurTime      ( float cTime ) { m_CurTime = cTime; }
    int             GetCurLoop      () const { return m_CurLoop; }
    float           GetStartTime    () const { return m_StartTime; }
    float           GetTempo        () const { return m_Tempo; }
    JAnimWrap       GetWrapMode     () const { return m_WrapMode; }

    void            SetParallel     ( bool bVal ) { m_bParallel = bVal; }

    virtual void    OnPlay          () {}
    virtual void    OnStop          () {}

    float           Wrap            ( float cTime, float duration );

    void            Pause           ( bool bPause );

    expose(JAnimation)
    {
        parent(JObject);

        field ( "Reentrant",    m_bReentrant    );
        field ( "Looped",       m_bLooped       );
        field ( "Parallel",     m_bParallel     );
        field ( "AutoPlay",     m_bAutoPlay     );
        field ( "Duration",     m_Duration      );
        field ( "MinDuration",  m_MinDuration   );
        field ( "MaxDuration",  m_MaxDuration   );
        field ( "StartTime",    m_StartTime     );
        field ( "NLoops",       m_NLoops        );
        field ( "PlayMode",     m_PlayMode      );
        field ( "Playing",      m_bPlaying      );
        prop  ( "CurTime",      GetCurTime, SetCurTime );
        field ( "CurChild:d",   m_CurChild      );
        field ( "CurLoop:d",    m_CurLoop       );
        method( "Play",         Play            );
        method( "Stop",         Stop            );
        method( "SkipChild",    SkipChild       );
        field ( "Wrap",         m_WrapMode      );
        prop  ( "Paused",       IsPaused, Pause );
        field ( "Tempo",        m_Tempo         );
        field ( "State",        m_State         );

    }

    static JAnmContext&         GetContext      ()    { return s_ContextStack.back(); }
    static JAnmContext&         PopContext      ();
    static JAnmContext&         PushContext     ();

private:
    static JContextStack        s_ContextStack;

    void                        OnChildStopped  ( JAnimation* pChild );
    void                        StartCurChild   ();
}; // class JAnimation

#endif //__JANIMATION_H__
