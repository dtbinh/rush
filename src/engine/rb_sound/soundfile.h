//****************************************************************************/
//  File:  SoundFile.h
//****************************************************************************/
#ifndef __SOUNDFILE_H__
#define __SOUNDFILE_H__

#include <mmsystem.h>

//  sound files bigger than this amount of bytes are streamed
const int c_MaxSoundBufferSize = 1024*1024*4;
const int c_MaxSoundBuffers    = 8;
//****************************************************************************/
//  Class:  SoundFile
//  Desc:   
//****************************************************************************/
class SoundFile : public JObject
{
protected:
    std::string                         m_Name;
    std::string                         m_Path;

    bool                                m_bLoaded;
    WAVEFORMATEX                        m_Format;
    bool                                m_bStreamed;
    std::vector<IDirectSoundBuffer*>    m_Buffers;
    int                                 m_Size;
    int                                 m_BufferSize;
    int                                 m_LastChunk;
    int                                 m_StreamPos;
    bool                                m_bPlayed;
    bool                                m_bPaused;
    float                               m_TotalTime;
    int                                 m_Tag;
    float                               m_Volume;
    bool                                m_bMuted;

public:
                        SoundFile       ();
    const char*         GetName         () const { return m_Name.c_str(); }
    void                SetName         ( const char* name ) { m_Name = name; }
    void                SetVolume       ( float volume );
    float               GetVolume       () const;
    void                Mute            ( bool bMute = true );
    bool                IsPlaying       () const { return m_bPlayed; }
    bool                IsPaused        () const { return m_bPaused; }

    const char*         GetPath         () const { return m_Path.c_str(); }
    void                SetPath         ( const char* path ) { m_Path = path; }

    int                 GetTag          () const { return m_Tag; }
    void                SetTag          ( int tag ) { m_Tag = tag; }

    float               GetTotalTime    () const { return m_TotalTime; }

    virtual bool        Play            ( bool bLooped = false );
    virtual bool        Stop            ();
    virtual bool        Pause           ( bool bPause );
    virtual bool        Load            ();
    virtual bool        Unload          ();
    virtual bool        LoadHeader      () { return false; }
    void                Update          ();
    void                UpdateStream    ();

    expose( SoundFile )
    {
        parent( JObject );
    }


protected:
    bool                RestoreBuffer   ( IDirectSoundBuffer* pBuffer );
    virtual int         FillBuffer      ( IDirectSoundBuffer* pBuffer, int startByte = 0, DWORD nBytes = 0 );
    void                ZeroBuffer      ( IDirectSoundBuffer* pBuffer, int startByte, DWORD nBytes );
    virtual int         ReadData        ( BYTE* pData, int nBytes ) { return false; }
    virtual bool        Reset           () { return false; }
    IDirectSoundBuffer* GetSoundBuffer  ();
}; // class SoundFile


#endif // __SOUNDFILE_H__


