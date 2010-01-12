//****************************************************************************/
//  File:   PSVelRamp.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "PSOperator.h"
#include "ParticleServer.h"
#include "PSVelRamp.h"

//****************************************************************************/
/*  PSVelRamp implementation
//****************************************************************************/
decl_class(PSVelRamp);
PSVelRamp::PSVelRamp()
{
    m_NRepeats = 1;
} // PSVelRamp::PSVelRamp

void PSVelRamp::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;
    Particle* p = pInst->m_pParticle;
    while (p)
    {
        float ratio = fmodf( float( m_NRepeats )*p->m_Age/p->m_TimeToLive, 1.0f );
        float velocity = m_Ramp.GetValue( ratio );
        Vec3 dir = p->m_Velocity;
        dir.normalize();
        dir *= velocity;
        p->m_Velocity = dir;
        p = p->m_pNext;
    }
} // PSVelRamp::Update

