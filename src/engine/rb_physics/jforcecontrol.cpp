/***********************************************************************************/
//  File:   JForceControl.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JForceControl.h"
#include "IPhysicsServer.h"
#include "OdeUtil.h"
#include "JModelInstance.h"

/***********************************************************************************/
/*  JForceControl implementation
/***********************************************************************************/
decl_class(JForceControl);
JForceControl::JForceControl()
{
    m_PhysObjInstID = -1;
    m_Magnitude     = 1.0f;
    m_JumpImpulse   = 10.0f;
} // JForceControl::JForceControl

JForceControl::~JForceControl()
{
   
} // JForceControl::~JForceControl

void JForceControl::Init()
{
}

void JForceControl::Render()
{
    JAnimation::Render();
    
    float dt = GetContext().m_TimeDelta;
    if (dt == 0.0f) return;

    Vec3 force = Vec3::null;
    Mat4 viewTM = g_pRenderServer->GetViewTM();
    Vec3 dir( viewTM.e02, viewTM.e12, viewTM.e22 );
    dir.z = 0.0f;
    dir.normalize();
    Vec3 right( -dir.y, dir.x, 0.0f );
    Vec3 up( 0.0f, 0.0f, 1.0f );
    if (GetKeyState( 'W' ) < 0)
    {
        force += dir;
    }
    if (GetKeyState( 'S' ) < 0)
    {
        force -= dir;
    }
    if (GetKeyState( 'A' ) < 0)
    {
        force -= right;
    }
    if (GetKeyState( 'D' ) < 0)
    {
        force += right;
    }
    if (GetKeyState( ' ' ) < 0)
    {
        force += up*m_JumpImpulse;
    }

    force *= m_Magnitude*dt;

    g_pPhysicsServer->SetObjectForce( m_PhysObjInstID, force );
}

void JForceControl::OnPlay()
{
    if (!m_pModelInst || m_PhysObjInstID == -1)
    {
        m_pModelInst = g_pObjectServer->FindObject<JModelInstance>( m_ModelInstance.c_str(), this );
        if (!m_pModelInst) 
        {
            return;
        }
        m_pModelInst->ResInit();
        int modelInstID = m_pModelInst->GetInstanceID();
        m_PhysObjInstID = g_pPhysicsServer->GetObjectID( modelInstID );
    }
}

void JForceControl::OnStop()
{

}

void JForceControl::DrawBounds()
{
}

