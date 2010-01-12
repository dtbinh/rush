//****************************************************************************/
//  File:   PSShot.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "particle.h"
#include "emitterinstance.h"
#include "particleserver.h"
#include "psoperator.h"
#include "psshot.h"

//****************************************************************************/
/*  PSShot implementation
//****************************************************************************/
decl_class(PSShot);
PSShot::PSShot()
{
    m_Dir           = Vec3( 0.0f, -1.0f, 0.0f );
    m_ConeAngle     = 20.0f;
    m_ConeAngleD    = 0.0f;
    m_Velocity      = 20.0f;
    m_VelocityD     = 10.0f;
    m_AngVel        = Vec3( 0.0f, 0.0f, 0.0f );
    m_AngVelD       = Vec3( 0.0f, 0.0f, 0.0f );
    m_bPlanar       = false;
    m_bSurface      = false;
} // PSShot::PSShot

void PSShot::Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;

    Particle* p = pInst->m_pParticle;
    while (p)
    {
        if ((p->m_Flags&pfJustBorn) != 0)
        {
            float velMag = frand( m_Velocity - m_VelocityD, m_Velocity + m_VelocityD );
            Vec3 vel = GetDirection()*velMag;
            p->m_Velocity += vel;
            
            p->m_AngVelocity.x = frand( m_AngVel.x - m_AngVelD.x, m_AngVel.x + m_AngVelD.x );
            p->m_AngVelocity.y = frand( m_AngVel.y - m_AngVelD.y, m_AngVel.y + m_AngVelD.y );
            p->m_AngVelocity.z = frand( m_AngVel.z - m_AngVelD.z, m_AngVel.z + m_AngVelD.z );
        }
        p = p->m_pNext;
    }
} // PSShot::Update

Vec3 PSShot::GetDirection() const
{
    Vec3  dir = m_Dir;
	if (m_bPlanar)
	{
        float ang = 0.0f;
		if (m_bSurface)
        {
            ang = DegToRad( frand( m_ConeAngle - m_ConeAngleD, m_ConeAngle + m_ConeAngleD ) );
            int v = rand()%2;
            if (v == 1) 
            {
                ang = -ang;
            }
        }
        else
        {
            ang = DegToRad( frand( -m_ConeAngle - m_ConeAngleD, m_ConeAngle + m_ConeAngleD ) );
        }

        float sa = sinf( ang );
        float ca = cosf( ang );
        Vec3 ndir( dir.x*ca - dir.y*sa, dir.x*sa + dir.y*ca, dir.z );
        dir = ndir;
	}
	else 
	{
        if (m_bSurface)
        {
            float fullAng = frand( 0.0f, c_DoublePI );
            float ang = DegToRad( frand( -m_ConeAngle - m_ConeAngleD, m_ConeAngle + m_ConeAngleD ) );
            float cAng = cosf( ang );
            float sAng = sinf( ang );
            dir += Vec3( sAng*cosf( fullAng ), sAng*sinf( fullAng ), cAng ); 
        }
        else
        {
		    float r1 = frand( 0.0f, 1.0f );
		    float r2 = frand( 0.0f, 1.0f );
            float ang = DegToRad( frand( -m_ConeAngle - m_ConeAngleD, m_ConeAngle + m_ConeAngleD ) );

		    float m1 = 1.0f - r2 * (1.0f - cosf( ang )); 
		    float m2 = sqrtf( 1.0f - m1*m1 );
		    dir += Vec3( m2*cosf( c_DoublePI * r1 ), m2*sinf( c_DoublePI * r1 ), m1 );
        }
    }
    return dir;
} // PSShot::GetDirection
