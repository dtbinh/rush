#include "stdafx.h"

#include "UniformPool.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "IParticleServer.h"
#include "ParticleServer.h"

/*
static ParticleServer  ps;
static Particle*       pParticle;
static int             bubblesID, explodeID;

TEST( ParticleHasGoodStride )
{
    CHECK_EQUAL( 128, sizeof( Particle ) );
}

TEST( NewParticlesAreCreated )
{
    pParticle = ps.CreateParticle( 71 );
    CHECK( pParticle );
    CHECK_EQUAL( 71, ps.GetNParticles() );

    ps.CreateParticle( 6, pParticle );
    CHECK( pParticle );
    CHECK_EQUAL( 77, ps.GetNParticles() );
}

TEST( TestParticleChain )
{
    CHECK( !pParticle->m_pPrev );
    Particle* pCur = pParticle;
    int nParticles = 0;
    while (pCur)
    {
        pCur = pCur->m_pNext;
        nParticles++;
    }
    CHECK_EQUAL( 77, nParticles );
}

TEST( ParticlesAreDeleted )
{
    for (int i = 0; i < 41; i++)
    {   
        Particle* pDeleted = pParticle;
        pParticle = pParticle->m_pNext;
        ps.DeleteParticle( pDeleted );
    }
    CHECK( pParticle );
    CHECK_EQUAL( 77 - 41, ps.GetNParticles() );
}

TEST( ResetParticleServer )
{
    ps.Reset();
    CHECK_EQUAL( 0, ps.GetNParticles() );
}


TEST( LoadEffectFile )
{
    bubblesID = ps.GetEffectID( "ps_bubbles" );
    CHECK_EQUAL( 0, bubblesID );
    CHECK_EQUAL( 0, ps.GetNParticles() );
}

static int explodeInst0, bubblesInst0, bubblesInst1;
TEST( EffectsAreInstanced )
{
    explodeInst0 = ps.CreateInstance( bubblesID );
    CHECK_EQUAL( 1, ps.GetNEmitters() );
    CHECK_EQUAL( 0, ps.GetNParticles() );
}

TEST( MoreThanOneInstance )
{
    bubblesInst0 = ps.CreateInstance( bubblesID );
    bubblesInst1 = ps.CreateInstance( bubblesID );
    CHECK_EQUAL( 3, ps.GetNEmitters() );
    CHECK_EQUAL( 0, ps.GetNParticles() );
}

TEST( EffectInstanceDeletion )
{
    ps.DeleteInstance( explodeInst0 );
    CHECK_EQUAL( 2, ps.GetNEmitters() );
    CHECK_EQUAL( 0, ps.GetNParticles() );
}

TEST( DifferentEffectInstance )
{
    explodeID = ps.GetEffectID( "ps_explode" );
    explodeInst0 = ps.CreateInstance( explodeID );
    CHECK_EQUAL( 3, ps.GetNEmitters() );
    CHECK_EQUAL( 0, ps.GetNParticles() );
}

TEST( HierarchicalEffectInstancing )
{
    ps.Render( explodeInst0, 0.0f );
    CHECK_EQUAL( 8, ps.GetNEmitters() );
    CHECK_EQUAL( 1, ps.GetNParticles() );
}

TEST( ParticleSpawningNormalEmitter )
{ 
    ps.Render( bubblesInst0, 1.0f );
    CHECK_EQUAL( 11, ps.GetNParticles() );

    ps.Render( bubblesInst0, 1.5f );
    CHECK_EQUAL( 15, ps.GetNParticles() );

    ps.Render( bubblesInst0, 2.0f );
    CHECK_EQUAL( 20, ps.GetNParticles() );
}

TEST( ParticleRemovingOnDeath )
{
    ps.Render( bubblesInst0, 4.0f );
    CHECK_EQUAL( 20 + 1, ps.GetNParticles() );
    ps.Render( bubblesInst0, 5.0f );
    CHECK_EQUAL( 21 + 1, ps.GetNParticles() );
}

TEST( SubEmittersRemovingOnParentParticleDeath )
{
    
}
*/