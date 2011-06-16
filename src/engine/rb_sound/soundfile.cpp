//****************************************************************************/
//  File:  SoundFile.cpp
//****************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "SoundServer.h"
#include "SoundFile.h"

//****************************************************************************/
/*  SoundFile implementation
//****************************************************************************/
decl_class( SoundFile )
SoundFile::SoundFile()
{
    m_bLoaded       = false;
    m_bStreamed     = false;
    m_Size          = 0;
    m_BufferSize    = 0;
    m_StreamPos     = 0;
    m_bPlayed       = false;
    m_TotalTime     = 0.0f;
    m_Tag           = -1;
    m_Volume        = 1.0f;
    m_bMuted        = false;
    m_bPaused       = false;
} // SoundFile::SoundFile

IDirectSoundBuffer* SoundFile::GetSoundBuffer()
{
    IDirectSound8* pDS = SoundServer::s_pInstance->GetDS();
    if (!pDS) return NULL;

    for (int i = 0; i < m_Buffers.size(); i++)
    {
        IDirectSoundBuffer* pBuffer = m_Buffers[i];
        if (pBuffer)
        {  
            DWORD status = 0;
            pBuffer->GetStatus( &status );
            if ((status&DSBSTATUS_PLAYING) == 0)
            {
                return pBuffer;
            }
        }
    }
    
    if (m_Buffers.size() == c_MaxSoundBuffers)
    {
        return m_Buffers[rand()%c_MaxSoundBuffers];
    }

    if (m_Buffers.size() == 0) return NULL;
    
    if (m_bStreamed)
    {
        return m_Buffers[0];
    }

    IDirectSoundBuffer* pBuffer = NULL;
    HRESULT hr = pDS->DuplicateSoundBuffer( m_Buffers[0], &pBuffer );
    m_Buffers.push_back( pBuffer );
    return pBuffer;
} // SoundFile::GetSoundBuffer

bool SoundFile::Load()
{
    IDirectSound8* pDS = SoundServer::s_pInstance->GetDS();
    if (!pDS) return false;
    if (m_Size == 0)
    {
        return false;
    }
    
    m_bStreamed = m_Size > c_MaxSoundBufferSize;
    m_BufferSize = m_Size;
    if (m_BufferSize > c_MaxSoundBufferSize) m_BufferSize = c_MaxSoundBufferSize;

    //  create first sound buffer
    DWORD flags = DSBCAPS_CTRLVOLUME;
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = flags;
    dsbd.dwBufferBytes   = m_BufferSize;
    dsbd.guid3DAlgorithm = GUID_NULL;
    dsbd.lpwfxFormat     = &m_Format;

    IDirectSoundBuffer* pBuffer = NULL;
    HRESULT hr = pDS->CreateSoundBuffer( &dsbd, &pBuffer, NULL );
            
    if( FAILED(hr) )
    {
        rlog.err( "Could not create sound buffer for file: %s", m_Name.c_str() );
        return false;
    }

    m_Buffers.push_back( pBuffer );
    int nBytes = m_BufferSize;
    if (m_bStreamed) nBytes = m_BufferSize/2;
    m_StreamPos = FillBuffer( pBuffer, 0, nBytes );
    m_bLoaded = true;
    return true;
} // SoundFile::Load

bool SoundFile::Unload()
{
    for (int i = 0; i < m_Buffers.size(); i++)
    {
        SAFE_RELEASE( m_Buffers[i] );
    }
    m_Buffers.clear();
    m_bLoaded = false;
    return true;
} // SoundFile::Unload

bool SoundFile::RestoreBuffer( IDirectSoundBuffer* pBuffer )
{
    if (!pBuffer) return false;
    DWORD status;
    pBuffer->GetStatus( &status );

    if (status&DSBSTATUS_BUFFERLOST)
    {
        // Since the app could have just been activated, then
        // DirectSound may not be giving us control yet, so 
        // the restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do 
        {
            if (pBuffer->Restore() == DSERR_BUFFERLOST)
            {
                Sleep( 10 );
            }
        }
        while (pBuffer->Restore() == DSERR_BUFFERLOST);
        return true;
    }
    return false;
} // SoundFile::RestoreBuffer

