/***********************************************************************************/
//  File:   physics.cpp
//  Desc:   Physics-related exporter part
//  Date:   26.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JGroup.h"
#include "JCamera.h"
#include "JModel.h"
#include "JModelAnim.h"
#include "ExportConfig.h"
#include "rmax.h"

void RBExport::ProcessCamera( INode* node )
{
    if (m_pConfig->m_bExportCameras == false)
    {
        return;
    }

    ObjectState os = node->EvalWorldState( m_CurTime );
    Object* pCurObject = os.obj;
    if (!pCurObject) return;

    CameraObject* camObj = (CameraObject*)pCurObject;
    Interval validIvl;
    CameraState cs;

    RefResult res = camObj->EvalCameraState( m_CurTime, validIvl, &cs );
    if ( res != REF_SUCCEED)
    {
        Err( "Error processing camera <%s>", node->GetName() );
        return;
    }

    Info( "Processing camera <%s>", node->GetName() );

    Matrix3 camTM = c_CamFlipTM*node->GetNodeTM( m_CurTime );

    Matrix3 parentTM;
    INode* pParent = node->GetParentNode();
    if (pParent && !pParent->IsRootNode())
    {
        parentTM = pParent->GetNodeTM( m_CurTime );
    }
    else
    {
        parentTM = c_IdentityTM;
    }
    parentTM = c_FlipTM*parentTM;
    camTM = camTM*Inverse( parentTM );

    JCamera* pCamera = new JCamera();
    pCamera->SetWorldTM( Convert( camTM ) );

    float aspect = ((float)m_pInterface->GetRendWidth())/((float)m_pInterface->GetRendHeight());
    float zn     = cs.nearRange;
    float zf     = cs.farRange;
    float fov    = cs.fov;
    if (zn < c_FltEpsilon) zn = 1.0f;

    pCamera->SetOrtho  ( cs.isOrtho == TRUE );
    pCamera->SetZNear  ( zn );
    pCamera->SetZFar   ( zf );
    pCamera->SetAspect ( aspect ); 

    if (cs.isOrtho)
    {
        float vVol = 2.0f * cs.tdist * tan( cs.fov * 0.5f );
        pCamera->SetFOVx( vVol );
    }
    else
    {
        pCamera->SetFOVx( RadToDeg( fov ) );
    }

    pCamera->SetName( node->GetName() );

    //  insert camera as the first node in the model
    m_pModel->AddChild( pCamera, 0 );
} // RBExport::ProcessCamera


