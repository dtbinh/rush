/*****************************************************************************/
/*	File:   IParticleServer.h
/*  Date:   12.07.2006
/*  Author: Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __IPARTICLESERVER_H__
#define __IPARTICLESERVER_H__

class Mat3;
class Vec3;

/*****************************************************************************/
/*	Class:  IParticleServer
/*	Desc:   Abstract interface for particle system
/*****************************************************************************/
class IParticleServer 
{
public: 
    virtual int         GetEffectID     ( const char* name ) = 0;
    virtual int         CreateInstance  ( int effID  ) = 0;
    virtual void        DeleteInstance  ( int instID ) = 0;
    virtual void        Reset           () = 0;
    virtual  bool       Render          ( int   instID, 
                                          float cTime,
                                          const Vec3& pos = Vec3::null, 
                                          float scale = 1.0f,
                                          float alpha = 1.0f, 
                                          const Mat3* pRot = NULL ) = 0;
    virtual void        SetProcessMode  ( uint32_t mode ) = 0;
    virtual void        SetFPS          ( float fps  ) = 0;
    virtual void        ReloadScripts   () = 0;
    virtual void        Enable          ( bool bEnable = true ) = 0;
    virtual bool        IsEnabled       () const = 0;
    virtual uint32_t    GetStamp        () const = 0;
}; // class IParticleServer

extern IParticleServer* g_pParticleServer;
#endif // __IPARTICLESERVER_H__