bool SoundFile::Play( bool bLooped )
{
    if (bLooped && m_bPlayed) 
    {
        return true;
    }

    m_StreamPos = -1;
    if (!m_bLoaded) 
    {
        Load();
    }
    else
    {
        Reset();
    }

    if (m_bStreamed)
    {
        UpdateStream();
    }

    IDirectSoundBuffer* pBuffer = GetSoundBuffer();
    if (!pBuffer) return false;

    //  restore the buffer if it was lost
    if (RestoreBuffer( pBuffer ))
    {
        FillBuffer( pBuffer );
    }

    pBuffer->SetCurrentPosition( 0 );
    DWORD flags = 0;
    if (m_bStreamed || bLooped) flags |= DSBPLAY_LOOPING;
    pBuffer->Play( 0, 0, flags );
    SetVolume( m_Volume );
    m_bPlayed = true;

    if (SoundServer::s_pInstance->IsVerbose())
    {
        SoundServer::s_pInstance->LogCurTime();
        rlog.msg( "SND: Started Playing %s, length is %f", m_Name.c_str(), m_TotalTime );
    }
    return true;
} // SoundFile::Play

bool SoundFile::Pause( bool bPause )
{
    if (bPause == m_bPaused) return false;
    IDirectSound8* pDS = SoundServer::s_pInstance->GetDS();
    if (!pDS) return false;

    if (bPause)
    {
        for (int i = 0; i < m_Buffers.size(); i++)
        {
            IDirectSoundBuffer* pBuffer = m_Buffers[i];
            if (pBuffer)
            {  
                DWORD status = 0;
                pBuffer->GetStatus( &status );
                if ((status&DSBSTATUS_PLAYING) != 0)
                {
                    pBuffer->Stop();
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < m_Buffers.size(); i++)
        {
            IDirectSoundBuffer* pBuffer = m_Buffers[i];
            if (pBuffer)
            {  
                DWORD flags = 0;
                if (m_bStreamed) flags |= DSBPLAY_LOOPING;
                pBuffer->Play( 0, 0, flags );
                SetVolume( m_Volume );
            }
        }
    }

    m_bPaused = bPause;
    return true;
} // SoundFile::Pause

bool SoundFile::Stop()
{
    IDirectSound8* pDS = SoundServer::s_pInstance->GetDS();
    if (!pDS) return false;
    for (int i = 0; i < m_Buffers.size(); i++)
    {
        IDirectSoundBuffer* pBuffer = m_Buffers[i];
        if (pBuffer)
        {  
            DWORD status = 0;
            pBuffer->GetStatus( &status );
            if ((status&DSBSTATUS_PLAYING) != 0)
            {
                pBuffer->Stop();
            }
        }
    }
    m_bPlayed = false;
    m_bPaused = false;
    m_StreamPos = -1;
    return true;
} // SoundFile::Stop

void SoundFile::ZeroBuffer( IDirectSoundBuffer* pBuffer, int startByte, DWORD nBytes )
{
    if (pBuffer == NULL) return;
    //  lock the buffer  
    void* pData  = NULL;
    DWORD bufSize = 0;
    if (nBytes == 0) nBytes = m_BufferSize;
    HRESULT hr = pBuffer->Lock( startByte, nBytes, &pData, &nBytes, NULL, NULL, 0L );
    if (hr != S_OK)
    {
        rlog.err( "Could not lock sound buffer for sound %s", m_Name.c_str() );
        return;
    }
    
    BYTE* pDataB = (BYTE*)pData;
    memset( pDataB, 0, nBytes );
    
    hr = pBuffer->Unlock( pData, m_BufferSize, NULL, 0 );
    if (hr != S_OK)
    {
        rlog.err( "Could not unlock sound buffer for sound %s", m_Name.c_str() );
    }
} // SoundFile::ZeroBuffer

int SoundFile::FillBuffer( IDirectSoundBuffer* pBuffer, int startByte, DWORD nBytes )
{
    if (pBuffer == NULL) return 0;

    //  make sure we have focus, and we didn't just switch in from
    //  an app which had a DirectSound device
    RestoreBuffer( pBuffer );

    //  lock the buffer  
    void* pData  = NULL;
    DWORD bufSize = 0;
    if (nBytes == 0) nBytes = m_BufferSize;
    HRESULT hr = pBuffer->Lock( startByte, nBytes, &pData, &nBytes, NULL, NULL, 0L );
    if (hr != S_OK)
    {
        rlog.err( "Could not lock sound buffer for sound %s", m_Name.c_str() );
        return false;
    }

    if (!m_bStreamed)
    {
        Reset();
    }

    int nRead = ReadData( (BYTE*)pData, nBytes );           
    hr = pBuffer->Unlock( pData, m_BufferSize, NULL, 0 );
    if (hr != S_OK)
    {
        rlog.err( "Could not unlock sound buffer for sound %s", m_Name.c_str() );
        return -1;
    }

    return nRead;
} // SoundFile::FillBuffer

void SoundFile::SetVolume( float volume )
{
    if (!m_bMuted)
    {
        float dVol = float( DSBVOLUME_MAX - DSBVOLUME_MIN );
        float lVol = DSBVOLUME_MIN + volume*dVol;
        for (int i = 0; i < m_Buffers.size(); i++)
        {
            HRESULT hres = m_Buffers[i]->SetVolume( lVol );
            if (hres != S_OK)
            {
                rlog.err( "Could not adjust volume of the sound: %s", m_Name.c_str() );
            }
        }
    }
    m_Volume = volume;
} // SoundFile::SetVolume

void SoundFile::Mute( bool bMute )
{
    m_bMuted = bMute;
    if (bMute) 
    {
        for (int i = 0; i < m_Buffers.size(); i++)
        {
            HRESULT hres = m_Buffers[i]->SetVolume( DSBVOLUME_MIN );
            if (hres != S_OK)
            {
                rlog.err( "Could not adjust volume of the sound: %s", m_Name.c_str() );
            }
        }
    }
    else
    {
        SetVolume( m_Volume );
    }
} // SoundFile::Mute

float SoundFile::GetVolume() const
{
    float vol = 0.0f;
    int nBuffers = m_Buffers.size();
    if (nBuffers == 0) return m_Volume;
    for (int i = 0; i < m_Buffers.size(); i++)
    {
        LONG lVol = 0;
        HRESULT hRes = m_Buffers[i]->GetVolume( &lVol );
        if (hRes == S_OK)
        {
            vol += float( DSBVOLUME_MAX - lVol )/float( DSBVOLUME_MAX - DSBVOLUME_MIN );
        }
        else
        {
            nBuffers--;
        }
    }
    return vol/float( nBuffers );
} // SoundFile::GetVolume

void SoundFile::UpdateStream()
{
    if (m_Buffers.size() == 0) return;
    IDirectSoundBuffer* pBuffer = m_Buffers[0];
    DWORD playCursor  = 0;
    DWORD writeCursor = 0;
    int   nToPlay     = 0;

    if (m_StreamPos == -1)
    {
        m_StreamPos = 0;
    }
    else
    {
        HRESULT hr = pBuffer->GetCurrentPosition( &playCursor, &writeCursor );

        //  number of bytes, which are filled properly
        nToPlay = m_StreamPos - playCursor;
        if (playCursor >= m_StreamPos) 
        {
            nToPlay = m_BufferSize - (playCursor - m_StreamPos);
        }
    }

    if (nToPlay < m_BufferSize/4)
    {
        //  time to stream in next chunk of data
        int nToRead = tmin( m_BufferSize/2, m_BufferSize - m_StreamPos );
        if (nToRead == 0)
        {
            m_StreamPos = 0;
            nToRead = m_BufferSize/2;
        }
        int nRead = FillBuffer( pBuffer, m_StreamPos, nToRead );
        m_StreamPos += nRead;

        if (nRead == 0)
        {
            //  end of file, add silence
            int nZero = m_BufferSize - m_StreamPos;
            if (playCursor > m_StreamPos)
            {
                nZero = playCursor - m_StreamPos;
            }
            ZeroBuffer( pBuffer, m_StreamPos, nZero );
        }

        if (SoundServer::s_pInstance->IsVerbose())
        {
            SoundServer::s_pInstance->LogCurTime();
            rlog.msg( "SND: Updated stream with chunk of %d bytes (from %d)", nRead, nToRead );
        }
    }
} // SoundFile::UpdateStream

void SoundFile::Update()
{
    if (m_bPlayed)
    {
        if (m_bStreamed)
        {
            UpdateStream();
        }
        else
        {
            for (int i = 0; i < m_Buffers.size(); i++)
            {
                IDirectSoundBuffer* pBuffer = m_Buffers[i];
                if (pBuffer)
                {  
                    DWORD status = 0;
                    pBuffer->GetStatus( &status );
                    if ((status&DSBSTATUS_PLAYING) != 0)
                    {
                        return;
                    }
                }
            }
            if (!m_bPaused)
            {
                Stop();
            }
        }
    }
} // SoundFile::Update



