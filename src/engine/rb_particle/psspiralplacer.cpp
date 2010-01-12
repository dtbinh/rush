//****************************************************************************/
//  File:   PSSpiralPlacer.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "ParticleServer.h"
#include "PSOperator.h"
#include "PSSpiralPlacer.h"

//****************************************************************************/
/*  PSSpiralPlacer implementation
//****************************************************************************/
decl_class(PSSpiralPlacer);
PSSpiralPlacer::PSSpiralPlacer()
{
    m_Radius     = 10.0f;      
    m_Period     = 0.5f;    
    m_bRelToDir  = true;
} // PSSpiralPlacer::PSSpiralPlacer

void PSSpiralPlacer::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    float angle = c_PI*2.0f*pInst->m_CurTime/m_Period;
    float cosAng = cosf( angle );
    float sinAng = sinf( angle );
    Particle* p = pInst->m_pParticle;
    while (p)
    {
        if ((p->m_Flags&pfJustBorn) != 0)
        {
            Vec3 vx = Vec3::oX;
            Vec3 vy = Vec3::oY;
            Vec3 vz = Vec3::oZ;
            if (m_bRelToDir)
            {
                vz = p->m_Velocity;
                vz.normalize();
                vz.ortonormalize( vx, vy );
            }
            Vec3 pos = vx*cosAng + vy*sinAng;
            p->m_Pos += pos*m_Radius;
        }
        p = p->m_pNext;
    }
} // PSSpiralPlacer::Update

