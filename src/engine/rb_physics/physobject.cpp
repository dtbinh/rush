/***********************************************************************************/
//  File:   PhysObject.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "PhysObject.h"
#include "PhysBody.h"
#include "ColGeom.h"
#include "JObjectIterator.h"
#include "PhysicsServer.h"

/***********************************************************************************/
/*  PhysObject implementation
/***********************************************************************************/
decl_class(PhysObject);
PhysObject::~PhysObject()
{
    PhysicsServer* pServer = PhysicsServer::s_pInstance;
    if (pServer)
    {
        pServer->UnregisterObject( this );
    }
}

void PhysObject::Enable( bool bEnable )
{
    if (bEnable == m_bEnabled) return;
    JObjectIterator it( this );
    while (*it) 
    {
        JObject* pObject = (*it);
        PhysBody* pBody = obj_cast<PhysBody>( pObject );
        if (pBody)
        {
            pBody->SetEnabled( bEnable );
        }
        ColGeom* pGeom = obj_cast<ColGeom>( pObject );
        if (pGeom)
        {
            pGeom->SetEnabled( bEnable );        
        }
        ++it;
    }
    m_bEnabled = bEnable;
} // PhysObject::Enable

PhysBody* PhysObject::GetRootBody()
{
    if (m_pRootBody) return m_pRootBody;
    if (m_RootBody.size() > 0)
    {
        m_pRootBody = (PhysBody*)g_pObjectServer->FindObject( m_RootBody.c_str(), this );
        return m_pRootBody;
    }
    JObjectIterator it( this );
    while (it)
    {
        JObject* pObj = *it;
        m_pRootBody = obj_cast<PhysBody>( pObj );
        if (m_pRootBody) return m_pRootBody;
        ++it;
    }   
    return NULL;
} // PhysObject::GetRootBody

void PhysObject::DrawBounds()
{
    JObjectIterator it( this );
    ++it;
    while (it)
    {
        JObject* pObj = *it;
        if (!pObj->IsDrawBounds())
        {
            pObj->DrawBounds();
        }
        ++it;
    }
} // PhysObject::DrawBounds
