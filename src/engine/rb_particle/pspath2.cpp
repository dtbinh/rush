//****************************************************************************/
//  File:   PSPath2.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "ParticleServer.h"
#include "PSOperator.h"
#include "PSPath2.h"

//****************************************************************************/
/*  PSPath2 implementation
//****************************************************************************/
decl_class(PSPath2);
PSPath2::PSPath2()
{
    m_bClosed   = false;
    m_Magnitude = 1.0f;
} // PSPath2::PSPath2

void PSPath2::Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    Particle* p = pInst->m_pParticle;
    while (p)
    {
        float ratio = p->m_Age/p->m_TimeToLive;
        Vec2 tan( 0.0f, 0.0f );
        Vec2 pt = m_Path.GetParamPoint( ratio, m_bClosed, &tan );
        
        float vel = p->m_Velocity.normalize();
        tan.normalize();
        Vec3 newVel = Vec3( tan.x, tan.y, 0.0f )*m_Magnitude + p->m_Velocity*(1.0f - m_Magnitude);
        newVel *= vel;
        p->m_Velocity = newVel;

        p = p->m_pNext;
    }
} // PSPath2::Update
    