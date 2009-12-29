/***********************************************************************************/
//  File:   JAnimServer.cpp
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JAnimServer.h"

/***********************************************************************************/
/*  JAnimServer implementation
/***********************************************************************************/
JAnimServer* JAnimServer::s_pInstance = NULL;
decl_class( JAnimServer );
JAnimServer::JAnimServer()
{
    SetName( "anim" );
    
    m_Timer.Start();

    m_bPaused       = false;
    m_PrevTime      = 0.0;
    m_TimeDeltaMax  = 0.5;
    s_pInstance     = this;
} //JAnimServer::JAnimServer

void JAnimServer::Render()
{
    double dTime = m_Timer.Seconds() - m_PrevTime;
    m_PrevTime = m_Timer.Seconds();
    dTime = clamp( dTime, 0.0, m_TimeDeltaMax );
    JAnmContext& ctx = JAnimation::GetContext();
    ctx.m_TimeDelta = dTime;
    ctx.m_TM        = Mat4::identity;
} // JAnimServer::Render

void JAnimServer::Pause( bool bPause )
{
    if (bPause)
    {
        m_Timer.Pause();
    }
    else
    {
        m_Timer.Resume();
    }

    m_bPaused = bPause;
} // JAnimServer::Pause
