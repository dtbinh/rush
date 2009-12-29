//****************************************************************************/
//  File:  SoundServer.h
//****************************************************************************/
#ifndef __SOUNDSERVER_H__
#define __SOUNDSERVER_H__

struct VolumeTag
{
    int     m_Tag;
    float   m_Volume;
    bool    m_bMuted;

    VolumeTag() : m_Tag( -1 ), m_Volume( 0.0f ), m_bMuted(false) {}
    VolumeTag( int tag, float volume ) : m_Tag( tag ), m_Volume( volume ), m_bMuted(false) {}
}; // struct VolumeTag

class SoundFile;
//****************************************************************************/
//  Class:  SoundServer
//  Desc:   
//****************************************************************************/
class SoundServer : public JObject, public ISoundServer
{
    IDirectSound8*                  m_pDS;
    std::vector<SoundFile*>         m_Sounds;
    mutable std::vector<VolumeTag>  m_VolumeTags;

    Timer                           m_Timer;    // internal timer to keep track of playing time
    bool                            m_bVerbose;

public:
                    SoundServer     ();
                    ~SoundServer    ();
    virtual void    Init            ();
    virtual void    Render          ();
    virtual int     GetSoundID      ( const char* fname, int tag = -1 );
    virtual bool    DeleteSound     ( int sndID );
    
    virtual bool    Play            ( int sndID, bool bLooped = false );
    virtual bool    IsPlaying       ( int sndID ) const;
    virtual bool    Stop            ( int sndID );
    virtual bool    Pause           ( int sndID, bool bPause = true );
    virtual bool    Pause           ( bool bPause = true );

    virtual void    SetVolume       ( int sndID, float vol );

    virtual float   GetTagVolume    ( int tag ) const;
    virtual void    SetTagVolume    ( int tag, float vol );

    virtual float   GetVolume       ( int sndID );

    virtual void    MuteTag         ( int tag, bool bMute = true );
    virtual bool    IsMutedTag      ( int tag );

    virtual float   GetTotalTime    ( int sndID );
    virtual void    Update          ();

    bool            IsVerbose       () const { return m_bVerbose; }
    void            LogCurTime      ();

    IDirectSound8*  GetDS           () const { return m_pDS; }


    expose(SoundServer)
    {
        parent(JObject);
        field( "Verbose", m_bVerbose );
        prop( "Volume", GetTagVolume, SetTagVolume );
    }

    static SoundServer* s_pInstance;

protected:
    VolumeTag*      GetTag          ( int tag ) const;

}; // class SoundServer

#endif // __SOUNDSERVER_H__


