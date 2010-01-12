//****************************************************************************/
//  File:   PSForce.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "particle.h"
#include "emitterinstance.h"
#include "particleserver.h"
#include "psoperator.h"
#include "psforce.h"

//****************************************************************************/
/*  PSForce implementation
//****************************************************************************/
decl_class(PSForce);
PSForce::PSForce()
{
    m_Force = Vec3( 0.0f, 1.0f, 0.0f );
} // PSForce::PSForce

void PSForce::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    Vec3 dv = m_Force;
	dv *= dt;

    Particle* p = pInst->m_pParticle;
    while (p)
    {
        p->m_Velocity += dv;
        p = p->m_pNext;
    }
} // PSForce::Update
    
