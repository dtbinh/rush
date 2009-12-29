//****************************************************************************/
//  File:   PSSizeRamp.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "ParticleServer.h"
#include "PSOperator.h"
#include "PSEmitter.h"
#include "PSSizeRamp.h"

//****************************************************************************/
/*  PSSizeRamp implementation
//****************************************************************************/
decl_class(PSSizeRamp);
PSSizeRamp::PSSizeRamp()
{
    m_bRelative = true;
    m_NRepeats  = 1;
    m_Usage     = RampUsage_Particle;
    m_Axis      = Axis_XY;
} // PSSizeRamp::PSSizeRamp

void PSSizeRamp::Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    Vec3 mask( 0.0f, 0.0f, 0.0f );
    if (m_Axis&Axis_X) mask.x = 1.0f;
    if (m_Axis&Axis_Y) mask.y = 1.0f;
    if (m_Axis&Axis_Z) mask.z = 1.0f;

    if (m_Usage == RampUsage_Particle)
    {
        Particle* p = pInst->m_pParticle;
        while (p)
        {
            float ratio = fmodf( float( m_NRepeats )*p->m_Age/p->m_TimeToLive, 1.0f );
            float size = m_Ramp.GetValue( ratio );
            Vec3 vSize( size*mask.x, size*mask.y, size*mask.z );
            if (m_bRelative) vSize *= p->m_StartSize;
            p->m_Size = vSize;
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
                float size = m_Ramp.GetValue( ratio );
                Vec3 vSize( size*mask.x, size*mask.y, size*mask.z );
                if (m_bRelative) vSize *= p->m_StartSize;
                p->m_Size = vSize;
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
                float size = m_Ramp.GetValue( ratio );
                Vec3 vSize( size*mask.x, size*mask.y, size*mask.z );
                if (m_bRelative) vSize *= p->m_StartSize;
                p->m_Size = vSize;
            }
            p = p->m_pNext;
        }
    }
} // PSSizeRamp::Update
