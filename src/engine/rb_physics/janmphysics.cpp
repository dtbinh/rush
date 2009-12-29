/***********************************************************************************/
//  File:   JAnmPhysics.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JAnmPhysics.h"
#include "PhysicsServer.h"
#include "OdeUtil.h"
#include "JModelInstance.h"

/***********************************************************************************/
/*  JAnmPhysics implementation
/***********************************************************************************/
decl_class(JAnmPhysics);
JAnmPhysics::JAnmPhysics()
{
    m_PhysObjInstID = -1;
    m_pModelInst    = NULL;
} // JAnmPhysics::JAnmPhysics

JAnmPhysics::~JAnmPhysics()
{
   
} // JAnmPhysics::~JAnmPhysics

void JAnmPhysics::Init()
{
}

void JAnmPhysics::Render()
{
    JAnimation::Render();
}

void JAnmPhysics::OnPlay()
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

void JAnmPhysics::OnStop()
{

}

void JAnmPhysics::DrawBounds()
{
    g_pPhysicsServer->DrawBounds( m_PhysObjInstID );
}

