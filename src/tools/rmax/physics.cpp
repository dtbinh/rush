/***********************************************************************************/
//  File:   physics.cpp
//  Desc:   Physics-related exporter part
//  Date:   26.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JGroup.h"
#include "JModel.h"
#include "JModelAnim.h"
#include "ExportConfig.h"
#include "rmax.h"

#include "ColBox.h"
#include "ColSphere.h"
#include "ColCapsule.h"
#include "ColCylinder.h"
#include "ColMesh.h"
#include "PhysJoint.h"
#include "PhysBody.h"

//  capsule parameter g_pDrawServer
#define PB_RADIUS   0
#define PB_HEIGHT   1
#define PB_CENTERS  2

JObject* RBExport::ProcessPhysicsObject( INode* node )
{
    ObjectState os = node->EvalWorldState( m_CurTime ); 
    Object* pObj = os.obj;
    const char* pNodeName = node->GetName();

    if (!pObj)
    {
        Warn( "Physics node %s is invalid.", pNodeName );
    }

    JObject* pPhysObj = NULL;
    Mat4 objTM = Convert( GetLocalTM( node, m_CurTime ) );

    JString hostName = "";

    ExpNode* pExportedParent = GetExportedNode( node->GetParentNode() );
    if (pExportedParent && pExportedParent->m_pObject)
    {
        JObject* pParentObj = pExportedParent->m_pObject;
        if (obj_cast<PhysBody>( pParentObj ))
        {
            hostName = pParentObj->GetName();
        }
        else if (obj_cast<JBone>( pParentObj ))
        {
            hostName = pParentObj->GetName();
        }
    }

    //  check for joint type
    DWORD scID = pObj->SuperClassID();
    if (scID == HELPER_CLASS_ID) 
    {
        PhysJoint* pJoint = new PhysJoint(); 
        pJoint->SetHostBone( hostName.c_str() );
        pPhysObj = pJoint;
    }
    else
    {
        CStr className;
        pObj->GetClassName( className );
        IParamArray* pParam = pObj->GetParamBlock();
        IParamBlock* pParamBlock = NULL;

        if (pParam) pParamBlock = pParam->GetParamBlock();

        if (!stricmp( className, "sphere" ))
        {
            ColSphere* pSphere = new ColSphere();
            pPhysObj = pSphere;
            float radius = 0.0f;
            pParamBlock->GetValue( SPHERE_RADIUS, 0, radius, FOREVER );
            pSphere->SetRadius( radius );
            pSphere->SetHost( hostName.c_str() );
            pSphere->SetOffsetTM( objTM );
        }

        if (!stricmp( className, "box" ))
        {
            ColBox* pBox = new ColBox();
            pPhysObj = pBox;
            Vec3 ext;
            pParamBlock->GetValue( BOXOBJ_LENGTH, 0, ext.x, FOREVER );
            pParamBlock->GetValue( BOXOBJ_WIDTH,  0, ext.y, FOREVER );
            pParamBlock->GetValue( BOXOBJ_HEIGHT, 0, ext.z, FOREVER );
            pBox->SetExtents( ext );
            pBox->SetHost( hostName.c_str() );
            pBox->SetOffsetTM( objTM );
        }
        
        if (!stricmp( className, "capsule" ))
        {
            ColCapsule* pCapsule = new ColCapsule();
            pPhysObj = pCapsule;
            int   bCenters = 0;
            float radius   = 0.0f;
            float height   = 0.0f;
            pParamBlock->GetValue( PB_RADIUS,  0, radius,   FOREVER );
            pParamBlock->GetValue( PB_HEIGHT,  0, height,   FOREVER );
            pParamBlock->GetValue( PB_CENTERS, 0, bCenters, FOREVER );
            pCapsule->SetRadius( radius );
            pCapsule->SetHeight( height );
            pCapsule->SetHost( hostName.c_str() );
            pCapsule->SetOffsetTM( objTM );
        }

        if (!stricmp( className, "cylinder" ))
        {
            ColCylinder* pCylinder = new ColCylinder();
            pPhysObj = pCylinder;
            int   bCenters = 0;
            float radius   = 0.0f;
            float height   = 0.0f;
            pParamBlock->GetValue( PB_RADIUS,  0, radius,   FOREVER );
            pParamBlock->GetValue( PB_HEIGHT,  0, height,   FOREVER );
            pParamBlock->GetValue( PB_CENTERS, 0, bCenters, FOREVER );
            pCylinder->SetRadius( radius );
            pCylinder->SetHeight( height );
            pCylinder->SetHost( hostName.c_str() );
            pCylinder->SetOffsetTM( objTM );
        }

        if (!stricmp( className, "teapot" ))
        {
            //  by teapot we represent physical body (point of mass)
            PhysBody* pBody = new PhysBody();
            pBody->SetHostBone( hostName.c_str() );
            pPhysObj = pBody;
            pBody->SetOffsetTM( objTM );
        }

        //  if none of above,  try to export as trimesh collision primitive
        if (pPhysObj == NULL && scID == GEOMOBJECT_CLASS_ID)
        {
            ColMesh* pMesh = GetColMesh( node );
            if (pMesh)
            {
                pPhysObj = pMesh;
                pMesh->SetHost( hostName.c_str() );
                pMesh->SetOffsetTM( objTM );
            }
        }
    }
    
    if (pPhysObj)
    {
        if (!m_pPhysicsGroup)
        {
            m_pPhysicsGroup = new JGroup();
            m_pPhysicsGroup->SetName( "physics" );
        }
        pPhysObj->SetName( pNodeName );
        m_pPhysicsGroup->AddChild( pPhysObj );
    }
    else
    {
        Warn( "Unrecognized physics object type in  node %s", pNodeName );
        return NULL;
    }
    return pPhysObj;
} // RBExport::ProcessPhysicsObject

