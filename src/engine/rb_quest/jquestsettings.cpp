/***********************************************************************************/
//  File:   JQuestSettings.cpp
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ISoundServer.h"
#include "JQuestGame.h"
#include "FStream.h"
#include "ISignalServer.h"
#include "JQuestSettings.h"

/***********************************************************************************/
/*  JQuestSettings implementation
/***********************************************************************************/
decl_class(JQuestSettings);
JQuestSettings::JQuestSettings()
{
    m_bPlayMusic    = true;
    m_bPlaySounds   = true;
    m_SoundVol      = 1.0f;
    m_MusicVol      = 1.0f;
    m_Tier          = 1;
} // JQuestSettings::JQuestSettings

void JQuestSettings::Init()
{
    Load();
} // JQuestSettings::Init

void JQuestSettings::SetText( const char* text )
{
    m_Text = text;
    SendSignal( "Text" );
}

void JQuestSettings::SetMusicVolume( float volume )
{
    g_pSoundServer->SetTagVolume( 1, volume );
    m_MusicVol = volume;
} // JQuestSettings::SetMusicVolume

float JQuestSettings::GetMusicVolume() const
{
    return m_MusicVol;
} // JQuestSettings::GetMusicVolume

void JQuestSettings::SetSoundVolume( float volume )
{
    g_pSoundServer->SetTagVolume( 0, volume );
    m_SoundVol = volume;
} // JQuestSettings::SetSoundVolume

float JQuestSettings::GetSoundVolume() const
{
    return m_SoundVol;
} // JQuestSettings::GetSoundVolume

void JQuestSettings::SetPlayMusic( bool bPlay )
{
    g_pSoundServer->MuteTag( 1, !bPlay );
    m_bPlayMusic = bPlay;
} // JQuestSettings::SetPlayMusic

bool JQuestSettings::GetPlayMusic() const
{
    return m_bPlayMusic;
} // JQuestSettings::GetPlayMusic

void JQuestSettings::SetPlaySounds( bool bPlay )
{
    g_pSoundServer->MuteTag( 0, !bPlay );
    m_bPlaySounds = bPlay;
} // JQuestSettings::SetPlaySounds

bool JQuestSettings::GetPlaySounds() const
{
    return m_bPlaySounds;
} // JQuestSettings::GetPlaySounds

void JQuestSettings::Load()
{
    char path[_MAX_PATH];
    if (!g_pQuestGame->GetGameUserFolder( path ))
    {
        return;
    }
    strcat( path, "\\options.ini" );

    FInStream is( path );
    if (!is) return;
    
    is >> m_SoundVol >> m_MusicVol >> m_bPlaySounds >> m_bPlayMusic >> m_Tier >> m_Text;

    SendSignal( "MusicVolume" );
    SendSignal( "SoundVolume" );
    SendSignal( "PlayMusic" );
    SendSignal( "PlaySounds" );
    SendSignal( "Tier" );
    SendSignal( "Text" );

    SetSoundVolume( m_SoundVol );
    SetMusicVolume( m_MusicVol );

    g_pSoundServer->MuteTag( 0, !m_bPlaySounds );
    g_pSoundServer->MuteTag( 1, !m_bPlayMusic );

    g_pSignalServer->Connect( this );

} // JQuestSettings::Load

void JQuestSettings::Save()
{
    char path[_MAX_PATH];
    if (!g_pQuestGame->GetGameUserFolder( path ))
    {
        return;
    }
    strcat( path, "\\options.ini" );

    FOutStream os( path );
    if (!os) return ;

    os << m_SoundVol << m_MusicVol << m_bPlaySounds << m_bPlayMusic << m_Tier << m_Text;

    g_pSignalServer->Connect( this );
} // JQuestSettings::Save
