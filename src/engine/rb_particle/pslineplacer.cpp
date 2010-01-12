//****************************************************************************/
//  File:   PSLinePlacer.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "particle.h"
#include "emitterinstance.h"
#include "particleserver.h"
#include "psoperator.h"
#include "pslineplacer.h"

//****************************************************************************/
/*  PSLinePlacer implementation
//****************************************************************************/
decl_class(PSLinePlacer);
PSLinePlacer::PSLinePlacer()
{
    m_Start = Vec3( -10.0f, 10.0f, 10.0f );
    m_End   = Vec3(  10.0f, 10.0f, 10.0f );
} // PSLinePlacer::PSLinePlacer

void PSLinePlacer::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    Particle* p = pInst->m_pParticle;
    while (p)
    {
        if ((p->m_Flags&pfJustBorn) != 0)
        {
            float ratio = frand( 0.0f, 1.0f );
            Vec3 pos = m_Start + ratio*(m_End - m_Start);
            p->m_Pos += pos;
        }
        p = p->m_pNext;
    }
} // PSLinePlacer::Update
