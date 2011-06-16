/***********************************************************************************/
//  File:   ColGeom.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ColGeom.h"
#include "PhysObject.h"
#include "PhysicsServer.h"
#include "OdeUtil.h"
#include "JModel.h"
#include "IModelServer.h"

/***********************************************************************************/
/*  ColGeom implementation
/***********************************************************************************/
decl_class(ColGeom);
ColGeom::ColGeom()
{
    m_ID            = 0;           
    m_pBody         = NULL;
    m_pMaterial     = NULL;
    m_BoneID        = -1;
    m_TM            = Mat4::identity;
    m_OffsetTM      = Mat4::identity;
} // ColGeom::ColGeom

ColGeom::~ColGeom()
{
    PhysicsServer* pServer = PhysicsServer::s_pInstance;
    if (pServer && pServer->IsInited() && m_ID)
    {   
        dGeomSetBody( m_ID, NULL );
        dGeomDestroy( m_ID );
    }
    m_ID = NULL;
} // ColGeom::~ColGeom

void ColGeom::BindToSolver()
{
    m_ID = CreateGeom( PhysicsServer::s_pInstance->GetSpaceID( this ) );
    if (m_ID == NULL) return;

    if (m_pBody)
    {
        dGeomSetBody( m_ID, m_pBody->GetBodyID() );
    }

    //  assign custom geom data to point at parent PhysObject
    JObject* pParent = GetParent();
    PhysObject* pobj = NULL;
    while (pParent)
    {
        if (!pobj) pobj = obj_cast<PhysObject>( pParent );
        pParent = pParent->GetParent();
    }
    dGeomSetData( m_ID, pobj );
} // ColGeom::BindToSolver

void ColGeom::Init()
{
    if (m_ID) return;
    if (!m_pBody && m_BoneID == -1 && m_HostName.size() > 0)
    {
        //  find body to attach to
        m_pBody = g_pObjectServer->FindObject<PhysBody>( m_HostName.c_str(), this );
        if (m_pBody == NULL)
        {
            //  find host bone
            JModel* pModel = g_pModelServer->GetModelByInst( GetModelInstID() );
            if (pModel)
            {
                m_BoneID = g_pModelServer->GetModelBoneID( pModel->GetModelID(), m_HostName.c_str() );
                g_pModelServer->GetBoneTM( GetModelInstID(), m_BoneID, m_TM );
            }
        }
    }

    if (!m_pMaterial)
    {
        m_pMaterial = g_pObjectServer->FindObject<PhysMaterial>( m_Material.c_str(), this );
        if (!m_pMaterial)
        {
            m_pMaterial = PhysicsServer::s_pInstance->GetDefaultMaterial();
        }
    }
} // ColGeom::Init

void ColGeom::Synchronize( bool bFromSolver )
{
    if (m_ID == 0) return;

    float scale = PhysicsServer::s_pInstance->GetWorldScale();

    if (bFromSolver)
    {
        //  body enabled status
        SetEnabled( dGeomIsEnabled( m_ID ) == 1 );
        
        //  get transform
        if (m_pBody)
        {
            m_TM = m_OffsetTM*m_pBody->GetTM();
        }
        else
        {
            g_pModelServer->GetBoneTM( GetModelInstID(), m_BoneID, m_TM );
        }
    }
    else
    {
        if (IsEnabled())
        {
            dGeomEnable( m_ID );
        }
        else
        {
            dGeomDisable( m_ID );
        }

        if (IsPlaceable())
        {
            //  set geom transform
            Mat3 rotTM;
            Vec3 pos;
            Vec3 scale;
            m_OffsetTM.factorize( scale, rotTM, pos );
            pos *= scale;
            
            dMatrix3 rotD;
            Convert( rotD, rotTM );

            dGeomSetOffsetPosition( m_ID, pos.x, pos.y, pos.z );
            dGeomSetOffsetRotation( m_ID, rotD );

            if (m_pBody)
            {
                dMass m = GetMass();
                if (!pos.is_equal( Vec3::null ))
                {
                    dMassTranslate( &m, pos.x, pos.y, pos.z );
                }
                if (!rotTM.is_equal( Mat3::identity ))
                {
                    dMassRotate( &m, rotD );
                }

                m_pBody->AccumulateMass( m );
            }
        }
    }
} // ColGeom::Synchronize


