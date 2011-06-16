//****************************************************************************/
//  File:   ParticleServer.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PARTICLESERVER_H__
#define __PARTICLESERVER_H__

#include "iparticleserver.h"
#include "jobject.h"

class PSEmitter;

const int c_PSPoolPageSize = 512;
typedef UniformPool<Particle, c_PSPoolPageSize> ParticlePool;
typedef UniformPool<EmitterInstance, c_PSPoolPageSize> EmitterPool;

//****************************************************************************/
//  Enum:  PSProcessMode
//  Desc:    
//****************************************************************************/
enum PSProcessMode
{
    ProcessMode_Visual   = 0x01,
    ProcessMode_State    = 0x02,
}; // enum PSProcessMode

//****************************************************************************/
//  Class:  ParticleServer
//  Desc:    
//****************************************************************************/
class ParticleServer : public JObject, public IParticleServer
{
    std::vector<PSEmitter*>     m_Effects;      //  registry of the all effects loaded


    ParticlePool        m_Particles;    //  particle pool
    EmitterPool         m_Emitters;     //  emitter pool 
    float               m_InstTimeout;  //  timeout for instance before auto-delete?
    float               m_Delta;        //  maximal update time delta 
    uint32_t               m_ProcessMode;  //  current particle system processing mode (i.e. visual only, state only etc.)
    bool                m_bEnabled;     //  whether particle system enabled
    uint32_t               m_Stamp;        //  used for checking whether effect instance is outdated and needs to be recreated

public: 
                        ParticleServer  ();
                        ~ParticleServer ();
    virtual int         GetEffectID     ( const char* name );
    virtual int         CreateInstance  ( int effID  );
    virtual void        DeleteInstance  ( int instID );
    virtual void        Reset           ();
    virtual void        Init            () { SetName( "particle" ); }
    virtual  bool       Render          ( int instID, 
                                          float cTime,
                                          const Vec3& pos = Vec3::null, 
                                          float scale = 1.0f,
                                          float alpha = 1.0f, 
                                          const Mat3* pRot = NULL );
    virtual void        SetProcessMode  ( uint32_t mode ) { m_ProcessMode = mode; }
    virtual void        SetFPS          ( float fps  ) { m_Delta = 1.0f/fps; }
    virtual void        ReloadScripts   ();
    virtual void        Enable          ( bool bEnable = true ) { m_bEnabled = bEnable; }
    virtual bool        IsEnabled       () const { return m_bEnabled; }
    virtual uint32_t       GetStamp        () const { return m_Stamp; }

    Particle*           CreateParticle  ( int num = 1, Particle* pPrev = NULL );
    int                 GetNParticles   () const { return m_Particles.GetNElem(); }
    int                 GetNEmitters    () const { return m_Emitters.GetNElem(); }
    void                DeleteParticle  ( Particle* pParticle );
    EmitterInstance*    CreateChildInst ( EmitterInstance* pInst, Particle* pParticle );

    expose(ParticleServer)
    {
        parent(JObject);
        method( "ReloadScripts", ReloadScripts );
    }

}; // class ParticleServer

#endif // __PARTICLESERVER_H__


