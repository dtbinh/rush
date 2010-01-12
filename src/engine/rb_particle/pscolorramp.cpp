//****************************************************************************/
//  File:   PSColorRamp.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "colorramp.h"
#include "particle.h"
#include "emitterinstance.h"
#include "particleserver.h"
#include "psoperator.h"
#include "psemitter.h"
#include "pscolorramp.h"

//****************************************************************************/
/*  PSColorRamp implementation
//****************************************************************************/
decl_class(PSColorRamp);
PSColorRamp::PSColorRamp()
{
    m_bRelative = false;
    m_NRepeats  = 1;
    m_Usage     = RampUsage_Particle;
} // PSColorRamp::PSColorRamp

void PSColorRamp::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    if (m_Usage == RampUsage_Particle)
    {
        Particle* p = pInst->m_pParticle;
        while (p)
        {
            float ratio = fmodf( float( m_NRepeats )*p->m_Age/p->m_TimeToLive, 1.0f );
            uint32_t clr = m_Ramp.GetValue( ratio );
            if (m_bRelative) 
            {
                ColorF clrF( clr );
                ColorF startClr( p->m_StartColor );
                clrF *= startClr;
                clr = clrF;
            }

            clr &= 0x00FFFFFF;
            p->m_Color &= 0xFF000000;
            p->m_Color |= clr;

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
                uint32_t clr = m_Ramp.GetValue( ratio );
                if (m_bRelative) 
                {
                    ColorF clrF( clr );
                    ColorF startClr( p->m_StartColor );
                    clrF *= startClr;
                    clr = clrF;
                }

                clr &= 0x00FFFFFF;
                p->m_Color &= 0xFF000000;
                p->m_Color |= clr;

                p->m_StartColor &= 0xFF000000;
                p->m_StartColor |= clr;
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
                uint32_t clr = m_Ramp.GetValue( ratio );
                if (m_bRelative) 
                {
                    ColorF clrF( clr );
                    ColorF startClr( p->m_StartColor );
                    clrF *= startClr;
                    clr = clrF;
                }

                clr &= 0x00FFFFFF;
                p->m_Color &= 0xFF000000;
                p->m_Color |= clr;

                p->m_StartColor &= 0xFF000000;
                p->m_StartColor |= clr;
            }
            p = p->m_pNext;
        }
    }
} // PSColorRamp::Update
