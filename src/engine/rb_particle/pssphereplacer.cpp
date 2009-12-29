//****************************************************************************/
//  File:   PSSpherePlacer.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "ParticleServer.h"
#include "PSOperator.h"
#include "PSSpherePlacer.h"

//****************************************************************************/
/*  PSSpherePlacer implementation
//****************************************************************************/
decl_class(PSSpherePlacer);
PSSpherePlacer::PSSpherePlacer()
{
    m_bPlanar   = false;      
    m_bSurface  = false;    
    m_Radius    = 100.0f;
} // PSSpherePlacer::PSSpherePlacer

void PSSpherePlacer::Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    Particle* p = pInst->m_pParticle;
    while (p)
    {
        if ((p->m_Flags&pfJustBorn) != 0)
        {
            Vec3 pos = Vec3::null;

            if (m_bPlanar) 
            {
                if (m_bSurface)
                {
                    float ang = frand( 0.0f, c_DoublePI );
                    pos = Vec3( m_Radius * cosf( ang ), m_Radius * sinf( ang ), 0.0f );
                }
                else
                {
                    float r = m_Radius*sqrtf( frand( 0.0f, 1.0f ) );
                    float phi = frand( 0, c_DoublePI );
                    pos = Vec3( r*cosf( phi ), r*sinf( phi ), 0.0f );
                }
            }
            else
            {
                if (m_bSurface)
                {
                    float z = frand( -m_Radius, m_Radius );
                    float phi = frand( 0, c_DoublePI );
                    float cosTheta = sqrtf( 1.0f - z*z/m_Radius/m_Radius );
                    pos = Vec3( m_Radius * cosTheta * cosf( phi ), 
                                m_Radius * cosTheta * sinf( phi ), z );
                }
                else
                {
                    float x, y, z;
                    while (true)
                    {
                        x = frand( -1.0f, 1.0f );
                        y = frand( -1.0f, 1.0f );
                        z = frand( -1.0f, 1.0f );
                        if (x*x + y*y + z*z <= 1.0f) break;
                    }
                    pos = Vec3( m_Radius*x, m_Radius*y, m_Radius*z );
                }
            }
            

            p->m_Pos += pos;
        }
        p = p->m_pNext;
    }
} // PSSpherePlacer::Update
