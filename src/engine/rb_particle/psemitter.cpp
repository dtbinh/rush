//****************************************************************************/
//  File:   PSEmitter.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "ParticleServer.h"
#include "PSOperator.h"
#include "PSEmitter.h"

//****************************************************************************/
/*  PSEmitter implementation
//****************************************************************************/
decl_class(PSEmitter);
PSEmitter::PSEmitter()
{
    m_Pos           = Vec3::null;       
    m_Color         = 0xFFFFFFFF;
    m_Size          = Vec3( 10.0f, 10.0f, 10.0f );
    m_LifeTime      = 5.0f;
    m_LifeTimeD     = 0.0f;
    m_Rate          = 1.0f;
    m_Period        = 0.0f;
    m_EmitMode      = EmitMode_Static;
    m_CreateMode    = CreateMode_OnBirth;
    m_StartTime     = 0.0f;
    m_Duration      = -1.0f;
    m_bParentSpace   = true;
} // PSEmitter::PSEmitter

void PSEmitter::InitParticle( EmitterInstance* pInst, Particle* pParticle ) const
{
    pParticle->m_Pos            = m_Pos;
    pParticle->m_Velocity       = Vec3::null;
    pParticle->m_Color          = m_Color;
    pParticle->m_StartColor     = m_Color;
    pParticle->m_Size           = m_Size;
    pParticle->m_StartSize      = m_Size;
    pParticle->m_RotX           = 0.0f;
    pParticle->m_RotY           = 0.0f;
    pParticle->m_RotZ           = 0.0f;
    pParticle->m_AngVelocity    = Vec3::null;
    pParticle->m_TimeToLive     = m_LifeTime + frand( -m_LifeTimeD, m_LifeTimeD );
    pParticle->m_Age            = 0.0f;
    pParticle->m_Flags          = pfJustBorn;
    pParticle->m_pEmitterInst   = pInst;
    pParticle->m_Index          = pInst->m_NEmitted;

    if (!m_bParentSpace) 
    {
        pParticle->m_Pos = pInst->GetPos();
    }

} // PSEmitter::InitParticle

