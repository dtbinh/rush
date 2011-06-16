//****************************************************************************/
//  File:   Particle.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vec3.h"

//****************************************************************************/
//  Enum:  ParticleFlags
//  Desc:    
//****************************************************************************/
enum ParticleFlags
{
    pfJustDied  = 0x01,
    pfDead      = 0x02,
    pfJustBorn  = 0x04,
    pfJustHit   = 0x08
}; // enum ParticleFlags

struct EmitterInstance;
//****************************************************************************/
//  Class:  Particle
//  Desc:    
//****************************************************************************/
struct Particle  
{
    Particle*           m_pNext;            //  next particle in list
    Particle*           m_pPrev;            //  previous particle in list

    EmitterInstance*    m_pEmitterInst;     //  owner emitter instance

    Vec3                m_Pos;              //  particle position
    float               m_RotY;             //  particle rotation around Y
    float               m_RotZ;             //  particle rotation around Z
    float               m_RotX;             //  particle rotation around X

    Vec3                m_Size;             //  particle size
    Vec3                m_StartSize;        //  particle size at creation time

    int                 m_Frame;            //  current frame in framed animation
    uint32_t               m_Color;            //  particle alpha/color
    uint32_t               m_StartColor;       //  particle alpha/color at creation time
    
    float               m_Age;              //  current age
    float               m_TimeToLive;       //  total time to live
    
    uint32_t               m_Flags;            

    Vec3                m_Velocity;         //  velocity vector
    Vec3                m_AngVelocity;      //  angular velocity vector
    uint32_t               m_Index;            //  index of the particle inside it's emmiter
    
    uint32_t               pad[4];             //  padding to 128 bytes


    void SetAlpha( float alpha )
    {
        uint32_t ba = alpha*255.0f;
        m_Color &= 0x00FFFFFF;
        m_Color |= (ba << 24);
    }

    void SetStartAlpha( float alpha )
    {
        uint32_t ba = alpha*255.0f;
        m_StartColor &= 0x00FFFFFF;
        m_StartColor |= (ba << 24);
    }

    float GetAlpha() const
    {
        return ((m_Color&0xFF000000)>>24)/255.0f;
    }

    float GetStartAlpha() const
    {
        return ((m_StartColor&0xFF000000)>>24)/255.0f;
    }

}; // class Particle

#endif // __PARTICLE_H__


