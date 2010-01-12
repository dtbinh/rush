//****************************************************************************/
//  File:   PSSprite.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "particle.h"
#include "emitterinstance.h"
#include "particleserver.h"
#include "psoperator.h"
#include "psemitter.h"
#include "pssprite.h"

//****************************************************************************/
/*  PSSprite implementation
//****************************************************************************/
decl_class(PSSprite);
PSSprite::PSSprite()
{
    m_Package       = "particles";
    m_PackageID     = -1;
    m_Frame         = 0;
    m_bAdditive     = false;
    m_bFlush        = false;
    m_bScreenSpace  = true;
} // PSSprite::PSSprite

void PSSprite::Init()
{
    m_PackageID = g_pDrawServer->GetSpriteID( m_Package.c_str(), NULL, true );
} // PSSprite::Init

void PSSprite::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_Visual) == 0) return;

    const Mat4& vm = g_pRenderServer->GetViewTM();

    Particle* p = pInst->m_pParticle;
    g_pDrawServer->SetAdditive( m_bAdditive );
    g_pDrawServer->SetZEnable( true );
    g_pDrawServer->SetZWriteEnable( false );
    bool bLocalSpace = pInst->m_pEmitter->IsParentSpace();
    while (p)
    {
        if (m_bScreenSpace)
        {
            Frame rct( p->m_Pos.x, p->m_Pos.y, p->m_Size.x, p->m_Size.y );
            if (bLocalSpace)
            {
                Vec3 pos = pInst->GetPos();
                rct.x += pos.x;
                rct.y += pos.y;
            }
            g_pDrawServer->DrawSprite( rct, m_PackageID, m_Frame, p->m_Color, p->m_RotZ );
        }
        else
        { 
            Vec3 pos = p->m_Pos;
            if (bLocalSpace)
            {
                pos += pInst->GetPos();
            }
            Vec2 pivot = g_pDrawServer->GetFramePivot( m_PackageID, m_Frame );
            float sx = sinf( p->m_RotZ );
            float cx = cosf( p->m_RotZ );

            Mat4 tm(  p->m_Size.x*cx, p->m_Size.x*sx, 0.0f, 0.0f,
                     -p->m_Size.y*sx, p->m_Size.y*cx, 0.0f, 0.0f, 
                      0.0f,           0.0f,           1.0f, 0.0f, 
                     -pivot.x*p->m_Size.x*cx + pivot.y*p->m_Size.y*sx, 
                     -pivot.x*p->m_Size.x*sx - pivot.y*p->m_Size.y*cx, 
                     0.0f, 1.0f ); 

            tm *= Mat4( 
                 vm.e00, vm.e10, vm.e20, 0.0f, 
                -vm.e01,-vm.e11,-vm.e21, 0.0f, 
                 vm.e02, vm.e12, vm.e22, 0.0f,
                 pos.x,  pos.y,  pos.z,  1.0f );
            
            g_pDrawServer->DrawSprite( tm, m_PackageID, m_Frame, p->m_Color );
        }
        p = p->m_pNext;
    }

    if (m_bFlush) g_pDrawServer->Flush();
    g_pDrawServer->SetAdditive( false );
} // PSSprite::Update
    
