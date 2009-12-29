/***********************************************************************************/
//  File:   PhysBody.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "PhysBody.h"
#include "PhysicsServer.h"
#include "OdeUtil.h"
#include "JModel.h"
#include "IModelServer.h"

/***********************************************************************************/
/*  PhysBody implementation
/***********************************************************************************/
decl_class(PhysBody);
PhysBody::PhysBody()
{
    m_Velocity      = Vec3::null;	
    m_AngVelocity   = Vec3::null;
    m_Mass          = 1.0f;		
    m_ID            = 0;
    m_Inertia       = Mat3::identity;
    m_TM            = Mat4::identity;
    m_OffsetTM      = Mat4::identity;
    m_bAutoDisable  = true;

    m_Force         = Vec3::null;	
    m_Torque        = Vec3::null;

    m_BodyScale     = Vec3::one;

    m_BoneID        = -1;
    m_NMasses       = 0;

} // PhysBody::PhysBody

PhysBody::~PhysBody()
{
    PhysicsServer* pServer = PhysicsServer::s_pInstance;
    if (pServer && pServer->IsInited() && m_ID) 
    {
        dBodyDestroy( m_ID );
    }
    m_ID = 0;
} // PhysBody::~PhysBody

void PhysBody::BindToSolver()
{
    PhysicsServer* pServer = PhysicsServer::s_pInstance;
    if (pServer && pServer->IsInited()) 
    {
        if (m_ID != 0)
        {
            dBodyDestroy( m_ID );
        }
        m_ID = dBodyCreate( pServer->GetWorldID() );
        dBodySetAutoDisableFlag( m_ID, m_bAutoDisable ? 1 : 0 );
    }
} // PhysBody::BindToSolver

void PhysBody::Init()
{
    if (GetModelInstID() != -1 && m_BoneID == -1)
    {
        JModel* pModel = g_pModelServer->GetModelByInst( GetModelInstID() );
        if (pModel)
        {
            m_BoneID = g_pModelServer->GetModelBoneID( pModel->GetModelID(), m_HostBone.c_str() );
        }
    }

    Mat4 tm = Mat4::identity;
    bool bRes = g_pModelServer->GetBoneTM( GetModelInstID(), m_BoneID, tm );
    if (bRes)
    {
        m_TM = tm;
    }

    Mat3 bodyRot;
    Vec3 bodyPos;
    m_TM.factorize( m_BodyScale, bodyRot, bodyPos );
} // PhysBody::Init

void PhysBody::Synchronize( bool bFromSolver )
{
    if (m_ID == 0) return;

    float scale = PhysicsServer::s_pInstance->GetWorldScale();

    if (bFromSolver)
    {
        //  body enabled status
        SetEnabled( dBodyIsEnabled( m_ID ) == 1 );
        
        //  body linear velocity
        const dReal* vel = dBodyGetLinearVel( m_ID );
        m_Velocity.x = vel[0]/scale;
        m_Velocity.y = vel[1]/scale;
        m_Velocity.z = vel[2]/scale;
        
        //  body force
        const dReal* force = dBodyGetForce( m_ID );
        m_Force.x = force[0]/scale;
        m_Force.y = force[1]/scale;
        m_Force.z = force[2]/scale;
        
        //  body torque
        const dReal* torque = dBodyGetTorque( m_ID );
        m_Torque.x = torque[0];
        m_Torque.y = torque[1];
        m_Torque.z = torque[2];

        //  body angular velocity
        const dReal* angvel = dBodyGetAngularVel( m_ID );
        m_AngVelocity.x = angvel[0]*scale;
        m_AngVelocity.y = angvel[1]*scale;
        m_AngVelocity.z = angvel[2]*scale;

        //  body transform
        const dVector3* pos = (const dVector3*)dBodyGetPosition( m_ID );
        Vec3 cpos;
        Convert( cpos, *pos );
        cpos /= scale;

        const dQuaternion* rot = (const dQuaternion*)dBodyGetQuaternion( m_ID );
        Quaternion quat;
        Convert( quat, *rot );
        Mat3 mrot( quat );

        m_TM.srt( m_BodyScale, mrot, cpos );

        //  set transform to the host bone
        g_pModelServer->SetBoneTM( GetModelInstID(), m_BoneID, m_TM );
    }
    else
    {
        if (IsEnabled())
        {
            dBodyEnable( m_ID );
        }
        else
        {
            dBodyDisable( m_ID );
        }

        //  set default body mass/inertia
        m_AccumulatedMass.mass = m_Mass;
        Mat3 inertia = m_Inertia;
        inertia *= scale;
        Convert( m_AccumulatedMass.I, inertia );
        Convert( m_AccumulatedMass.c, Vec4( 0, 0, 0, 1 ) );
        dBodySetMass( m_ID, &m_AccumulatedMass );
        m_NMasses = 0;
        
        //  set body force
        dBodySetForce( m_ID, m_Force.x*scale, m_Force.y*scale, m_Force.z*scale );
        
        //  body torque
        dBodySetTorque( m_ID, m_Torque.x, m_Torque.y, m_Torque.z );

        

        //  body angular velocity
        dBodySetAngularVel( m_ID, m_AngVelocity.x/scale, m_AngVelocity.y/scale, m_AngVelocity.z/scale );
        
        //  set body linear velocity
        dBodySetLinearVel( m_ID, m_Velocity.x*scale, m_Velocity.y*scale, m_Velocity.z*scale );

        // set transform to the solver
        
        //  get body transform from the host bone
        Mat4 tm = Mat4::identity;
        bool bRes = g_pModelServer->GetBoneTM( GetModelInstID(), m_BoneID, tm );
        if (bRes)
        {
            m_TM = tm;
        }

        Mat3 bodyRot;
        Vec3 bodyPos;
        m_TM.factorize( m_BodyScale, bodyRot, bodyPos );
        bodyPos *= scale;
        dQuaternion dq;
        Quaternion  quat( bodyRot );
        quat.normalize();
        Convert( dq, quat );
        dBodySetPosition( m_ID, bodyPos.x, bodyPos.y, bodyPos.z );
        dBodySetQuaternion( m_ID, dq );
    }
} // PhysBody::Synchronize

void PhysBody::DrawBounds()
{
    g_pDrawServer->SetWorldTM( Mat4::identity );
    g_pDrawServer->SetZEnable( false );
    Vec3 pos = m_TM.translation();
    const float c_HandleSize = 0.3f;
    DWORD clr = IsEnabled() ? 0x50FF7777 : 0x50777777;
    g_pDrawServer->DrawSphere( pos, c_HandleSize*0.5f, 0, clr );

    Vec3 dx( m_TM.e00, m_TM.e01, m_TM.e02 );
    Vec3 dy( m_TM.e10, m_TM.e11, m_TM.e12 );
    Vec3 dz( m_TM.e20, m_TM.e21, m_TM.e22 );

    dx.normalize();
    dy.normalize();
    dz.normalize();

    g_pDrawServer->DrawLine( pos, pos + c_HandleSize*dx, 0x77FF0000, 0x77FF0000 );
    g_pDrawServer->DrawLine( pos, pos + c_HandleSize*dy, 0x7700FF00, 0x7700FF00 );
    g_pDrawServer->DrawLine( pos, pos + c_HandleSize*dz, 0x770000FF, 0x770000FF );
} // PhysBody::DrawBounds
