//****************************************************************************/
//  File:   PSBoxPlacer.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "particle.h"
#include "emitterinstance.h"
#include "particleserver.h"
#include "psoperator.h"
#include "psboxplacer.h"

//****************************************************************************/
/*  PSBoxPlacer implementation
//****************************************************************************/
decl_class(PSBoxPlacer);
PSBoxPlacer::PSBoxPlacer()
{
    m_bPlanar   = false;      
    m_bSurface  = false;    
    m_Ext       = Vec3( 100.0f, 100.0f, 100.0f );
} // PSBoxPlacer::PSBoxPlacer

void PSBoxPlacer::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    Particle* p = pInst->m_pParticle;
    while (p)
    {
        if ((p->m_Flags&pfJustBorn) != 0)
        {
            Vec3 pos;
            pos.x = frand( -m_Ext.x, m_Ext.x )*0.5f;
            pos.y = frand( -m_Ext.y, m_Ext.y )*0.5f;
            pos.z = frand( -m_Ext.z, m_Ext.z )*0.5f;

            if (m_bSurface)
            {
                if (pos.x < 0.0f) pos.x = -m_Ext.x*0.5f; else pos.x = m_Ext.x*0.5f;
                if (pos.y < 0.0f) pos.y = -m_Ext.y*0.5f; else pos.y = m_Ext.y*0.5f;
                if (pos.z < 0.0f) pos.z = -m_Ext.z*0.5f; else pos.z = m_Ext.z*0.5f;
            }
            if (m_bPlanar) pos.z = 0.0f;

            p->m_Pos += pos;
        }
        p = p->m_pNext;
    }
} // PSBoxPlacer::Update

