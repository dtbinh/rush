/***********************************************************************************/
//  File:   JAnmSound.cpp
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JAnimation.h"
#include "ISoundServer.h"
#include "JAnmSound.h"

/***********************************************************************************/
/*  JAnmSound implementation
/***********************************************************************************/
decl_class(JAnmSound);
JAnmSound::JAnmSound()
{
    m_SoundID   = -1;   
    SetTag( 0 );
} // JAnmSound::JAnmSound

void JAnmSound::Render()
{
    if (IsPlaying() && g_pSoundServer)
    {
        JAnmContext& ctx = GetContext();
        if (ctx.m_bStateOnly) 
        {
            Stop();
        }
        else if (GetCurTime() >= GetDuration() && g_pSoundServer->IsPlaying( m_SoundID ) && !IsLooped())
        {
            SetDuration( GetCurTime() + c_FltEpsilon );
        }

        g_pSoundServer->Pause( m_SoundID, GetContext().m_TimeDelta == 0.0f || IsPaused() );
    }
    JAnimation::Render();
} // JAnmSound::Render

void JAnmSound::OnStop()
{
    if (!g_pSoundServer)
    {
        return;
    }
    g_pSoundServer->Stop( m_SoundID );
} // JAnmSound::OnStop

void JAnmSound::ResInit()
{
    if (!g_pSoundServer)
    {
        return;
    }
    m_SoundID = g_pSoundServer->GetSoundID( m_File.c_str(), GetTag() );
    if (GetDuration() == 0.0f)
    {
        SetDuration( g_pSoundServer->GetTotalTime( m_SoundID ) );
    }
} // JAnmSound::OnStop

void JAnmSound::OnPlay()
{
    if (!g_pSoundServer)
    {
        return;
    }
    if (m_SoundID == -1) 
    {
        ResInit();
    }
    g_pSoundServer->Play( m_SoundID, IsLooped() );
} // JAnmSound::OnPlay
