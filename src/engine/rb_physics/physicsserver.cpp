/***********************************************************************************/
//  File:   PhysicsServer.cpp
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "PhysMaterial.h"
#include "PhysObject.h"
#include "PhysBody.h"
#include "PhysJoint.h"
#include "ColGeom.h"
#include "JModel.h"
#include "AABox.h"
#include "IModelServer.h"
#include "JObjectIterator.h"
#include "JAnimation.h"
#include "PhysicsServer.h"

/***********************************************************************************/
/*  PhysicsServer implementation
/***********************************************************************************/
decl_class(PhysicsServer);
PhysicsServer*  PhysicsServer::s_pInstance = NULL;
IPhysicsServer*       g_pPhysicsServer = NULL;

PhysicsServer::PhysicsServer()
{
    m_ERP		        = 0.9f;
    m_CFM		        = 0.001f;
    m_Gravity           = Vec3( 0.0f, 0.0f, -9.81f );
    m_WorldScale        = 0.01f;
    m_Mode              = smStepFast;
    m_MaxIter	        = 10;
    m_StepSize          = 0.01f;
    m_TimeRepr          = 0.0f;
    m_bDrawVolumes      = false;
    m_bInited           = false;
    m_WorldID           = 0;
    m_DefaultSpaceID    = 0;
    m_ContactGroupID    = 0;

    m_GeomColor         = 0x207102a0;   
    m_GeomLinesColor    = 0x227102a0;   

    SetName( "physics" );

    g_pPhysicsServer  = this;
    s_pInstance = this;

} // PhysicsServer::PhysicsServer

PhysicsServer::~PhysicsServer()
{
    RemoveChildren();
    if (m_WorldID) 
    {
        dJointGroupDestroy( m_ContactGroupID );
        dSpaceDestroy( m_DefaultSpaceID );
        dWorldDestroy( m_WorldID );
        dCloseODE();
    }
    m_bInited = false;

    if (s_pInstance == this)
    {
        s_pInstance = NULL;
    }
} // PhysicsServer::~PhysicsServer

void PhysicsServer::Init()
{
    if (m_bInited)
    {
        return;
    }
    RemoveChildren();
    m_Objects.clear();

    dInitODE();

    m_WorldID           = dWorldCreate();
    m_DefaultSpaceID    = dHashSpaceCreate( NULL );
    m_ContactGroupID    = dJointGroupCreate( 0 );

    dWorldSetAutoDisableFlag( m_WorldID, 1 );
    dWorldSetAutoDisableAverageSamplesCount( m_WorldID, 1 );

    if (m_WorldID)
    {
        SetERP( m_ERP );
        SetCFM( m_CFM );
        SetGravity( m_Gravity );
    }

    m_pDefMaterial = g_pObjectServer->FindObject<PhysMaterial>( "defaullt_mtl", this );
    if (!m_pDefMaterial) 
    {
        m_pDefMaterial = new PhysMaterial();
        m_pDefMaterial->SetName( "defaullt_mtl" );
        AddChild( m_pDefMaterial );
    }

    m_bInited = true;
} // PhysicsServer::Init

PhysMaterial* PhysicsServer::GetGeomSurface( dGeomID geomID )
{
    return m_pDefMaterial;
} // PhysicsServer::GetGeomSurface

void PhysicsServer::SetCFM( float val )
{
    m_CFM = val;
    if (m_WorldID) dWorldSetCFM( m_WorldID, val );
} // PhysicsServer::SetCFM

void PhysicsServer::SetERP( float val )
{
    m_ERP = val;
    if (m_WorldID) dWorldSetERP( m_WorldID, val );
} // PhysicsServer::SetERP

void PhysicsServer::SetGravity( Vec3 g )
{
    m_Gravity = g;
    Vec3 gr( g );
    gr *= m_WorldScale;
    if (m_WorldID) dWorldSetGravity( m_WorldID, gr.x, gr.y, gr.z );
} // PhysicsServer::SetGravity

int PhysicsServer::GetNBodies() const
{
    return -1;
} // PhysicsServer::GetNBodies

int PhysicsServer::GetNGeoms() const
{
    return -1;
} // PhysicsServer::GetNGeoms

