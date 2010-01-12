//****************************************************************************/
//  File:   PSAlphaRamp.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "particle.h"
#include "emitterinstance.h"
#include "particleserver.h"
#include "psoperator.h"
#include "psemitter.h"
#include "psalpharamp.h"

//****************************************************************************/
/*  PSAlphaRamp implementation
//****************************************************************************/
decl_class(PSAlphaRamp);
PSAlphaRamp::PSAlphaRamp()
{
    m_bRelative = true;
    m_NRepeats  = 1;
    m_Usage     = RampUsage_Particle;
} // PSAlphaRamp::PSAlphaRamp

void PSAlphaRamp::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    if (m_Usage == RampUsage_Particle)
    {
        Particle* p = pInst->m_pParticle;
        while (p)
        {
            float ratio = fmodf( float( m_NRepeats )*p->m_Age/p->m_TimeToLive, 1.0f );
            float alpha = m_Ramp.GetValue( ratio );
            if (m_bRelative) alpha *= p->GetStartAlpha();
            p->SetAlpha( alpha );
            p = p->m_pNext;
        }
    }
    else if (m_Usage == RampUsage_Emitter)
    {
        Particle* p = pInst->m_pParticle;
        float ratio = fmodf( float( m_NRepeats )*pInst->m_CurTime/pInst->m_pEmitter->GetDuration(), 1.0f );
        while (p)
        {
            if ((p->m_Flags&pfJustBorn) != 0)
            {
                float alpha = m_Ramp.GetValue( ratio );
                if (m_bRelative) alpha *= p->GetAlpha();
                p->SetAlpha( alpha );
                p->SetStartAlpha( alpha );
            }
            p = p->m_pNext;
        }
    }
    else if (m_Usage == RampUsage_Init)
    {
        Particle* p = pInst->m_pParticle;
        while (p)
        {
            if ((p->m_Flags&pfJustBorn) != 0)
            {
                float ratio = frand( 0.0f, 1.0f );
                float alpha = m_Ramp.GetValue( ratio );
                if (m_bRelative) alpha *= p->GetAlpha();
                p->SetAlpha( alpha );
                p->SetStartAlpha( alpha );
            }
            p = p->m_pNext;
        }
    }
} // PSAlphaRamp::Update
