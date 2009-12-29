//****************************************************************************/
//  File:   SoundServer.cpp
//  Date:   25.05.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "SoundFile.h"
#include "OggFile.h"
#include "WavFile.h"
#include "SoundServer.h"
#include "IWindowServer.h"
#include "IFileServer.h"

//****************************************************************************/
/*  SoundServer implementation
//****************************************************************************/
SoundServer* SoundServer::s_pInstance = NULL;
decl_class( SoundServer );
SoundServer::SoundServer()
{
    m_pDS           = NULL;
    g_pSoundServer  = this;
    s_pInstance     = this;
    m_bVerbose      = false;
    SetName( "sound" );
} // SoundServer::SoundServer

SoundServer::~SoundServer()
{
    CoUninitialize();
    SAFE_RELEASE( m_pDS ); 
} // SoundServer::~SoundServer

void SoundServer::Init()
{
    HWND hWnd = (HWND)g_pWindowServer->GetRootHandle();
    
    HRESULT hr;
    SAFE_RELEASE( m_pDS );
    //  create IDirectSound using the primary sound device
    if( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS, NULL ) ) )
    {
        rlog.err( "Could not create sound device." );
        return;
    }

    //  set DirectSound coop level 
    if( FAILED( hr = m_pDS->SetCooperativeLevel( hWnd, 0x01 ) ) )
    {
        rlog.err( "Could set cooperative level for device." );
        return;
    }
    m_Timer.Start();
} // SoundServer::Init

void SoundServer::Render()
{
    //  correct playing sounds to accomodate for a possible gap in update time
    double cTimeDelta = m_Timer.Seconds();
    //  TODO...

}  

void SoundServer::LogCurTime()
{
    double cSeconds = m_Timer.Seconds();
    rlog.msg( "SND: Time is %f sec.", cSeconds ); 
}  

int SoundServer::GetSoundID( const char* fname, int tag )
{
    for (int i = 0; i < m_Sounds.size(); i++)
    {
        if (!stricmp( m_Sounds[i]->GetName(), fname )) return i;
    }
    
    char drive  [_MAX_PATH];
    char dir    [_MAX_PATH];
    char file   [_MAX_PATH];
    char ext    [_MAX_PATH];
    _splitpath( fname, drive, dir, file, ext );
    
    JString mediaPath;
    if (!g_pFileServer->FindMedia( file, ext, &mediaPath )) 
    {
        return -1;
    }

    SoundFile* pSound = NULL;
    if (!stricmp( ext, ".wav" ))
    {
        pSound = new WavFile();
    }
    else if (!strnicmp( ext, ".ogg", 3 ))
    {
        pSound = new OggFile();
    }

    if (!pSound) return -1;
    
    pSound->SetName( fname );
    pSound->SetPath( mediaPath );
    pSound->SetTag ( tag );
    pSound->LoadHeader();
    m_Sounds.push_back( pSound );

    AddChild( pSound );
    pSound->AddRef();

    return m_Sounds.size() - 1;
} // SoundServer::GetSoundID

bool SoundServer::DeleteSound( int sndID )
{
    return false;
} // SoundServer::DeleteSound

bool SoundServer::Play( int sndID, bool bLooped )
{
    if (sndID < 0 || sndID >= m_Sounds.size()) return false;
    SoundFile* pSound = m_Sounds[sndID];
    int tag = pSound->GetTag();
    pSound->Play( bLooped );
    VolumeTag* pTag = GetTag( tag );
    if (pTag)
    {
        pSound->SetVolume( pTag->m_Volume );
        if (pTag->m_bMuted) 
        {
            pSound->Mute();
        }
    }
    
    return true;
} // SoundServer::Play

bool SoundServer::Stop( int sndID )
{
    if (sndID < 0 || sndID >= m_Sounds.size()) return false;
    m_Sounds[sndID]->Stop();
    return true;
} // SoundServer::Stop

void SoundServer::SetTagVolume( int tag, float vol )
{
    int nSounds = m_Sounds.size();
    for (int i = 0; i < nSounds; i++)
    {
        SoundFile* pSnd = m_Sounds[i];
        if (!pSnd || 
            ((tag != -1) && (pSnd->GetTag() != tag)))
        {
            continue;
        }
        pSnd->SetVolume( vol );
    }

    if (tag != -1)
    {
        VolumeTag* pTag = GetTag( tag );
        if (!pTag)
        {
            m_VolumeTags.push_back( VolumeTag( tag, vol ) );
            pTag = &m_VolumeTags.back();
        }
        pTag->m_Volume = vol;
    }
} // SoundServer::SetTagVolume

void SoundServer::SetVolume( int sndID, float vol )
{
    if (sndID < 0 || sndID >= m_Sounds.size()) return;
    return m_Sounds[sndID]->SetVolume( vol );
} // SoundServer::SetVolume

float SoundServer::GetTagVolume( int tag ) const
{
    VolumeTag* pTag = GetTag( tag );
    if (!pTag) return 1.0f;
    return pTag->m_Volume;
} // SoundServer::GetTagVolume

float SoundServer::GetVolume( int sndID )
{
    if (sndID < 0 || sndID >= m_Sounds.size()) return 1.0f;
    return m_Sounds[sndID]->GetVolume();
} // SoundServer::GetVolume

float SoundServer::GetTotalTime( int sndID )
{
    if (sndID < 0 || sndID >= m_Sounds.size()) return 1.0f;
    return m_Sounds[sndID]->GetTotalTime();
} // SoundServer::GetTotalTime

void SoundServer::Update()
{
    for (int i = 0; i < m_Sounds.size(); i++)
    {
        m_Sounds[i]->Update();
    }
} // SoundServer::Update

bool SoundServer::Pause( int sndID, bool bPause )
{
    if (sndID < 0 || sndID >= m_Sounds.size()) return false;
    return m_Sounds[sndID]->Pause( bPause );
} // SoundServer::Pause

bool SoundServer::IsPlaying( int sndID ) const
{
    if (sndID < 0 || sndID >= m_Sounds.size()) return false;
    return m_Sounds[sndID]->IsPlaying();
} // SoundServer::IsPlaying

bool SoundServer::Pause( bool bPause )
{
    for (int i = 0; i < m_Sounds.size(); i++)
    {
        if (m_Sounds[i]->IsPaused() != bPause && 
            m_Sounds[i]->IsPlaying())
        {
            m_Sounds[i]->Pause( bPause );
        }
    }
    return true;
} // SoundServer::Pause

VolumeTag* SoundServer::GetTag( int tag ) const
{
    for (int i = 0; i < m_VolumeTags.size(); i++)
    {
        if (m_VolumeTags[i].m_Tag == tag) 
        {
            return &m_VolumeTags[i];
        }
    }
    return NULL;
} // SoundServer::GetTag

void SoundServer::MuteTag( int tag, bool bMute )
{
    int nSounds = m_Sounds.size();
    for (int i = 0; i < nSounds; i++)
    {
        SoundFile* pSnd = m_Sounds[i];
        if (!pSnd || (pSnd->GetTag() != tag))
        {
            continue;
        }
        pSnd->Mute( bMute );
    }

    VolumeTag* pTag = GetTag( tag );
    if (!pTag)
    {
        m_VolumeTags.push_back( VolumeTag( tag, 1.0f ) );
        pTag = &m_VolumeTags.back();
    }
    pTag->m_bMuted = bMute;
} // SoundServer::MuteTag

bool SoundServer::IsMutedTag( int tag )
{
    VolumeTag* pTag = GetTag( tag );
    if (!pTag) return false;
    return pTag->m_bMuted;
} // SoundServer::IsMutedTag