void PhysicsServer::ProcessCollision( void *data, dGeomID o1, dGeomID o2 )
{
    //  skip connected bodies
    dBodyID bodyID1 = dGeomGetBody( o1 );
    dBodyID bodyID2 = dGeomGetBody( o2 );
    if (bodyID1 && bodyID2 && dAreConnected( bodyID1, bodyID2 )) return;
    if (!bodyID1 && !bodyID2) return;

    //  do collision
    int nContacts = dCollide( o1, o2, c_MaxContacts, &m_Contacts[0].geom, sizeof( dContact ) );
    if (nContacts == 0) return;

    float scale = GetWorldScale();

    nContacts = tmin( nContacts, c_MaxContacts - m_NContacts );
    for (int i = 0; i < nContacts; i++) 
    {
        PhysMaterial* pSurf1 = GetGeomSurface( o1 );
        PhysMaterial* pSurf2 = GetGeomSurface( o2 );
        dContact& contact = m_Contacts[i];
        dSurfaceParameters& surface = contact.surface;
        
        //  TODO: proper flags setup
        m_Contacts[i].surface.mode = 
            //dContactApprox1 | 
            //dContactFDir1   | 
            dContactSoftERP | 
            dContactSoftCFM |
            //dContactSlip1   | 
            //dContactSlip2   | 
            dContactBounce;

        if (!pSurf1 && !pSurf2)
        {
            surface.mu			= 8.0f;
            surface.mu2			= 8.0f;
            surface.bounce		= 1.0f; 
            surface.bounce_vel	= 0.0f;
            surface.soft_erp	= 0.1f; 
            surface.soft_cfm	= 1e-3f;
            surface.motion1		= 0.0f; 
            surface.motion2		= 0.0f; 
            surface.slip1		= 0.1f;
            surface.slip2		= 0.1f;
        }
        else if (pSurf2 == pSurf1)
        {
            surface.mu			= pSurf1->m_Mu; 
            surface.mu2			= pSurf1->m_Mu2;
            surface.bounce		= pSurf1->m_Bounce;
            surface.bounce_vel	= pSurf1->m_BounceVel;
            surface.soft_erp	= pSurf1->m_SoftERP; 
            surface.soft_cfm	= pSurf1->m_SoftCFM;
            surface.motion1		= pSurf1->m_Motion1;
            surface.motion2		= pSurf1->m_Motion2; 
            surface.slip1		= pSurf1->m_Slip1;
            surface.slip2		= pSurf1->m_Slip2;
        }
        else
        {
            if (!pSurf1) pSurf1 = pSurf2;
            if (!pSurf2) pSurf2 = pSurf1;

            surface.mu			= sqrtf( pSurf1->m_Mu * pSurf2->m_Mu );
            surface.mu2			= sqrtf( pSurf1->m_Mu2 * pSurf2->m_Mu2 );
            surface.bounce		= 0.5f*( pSurf1->m_Bounce + pSurf2->m_Bounce );
            surface.bounce_vel	= tmin( pSurf1->m_BounceVel, pSurf2->m_BounceVel );
            surface.soft_erp	= sqrtf( pSurf1->m_SoftERP * pSurf2->m_SoftERP ); 
            surface.soft_cfm	= 0.5f*( pSurf1->m_SoftCFM + pSurf2->m_SoftCFM );
            surface.motion1		= sqrtf( pSurf1->m_Motion1 * pSurf2->m_Motion1 );
            surface.motion2		= sqrtf( pSurf1->m_Motion2 * pSurf2->m_Motion2 ); 
            surface.slip1		= sqrtf( pSurf1->m_Slip1 * pSurf2->m_Slip1 );
            surface.slip2		= sqrtf( pSurf1->m_Slip2 * pSurf2->m_Slip2 );
        }

        PhysObject* pObj1 = (PhysObject*)dGeomGetData( o1 );
        PhysObject* pObj2 = (PhysObject*)dGeomGetData( o2 );
        int obj1 = pObj1 ? pObj1->GetID() : -1;
        int obj2 = pObj2 ? pObj2->GetID() : -1;

        dContactGeom& geom = contact.geom;
        m_NContacts++;

        if (IsDrawBounds())
        {
            Vec3 pos = Vec3( geom.pos[0], geom.pos[1], geom.pos[2] ); 
            pos /= scale;
            Vec3 norm = Vec3( geom.normal[0], geom.normal[1], geom.normal[2] ); 
            const float c_HandleSize = 0.3f;
            g_pDrawServer->SetWorldTM( Mat4::identity );
            g_pDrawServer->DrawBox( AABox( pos, c_HandleSize*0.5f ), 0x5500FF00, 0x22FFFF00 );
            g_pDrawServer->DrawLine( pos, pos + norm*c_HandleSize, 0x5500FF00, 0x5500FF00 );
        }
        
        dJointID jID = dJointCreateContact( m_WorldID, m_ContactGroupID, &contact );
        dJointAttach( jID, bodyID1, bodyID2 ); 
    }
} // PhysicsServer::ProcessCollision

