/***********************************************************************************/
//  File:   JAnimation.cpp
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "jobject.h"
#include "janimserver.h"
#include "janimation.h"

/***********************************************************************************/
/*  JAnimation implementation
/***********************************************************************************/
decl_class(JAnimation);
JContextStack  JAnimation::s_ContextStack = JContextStack( 1 );
JAnimation::JAnimation()
{
    m_bReentrant    = true;
    m_bLooped       = false;
    m_bParallel     = false;
    m_bAutoPlay     = false;
    m_bPlaying      = false;
    m_bPaused       = false;
    m_PlayMode      = AnimMode_PlayAll;
    m_WrapMode      = AnimWrap_Loop;

    m_Duration      = 0.0f;
    m_MinDuration   = 0.0f;
    m_MaxDuration   = 0.0f;
    m_StartTime     = 0.0f;
    m_NLoops        = 0;

    m_CurChild      = 0;
    m_CurLoop       = 0;
    m_CurTime       = 0.0f;
    m_Tempo         = 1.0f;

    SetVisible( false );
}

float JAnimation::Wrap( float cTime, float duration )
{
    int nCycles = int( cTime/duration );
    switch (m_WrapMode)
    {
    case AnimWrap_Loop:
        return fmodf( cTime, duration );
    case AnimWrap_Pong:
        return (nCycles%2 == 0) ? fmodf( cTime, duration ) :
                                  duration - fmodf( cTime, duration );
    case AnimWrap_Freeze:
        return tmin( cTime, duration );
    case AnimWrap_Reset:
        return (cTime > duration) ? 0.0f : cTime;
    }
    return cTime;
}

void JAnimation::Play()
{
    if (m_bPlaying && !m_bReentrant) return;

    if (m_MaxDuration != 0.0f || m_MinDuration != 0.0f)
    {
        SetDuration( frand( m_MinDuration, m_MaxDuration ) );
    }

    SetVisible( true );

    m_CurTime = 0.0f;

    if (m_CurLoop == 0)
    {
        m_CurTime += double( m_StartTime );
    }

    int nCh = GetNChildren();
    if (m_PlayMode == AnimMode_PlayAll)
    {
        m_CurChild = 0;
    }
    else if (m_PlayMode == AnimMode_PlayRandom)
    {
        m_CurChild = nCh > 0 ? rand()%nCh : 0;
    }
    else if (m_PlayMode == AnimMode_PlayOne)
    {
         if (m_CurChild >= nCh)
         {
            m_CurChild = 0;
         }
    }
    m_CurLoop  = 0;
    //  reset child animations to idle state
    for (int i = 0; i < GetNChildren(); i++)
    {
        JAnimation* pChild = obj_cast<JAnimation>( GetChild( i ) );
        if (pChild && pChild->IsPlaying())
        {
            pChild->Stop();
        }
    }

    m_bPlaying = true;

    OnPlay();
    StartCurChild();

    if (GetNChildren() == 0 && GetDuration() <= 0.0f)
    {
        Stop();
    }
}

void JAnimation::Pause( bool bPause )
{
    m_bPaused = bPause;
}

void JAnimation::Stop()
{
    if (!m_bPlaying)
    {
        return;
    }
    if (!m_bAutoPlay)
    {
        SetVisible( false );
    }
    m_bPlaying = false;

    //  stop children as well
    for (int i = 0; i < GetNChildren(); i++)
    {
        JAnimation* pChild = obj_cast<JAnimation>( GetChild( i ) );
        if (pChild)
        {
            pChild->Stop();
        }
    }

    OnStop();

    JAnimation* pParent = obj_cast<JAnimation>( GetParent() );
    if (pParent && !m_bPlaying)
    {
        pParent->OnChildStopped( this );
    }
    m_CurLoop++;

}

void JAnimation::StartCurChild()
{
    if (!m_bPlaying) return;
    while (m_CurChild < GetNChildren())
    {
        JAnimation* pNewChild = obj_cast<JAnimation>( GetChild( m_CurChild ) );
        m_CurChild++;
        if (!pNewChild)
        {
            continue;
        }
        pNewChild->Play();
        if (!pNewChild->IsParallel())
        {
            return;
        }
    }
}

void JAnimation::OnChildStopped( JAnimation* pChild )
{
    if (!pChild->IsParallel())
    {
        StartCurChild();
    }

    int nCh = GetNChildren();
    bool bAllDone = true;
    for (int i = 0; i < nCh; i++)
    {
        JAnimation* pCurChild = obj_cast<JAnimation>( GetChild( i ) );
        if (!pCurChild || pCurChild == pChild) continue;
        if (pCurChild->m_bPlaying && !pCurChild->IsParallel())
        {
            bAllDone = false;
        }
    }

    if (m_PlayMode == AnimMode_PlayAll && m_CurChild < nCh)
    {
        bAllDone = false;
    }

    if (nCh > 0 && m_Duration == 0.0f && bAllDone && IsPlaying())
    {
        if (m_bLooped)
        {
            m_bPlaying = false;
            Play();
        }
        else
        {
            Stop();
        }
    }
}

float JAnimation::GetCurTime() const
{
    if (!IsPlaying()) return m_StartTime;
    return m_CurTime;
}

void JAnimation::SkipChild()
{
    m_CurChild++;
    if (m_CurChild >= GetNChildren())
    {
        if (m_bLooped)
        {
            m_CurChild = 0;
        }
        else
        {
            Stop();
        }
    }
}

void JAnimation::Render()
{
    JAnmContext& ctx = PushContext();
    //  filter by state
    if (m_State.size() > 0)
    {
        if (stricmp( m_State.c_str(), ctx.m_State.c_str() ))
        {
            Stop();
            return;
        }
    }

    if (m_bAutoPlay && !m_bPlaying)
    {
        Play();
    }
    if (m_bPaused)
    {
        GetContext().m_TimeDelta = 0.0f;
    }

    m_CurTime += m_Tempo*GetContext().m_TimeDelta;
    GetContext().m_Time = m_Tempo*m_CurTime;
    GetContext().m_TimeDelta *= m_Tempo;
}

void JAnimation::PostRender()
{
    //  if animation is ended, stop it (or loop further)
    double cTime = GetContext().m_Time;
    if (m_Duration > 0.0f)
    {
        if (cTime >= m_Duration)
        {
            if (m_bLooped)
            {
                m_CurTime = fmodf( cTime, m_Duration );
                if (m_NLoops > 0 && m_CurLoop == m_NLoops)
                {
                    Stop();
                }
                else
                {
                    Play();
                }
            }
            else
            {
                Stop();
            }
        }
    }
    PopContext();
}

JAnmContext& JAnimation::PopContext()
{
    return s_ContextStack.pop();
}

JAnmContext& JAnimation::PushContext()
{
    if (s_ContextStack.size() >= 1)
    {
        const JAnmContext& top = s_ContextStack.back();
        JAnmContext& newTop = s_ContextStack.expand();
        newTop = top;
        return newTop;
    }
    else
    {
        JAnmContext& newTop = s_ContextStack.expand();
        return newTop;
    }
}
