//****************************************************************************/
//  File:   ParticleServer.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "particle.h"
#include "emitterinstance.h"
#include "psoperator.h"
#include "psemitter.h"
#include "ipersistserver.h"
#include "particleserver.h"

//****************************************************************************/
/*  ParticleServer implementation
//****************************************************************************/
decl_class(ParticleServer);
ParticleServer::ParticleServer()
{
    m_InstTimeout  = 10.0f;
    m_Delta        = 0.05; // 20 fps
    m_ProcessMode  = ProcessMode_Visual | ProcessMode_State;
    m_bEnabled     = true;
    m_Stamp        = 0;

    g_pParticleServer            = this;
} // ParticleServer::ParticleServer

ParticleServer::~ParticleServer()
{

}

int ParticleServer::GetEffectID( const char* name )
{
    int nEff = (int)m_Effects.size();
    for (int i = 0; i < nEff; i++)
    {
        if (!stricmp( name, m_Effects[i]->GetName() ))
        {
            return i;
        }
    }

    JObject* pObject = g_pPersistServer->LoadObject( name );
    PSEmitter* pEmitter = obj_cast<PSEmitter>( pObject );
    if (!pEmitter) 
    {
        delete pObject;
        return -1;
    }
    pEmitter->SetName( name );
    pEmitter->InitTree();
    m_Effects.push_back( pEmitter );

    AddChild( pEmitter );
    pEmitter->AddRef();

    return m_Effects.size() - 1;
} // ParticleServer::GetEffectID

int ParticleServer::CreateInstance( int effID )
{
    if (effID < 0 || effID >= m_Effects.size()) return -1;
    PSEmitter* pEmitter = m_Effects[effID];

    EmitterInstance* pInst = m_Emitters.Allocate();
    pInst->Init();
    pInst->m_pEmitter       = pEmitter;

    return reinterpret_cast<int>( pInst );
} // ParticleServer::CreateInstance

EmitterInstance* ParticleServer::CreateChildInst( EmitterInstance* pInst, Particle* pParticle )
{
    EmitterInstance* pNewInst = m_Emitters.Allocate();

    pNewInst->Init();
    pNewInst->m_pEmitter        = NULL;
    pNewInst->m_pHostParticle   = pParticle; 
    pNewInst->m_pParent         = pInst;

    pInst->AddChild( pNewInst );

    return pNewInst;
} // ParticleServer::CreateChildInst

void ParticleServer::DeleteInstance( int instID )
{
    if (instID == -1 || instID == 0) return;
    EmitterInstance* pInst = reinterpret_cast<EmitterInstance*>( instID );
    
    //  remove all particles
    Particle* pParticle = pInst->m_pParticle;
    while (pParticle)
    {
        Particle* pNext = pParticle->m_pNext;
        m_Particles.Free( pParticle );
        pParticle = pNext;
    }

    //  remove all children emitter instances
    EmitterInstance* pChildInst = pInst->m_pChild;
    while (pChildInst)
    {
        EmitterInstance* pNextInst = pChildInst->m_pNext;
        DeleteInstance( reinterpret_cast<int>( pChildInst ) );
        pChildInst = pNextInst;
    }

    EmitterInstance* pParent = pInst->m_pParent;
    if (pParent)
    {
        if (pParent->m_pChild == pInst)
        {
            pParent->m_pChild = pInst->m_pNext;
        }
    }

    if (pInst->m_pPrev)
    {
        pInst->m_pPrev->m_pNext = pInst->m_pNext;
    }

    if (pInst->m_pNext)
    {
        pInst->m_pNext->m_pPrev = pInst->m_pPrev;
    }

    //  remove root instance itself
    m_Emitters.Free( pInst );
} // ParticleServer::DeleteInstance

bool ParticleServer::Render( int instID, float cTime, const Vec3& pos, 
                             float scale, float alpha, const Mat3* pRot )
{
    if (instID == -1 || instID == 0 || !m_bEnabled) return false;
    EmitterInstance* pInst = reinterpret_cast<EmitterInstance*>( instID );
    
    pInst->m_Pos    = pos;
    pInst->m_Scale  = scale;
    pInst->m_Alpha  = alpha;

    if (pInst->m_NEmitted == 0) pInst->m_PrevPos = pos;

    //  process emitter, making sure that minimal processing step is less or equal to m_Delta
    float dTime = cTime - pInst->m_CurTime;
    if (dTime <= 0.0f) 
    {
        dTime = 0.0f;
    }
    else if (dTime > m_Delta)
    {
        uint32_t mode = ProcessMode_State;
        while (dTime > m_Delta)
        {
            pInst->m_pEmitter->Process( pInst, m_Delta, mode, this );
            dTime -= m_Delta;
        }
    }
    pInst->m_pEmitter->Process( pInst, dTime, m_ProcessMode, this );

    return true;
} // ParticleServer::Render

void ParticleServer::Reset()
{
    m_Particles.Clear();
    m_Emitters.Clear();
} // ParticleServer::Reset

Particle* ParticleServer::CreateParticle( int num, Particle* pPrev )
{
    if (num == 0) return NULL; 
    Particle* pFirst  = m_Particles.Allocate();
    pFirst->m_pPrev   = pPrev;
    pFirst->m_pEmitterInst = NULL;

    Particle* pCur = pFirst;
    for (int i = 1; i < num; i++)
    {
        Particle* pNext = m_Particles.Allocate();
        pNext->m_pPrev  = pCur;
        pCur->m_pNext   = pNext;
        pNext->m_pNext  = NULL;
        pCur = pNext;
        pCur->m_pEmitterInst = NULL;
    }

    if (pPrev)
    {
        pCur->m_pNext   = pPrev->m_pNext;
        pPrev->m_pNext  = pFirst;
    }
    else
    {
        pFirst->m_pPrev = NULL;
        pCur->m_pNext   = NULL;
    }

    return pFirst;
} // ParticleServer::CreateParticle

void ParticleServer::DeleteParticle( Particle* pParticle )
{
    if (!pParticle) return;
    if (pParticle->m_pPrev)
    {
        pParticle->m_pPrev->m_pNext = pParticle->m_pNext;
    }
    else
    {
        EmitterInstance* pInst = pParticle->m_pEmitterInst;
        if (pInst)
        {
            pInst->m_pParticle = pParticle->m_pNext;
        }
    }

    if (pParticle->m_pNext)
    {
        pParticle->m_pNext->m_pPrev = pParticle->m_pPrev;
    }
    m_Particles.Free( pParticle );
} // ParticleServer::DeleteParticle

void ParticleServer::ReloadScripts()
{
    Reset();
    int nEff = m_Effects.size();
    for (int i = 0; i < nEff; i++)
    {
        JObject* pObject = g_pPersistServer->LoadObject( m_Effects[i]->GetName() );
        PSEmitter* pEmitter = obj_cast<PSEmitter>( pObject );
        if (!pEmitter) 
        {
            delete pObject;
            continue;
        }
        pEmitter->SetName( m_Effects[i]->GetName() );
        delete m_Effects[i];
        m_Effects[i] = pEmitter;
    }

    m_Stamp++;
} // ParticleServer::ReloadScripts
