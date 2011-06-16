/*****************************************************************************/
/*  File:  ISoundServer.h
/*****************************************************************************/
#ifndef __ISOUNDSERVER_H__
#define __ISOUNDSERVER_H__

/*****************************************************************************/
/*  Class:  ISoundServer
/*  Desc:   
/*****************************************************************************/
class ISoundServer 
{
public:
  
    virtual int     GetSoundID      ( const char* fname, int tag = -1 ) = 0;
    virtual bool    DeleteSound     ( int sndID ) = 0;
    
    virtual bool    Play            ( int sndID, bool bLooped = false ) = 0;
    virtual bool    IsPlaying       ( int sndID ) const = 0;
    virtual bool    Stop            ( int sndID ) = 0;
    virtual bool    Pause           ( int sndID, bool bPause = true ) = 0;
    virtual bool    Pause           ( bool bPause = true ) = 0;

    virtual void    SetTagVolume    ( int tag, float vol ) = 0;
    virtual float   GetTagVolume    ( int tag ) const = 0;
    virtual void    MuteTag         ( int tag, bool bMute = true ) = 0;
    virtual bool    IsMutedTag      ( int tag ) = 0;

    virtual void    SetVolume       ( int sndID, float vol ) = 0;
    virtual float   GetVolume       ( int sndID ) = 0;

    virtual float   GetTotalTime    ( int sndID ) = 0;
    virtual void    Update          () = 0;

}; // class ISoundServer

extern ISoundServer* g_pSoundServer;

#endif // __ISOUNDSERVER_H__