ColMesh* RBExport::GetColMesh( INode* node )
{
    ObjectState os      = node->EvalWorldState( m_CurTime );
    Object*     pObject = os.obj;
    if (!pObject) 
    {
        Warn( "Could not evaluate object state. Collision mesh %s was skipped.", node->GetName() );
        return NULL;
    }

    Matrix3 nodeTM          = node->GetNodeTM( m_CurTime );
    Matrix3 nodeTMAfterWSM  = node->GetObjTMAfterWSM( m_CurTime );
    Matrix3 mOffs           = nodeTMAfterWSM*Inverse( nodeTM );

    //  triangulate
    TriObject* pTriObj = NULL;
    if (pObject->CanConvertToType( Class_ID( TRIOBJ_CLASS_ID, 0 ) )) 
    { 
        pTriObj = (TriObject*)pObject->ConvertToType( m_CurTime, Class_ID( TRIOBJ_CLASS_ID, 0 ) );
    }
    bool bReleaseTriObj = (pTriObj != pObject);
    if (!pTriObj) 
    {
        Warn( "Could not triangulate mesh in node. Collision mesh %s was skipped.", node->GetName() );
        return NULL;
    }

    //  ensure, that vertex winding direction in polygon is CCW
    Matrix3 objTM = node->GetObjTMAfterWSM( m_CurTime );
    bool bNegScale = (DotProd( CrossProd( objTM.GetRow(0), objTM.GetRow(1) ), objTM.GetRow(2) ) >= 0.0);

    int vx[3];
    vx[0] = bNegScale ? 2 : 0;
    vx[1] = bNegScale ? 1 : 1;
    vx[2] = bNegScale ? 0 : 2;

    Mesh& mesh = pTriObj->GetMesh();

    //  some cosmetics
    mesh.RemoveDegenerateFaces();
    mesh.RemoveIllegalFaces();
    
    //  create collision mesh
    ColMesh* pMesh = new ColMesh();
    
    int numPri  = mesh.getNumFaces();
    int numVert = mesh.numVerts;

    //  copy vertices
    for (int i = 0; i < numVert; i++)
    {
        Point3 pt   = mesh.verts[i];
        pt = mOffs.PointTransform( pt );
        pt = c_FlipTM.PointTransform( pt );
        pMesh->AddVertex( Vec3( pt.x, pt.y, pt.z ) );
    }

    //  loop on mesh faces
    for (int i = 0; i < numPri; i++)
    {
        Face& face = mesh.faces[i];
        pMesh->AddPoly( face.v[vx[0]], face.v[vx[1]], face.v[vx[2]] );
    }

    Msg( LogType_Stats, "Physics collision trimesh has %d vertices and %d faces.", numVert, numPri );

    return pMesh;
} // RBExport::GetColMesh