void OnCollide( void *data, dGeomID o1, dGeomID o2 )
{
    PhysicsServer::s_pInstance->ProcessCollision( data, o1, o2 );
} // OnCollide

void PhysicsServer::StepSolver( float dt )
{
    // process collision
    dJointGroupEmpty( m_ContactGroupID );
    m_NContacts = 0;
    dSpaceCollide( m_DefaultSpaceID, this, OnCollide );
    
    //  perform integration
    if (m_Mode == smStepFast)
    {
        dWorldStepFast1( m_WorldID, dt, m_MaxIter );
    }
    else if (m_Mode == smNormal)
    {
        dWorldStep( m_WorldID, dt );
    }
    else if (m_Mode == smQuickStep)
    {
        dWorldQuickStep( m_WorldID, dt );
    }
} // PhysicsServer::StepSolver

void PhysicsServer::Render()
{
    if (m_WorldID == 0)
    {
        return;
    }
    //  synchronize solver with physics objects 
    JObjectIterator it( this );
    ++it;
    while (it)
    {
        PhysEntity* pObj = obj_cast<PhysEntity>( *it );
        pObj->Synchronize( false );
        ++it;
    }

    float timePassed = JAnimation::GetContext().m_TimeDelta + m_TimeRepr;
    m_NStepsPerFrame = 0;
    while (timePassed > 0)
    {
        StepSolver( m_StepSize );
        timePassed -= m_StepSize;
        m_NStepsPerFrame++;
    }
    m_TimeRepr = timePassed;
    
    //  synchronize physics objects with solver
    JObjectIterator fit( this );
    ++fit;
    while (fit)
    {
        assert( obj_cast<PhysEntity>( *fit ) );
        PhysEntity* pObj = static_cast<PhysEntity*>( *fit );
        pObj->Synchronize( true );
        ++fit;
    }


    dReal cfm = dWorldGetCFM( m_WorldID );
    dReal erp = dWorldGetERP( m_WorldID );
    
    dVector3 gr;
    dWorldGetGravity( m_WorldID, gr );

    SendSignals();
} // PhysicsServer::Update

int PhysicsServer::GetObjectID( const char* name, JObject* context )
{
    int nObj = m_Objects.size();
    for (int i = 0; i < nObj; i++)
    {
        JObject* pObject = m_Objects[i];
        if (!pObject) continue;
        if (!stricmp( name, pObject->GetName() ) && 
            (!context || pObject->GetParent() == context )) return i;    
    }

    if (context == NULL) 
    {
        context = this;
    }

    PhysObject* pObj = g_pObjectServer->FindObject<PhysObject>( name, context );
    if (pObj)
    {
        m_Objects.push_back( pObj );
        int id = m_Objects.size() - 1;
        pObj->SetID( id );
        return id;
    }

    return -1;
} // PhysicsServer::GetObjectID