bool PSEmitter::Process( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps )
{
    PSEmitter* pEff = pInst->m_pEmitter;
    assert( pEff );

    float cTime = float( pInst->m_CurTime ) + dt;
    //  number of particles to emit at this time delta
    if (m_EmitMode == EmitMode_Static)
    {
        if (!pInst->m_pParticle && cTime >= m_StartTime)
        {
            pInst->m_pParticle = ps->CreateParticle();
            InitParticle( pInst, pInst->m_pParticle );
            pInst->m_NumToEmit = 0;
            pInst->m_NEmitted  = 1;
            pInst->m_pParticle->m_TimeToLive = 
                m_Duration > 0.0f ? m_Duration : c_VeryBigLifeTime;
        }
    }
    else if (m_EmitMode == EmitMode_Burst)
    {
        //  default number to emit with burst emit rate
        float relTime = cTime - m_StartTime;
        if (relTime >= 0.0f)
        {
            float period = (m_Period != 0.0f) ? m_Period : m_Duration;
            int num = (relTime - fmodf( relTime, period ) + 1.0f)*m_Rate;
            pInst->m_NumToEmit = num - pInst->m_NEmitted;
        }
    }
    else if (m_EmitMode == EmitMode_Normal)
    {
        //  default number to emit with constant emit rate
        int num = (cTime - m_StartTime)*m_Rate;
        pInst->m_NumToEmit = num - pInst->m_NEmitted;
    }

    if (pInst->m_NumToEmit < 0)
    {
        pInst->m_NumToEmit = 0;
    }

    //  perform integration on particles, find particles with ended lifetime
    Particle* pParticle     = pInst->m_pParticle;
    while (pParticle)
    {
        pParticle->m_Age  += dt;
        pParticle->m_Pos  += dt*pParticle->m_Velocity;
        
        pParticle->m_RotX += dt*pParticle->m_AngVelocity.x;
        pParticle->m_RotY += dt*pParticle->m_AngVelocity.y;
        pParticle->m_RotZ += dt*pParticle->m_AngVelocity.z;
        
        if (pParticle->m_Age >= pParticle->m_TimeToLive)
        {
            pParticle->m_Flags |= pfJustDied;
        }

        pParticle = pParticle->m_pNext;
    }

    //  apply operators
    int nOp = pEff->GetNChildren();
    for (int i = 0; i < nOp; i++)
    {
        PSOperator* pOp = (PSOperator*)pEff->GetChild( i );
        if (pOp->IsVisible())
        {
            pOp->Update( pInst, dt, mode, ps );
        }
    }
    
    //  purge dead particles, or reset just born status
    pParticle = pInst->m_pParticle;
    while (pParticle)
    {
        Particle* pCurParticle = pParticle;
        pParticle = pParticle->m_pNext;
        if (pCurParticle->m_Flags & pfJustDied)
        {
            ps->DeleteParticle( pCurParticle );
        }

        if (pCurParticle->m_Flags & pfJustBorn)
        {
            pCurParticle->m_Flags = 0;
        }
    }

    //  emit new particles
    if (pInst->m_NumToEmit > 0)
    {
        //  find last particle in chain
        pParticle = pInst->m_pParticle;
        Particle* pLastParticle = pParticle;
        while (pParticle)
        {
            pLastParticle = pParticle;
            pParticle = pParticle->m_pNext;
        }
        
        //  create particles
        float numToEmit = float( pInst->m_NumToEmit )*pInst->m_RateMultiplier;
        Particle* pNewParticles = ps->CreateParticle( (int)numToEmit, pLastParticle );
        if (!pInst->m_pParticle) 
        {
            pInst->m_pParticle = pNewParticles;
        }
        pInst->m_NEmitted  += pInst->m_NumToEmit;

        //  init emitted particles
        Particle* pCurParticle = pNewParticles;
        while (pCurParticle)
        {
            InitParticle( pInst, pCurParticle );
            pCurParticle = pCurParticle->m_pNext;
        }
    }

    //  process children emitters
    EmitterInstance* pChild = pInst->m_pChild;
    while (pChild)
    {
        PSEmitter* pEff = pChild->m_pEmitter;
        assert( pEff );
        pEff->Process( pChild, dt, mode, ps );
        pChild = pChild->m_pNext;
    }

    //  update emitter's properties
    pInst->m_PrevPos  = pInst->GetPos();
    pInst->m_CurTime += dt;
    if (pInst->m_pHostParticle) 
    {
        pInst->m_Pos = pInst->m_pHostParticle->m_Pos;
    }
    float duration = m_Duration + m_StartTime;
    bool bHostDied = pInst->m_pHostParticle && (pInst->m_pHostParticle->m_Flags & pfJustDied);
    if ((duration >= 0.0f && pInst->m_CurTime > duration) || bHostDied)
    {
        if (pInst->m_pParent)
        {
            ps->DeleteInstance( reinterpret_cast<int>( pInst ) );
        }
        else 
        {
            return false;
        }
    }
    
    return true;
} // PSEmitter::Process

void PSEmitter::Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps )
{
    //  create child emitter instance, if appropriate
    Particle* pParticle = pInst->m_pParticle;
    DWORD mask = 0;
         if (m_CreateMode == CreateMode_OnBirth) mask = pfJustBorn;
    else if (m_CreateMode == CreateMode_OnDeath) mask = pfJustDied;
    else if (m_CreateMode == CreateMode_OnHit)   mask = pfJustHit;
    else return;

    while (pParticle)
    {
        if (pParticle->m_Flags & mask)
        {
            EmitterInstance* pNewInst = ps->CreateChildInst( pInst, pParticle );
            pNewInst->m_pEmitter = AssignEmitter( pNewInst, pParticle );
        }
        pParticle = pParticle->m_pNext;
    }
} // PSEmitter::Update
