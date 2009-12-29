//****************************************************************************/
//  File:   PSPolyPlacer.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "ParticleServer.h"
#include "PSOperator.h"
#include "PSPolyPlacer.h"

//****************************************************************************/
/*  PSPolyPlacer implementation
//****************************************************************************/
decl_class(PSPolyPlacer);
PSPolyPlacer::PSPolyPlacer()
{
    m_bSurface = false;

} // PSPolyPlacer::PSPolyPlacer

void PSPolyPlacer::Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    Particle* p = pInst->m_pParticle;
    Frame ext = m_Contour.GetBound();


    while (p)
    {
        if ((p->m_Flags&pfJustBorn) != 0)
        {
            float fx = 0.0f;
            float fy = 0.0f;
            do
            {
                fx = frand( ext.x, ext.r() );
                fy = frand( ext.y, ext.b() );
            } while (!m_Contour.PtIn( Vec2( fx, fy ) ));

            Vec3 pos( fx, fy, 0.0f );
            p->m_Pos += pos;
        }
        p = p->m_pNext;
    }
} // PSPolyPlacer::Update