int PhysicsServer::GetObjectID( int modelInstID )
{
    int nObj = m_Objects.size();
    for (int i = 0; i < nObj; i++)
    {
        PhysObject* pObject = (PhysObject*)( m_Objects[i] );
        if (!pObject) continue;
        if (pObject->GetModelInstID() == modelInstID) return i;    
    }
    
    JModel* pModel = g_pModelServer->GetModelByInst( modelInstID );
    if (!pModel) return -1;

    PhysObject* pObj = new PhysObject();
    pObj->SetModelInstID( modelInstID );

    //  clone model's physics entities
    JObjectIterator it( (JObject*)pModel );
    while (it)
    {
        const PhysEntity* pCh = obj_cast<PhysEntity>( *it );
        if (pCh)
        {
            PhysEntity* pClonedCh = (PhysEntity*)g_pObjectServer->CloneObject( pCh, pObj, pCh->GetName(), false );
            pClonedCh->SetModelInstID( modelInstID );
            pClonedCh->InitTree();
            pClonedCh->BindToSolver();
            pObj->AddChild( pClonedCh );
        }
        ++it;
    }

    AddChild( pObj );

    int id = m_Objects.size();
    JString objName = pModel->GetName();
    objName += "_physobj";
    JString objID;
    cast( objID, id );
    objName += objID;
    pObj->SetName( objName.c_str() );

    m_Objects.push_back( pObj );
    pObj->SetID( id );
    return id;
} // PhysicsServer::GetObjectID

void PhysicsServer::UnregisterObject( PhysObject* pObject )
{
    int nObj = m_Objects.size();
    for (int i = 0; i < nObj; i++)
    {
        if (m_Objects[i] == pObject) 
        {
            m_Objects[i] = NULL;
        }
    }
} // PhysicsServer::UnregisterObject

int PhysicsServer::GetPhysMaterialID( const char* name )
{
    return -1;
}

const Mat4& PhysicsServer::GetObjectTM( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return Mat4::identity;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return Mat4::identity;
    return pBody->GetTM();
} // PhysicsServer::GetObjectTM

void PhysicsServer::SetObjectTM( int objID, const Mat4& tm )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return;
    pBody->SetTM( tm );
} // PhysicsServer::SetObjectTM

Vec3 PhysicsServer::GetObjectPos( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return Vec3::null;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return Vec3::null;
    return pBody->GetPos();
} // PhysicsServer::GetObjectPos

void PhysicsServer::SetObjectPos( int objID, const Vec3& pos )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return;
    pBody->SetPos( pos );
} // PhysicsServer::SetObjectPos

Vec3 PhysicsServer::GetObjectVel( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return Vec3::null;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return Vec3::null;
    return pBody->GetVelocity();
}

void PhysicsServer::SetObjectVel( int objID, const Vec3& v )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return;
    pBody->SetVelocity( v );
}

float PhysicsServer::GetObjectMass( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return 0.0f;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return 0.0f;
    return pBody->GetMass();  
} // PhysicsServer::GetObjectMass

void PhysicsServer::SetObjectMass( int objID, float mass )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return;
    pBody->SetMass( mass );  
} // PhysicsServer::SetObjectMass

Mat3 PhysicsServer::GetObjectInertia( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return Mat3::identity;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return Mat3::identity;
    return pBody->GetInertia();  
} // PhysicsServer::GetObjectInertia

void PhysicsServer::SetObjectInertia( int objID, const Mat3& inertiaTensor )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return;
    pBody->SetInertia( inertiaTensor );  
} // PhysicsServer::SetObjectInertia

Vec3 PhysicsServer::GetObjectAngVel( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return Vec3::null;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return Vec3::null;
    return pBody->GetAngVelocity();
}

void PhysicsServer::SetObjectAngVel( int objID, const Vec3& v )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return;
    pBody->SetAngVelocity( v );
}

Vec3 PhysicsServer::GetObjectForce( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return Vec3::null;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return Vec3::null;
    return pBody->GetForce();
}

void PhysicsServer::SetObjectForce( int objID, const Vec3& v )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return;
    pBody->SetForce( v );
}

Vec3 PhysicsServer::GetObjectTorque( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return Vec3::null;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return Vec3::null;
    return pBody->GetTorque();
}

void PhysicsServer::SetObjectTorque( int objID, const Vec3& v )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysBody* pBody = m_Objects[objID]->GetRootBody();
    if (!pBody) return;
    pBody->SetTorque( v );
}

void PhysicsServer::EnableObject( int objID, bool bEnable )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    m_Objects[objID]->Enable( bEnable );
} // PhysicsServer::EnableObject

void PhysicsServer::DrawBounds( int objID )
{
    if (objID < 0 || objID >= m_Objects.size()) return;
    PhysObject* pObj = m_Objects[objID];
    pObj->DrawBounds();
} // PhysicsServer::DrawBounds


