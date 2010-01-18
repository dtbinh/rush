/***********************************************************************************/
//  File:   JModelInstance.cpp
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "jobjectiterator.h"
#include "janimation.h"
#include "jcamera.h"
#include "jmodel.h"
#include "jmodelinstance.h"
#include "imodelserver.h"

/***********************************************************************************/
/*  JModelInstance implementation
/***********************************************************************************/
decl_class(JModelInstance)

JModelInstance::JModelInstance()
{
    m_Model             = "";
    m_Scale             = 1.0f;
    m_Position          = Vec3::null;
    m_Rotation          = Vec3::null;
    m_ModelID           = -1;
    m_bHasShadow        = false;
    m_InstanceID        = -1;
    m_Color             = 0xFFFFFFFF;
    m_AttachInstance    = "";
    m_AttachBone        = "";
    m_bCameraPlacement  = true;
    m_AttachInstanceID  = -1;
    m_AttachBoneID      = -1;
    m_ZBias             = 0.0f;
    m_PreTM             = Mat4::identity;
    m_bUseCamera        = false;
} 

void JModelInstance::SetModel( const char* model ) 
{ 
    m_Model         = model; 
    m_ModelID       = -1;
    m_InstanceID    = -1;
}

float JModelInstance::GetTransparency() const
{
    uint32_t transp = (m_Color&0xFF000000) >> 24;
    float res = float( transp )/255.0f;
    return res;
}

void JModelInstance::SetTransparency( float val )
{
    uint32_t transp = uint32_t( val*255.0f );
    m_Color = (m_Color&0x00FFFFFF)|(transp << 24);
}

void JModelInstance::SetAttachInstance( const char* name )
{
    m_AttachInstance    = name;
    m_AttachBoneID      = -1;
    m_AttachInstanceID  = -1;
}

void JModelInstance::SetAttachBone( const char* name )
{
    m_AttachBone    = name;
    m_AttachBoneID  = -1;
}

void JModelInstance::ResInit()
{
    m_ModelID    = g_pModelServer->GetModelID( m_Model.c_str() );
    if (m_InstanceID == -1)
    {
        m_InstanceID = g_pModelServer->InstanceModel( m_ModelID );
    }
}  

void JModelInstance::DrawBounds()
{
    Mat4 tm;
    GetWorldTM( tm );
    
    Vec3 center = tm.translation();
    Vec3 right( tm.e00, tm.e01, tm.e02 );
    Vec3 up   ( tm.e10, tm.e11, tm.e12 );
    Vec3 dir  ( tm.e20, tm.e21, tm.e22 );

    const float c_HandleLen = 1.0f;
    g_pDrawServer->SetZEnable( false );
    g_pDrawServer->DrawLine( center, center + right*c_HandleLen, 0xFFFF0000, 0xFFFF0000 );
    g_pDrawServer->DrawLine( center, center + up*c_HandleLen,    0xFF00FF00, 0xFF00FF00 );
    g_pDrawServer->DrawLine( center, center + dir*c_HandleLen,   0xFF0000FF, 0xFF0000FF );
    
    g_pDrawServer->Flush();
}  

void JModelInstance::GetWorldTM( Mat4& tm ) const
{
    const JAnmContext& ctx = JAnimation::GetContext();

    Mat3 rotTM;
    rotTM.from_euler( DegToRad( m_Rotation.x ), DegToRad( m_Rotation.y ), DegToRad( m_Rotation.z ) );
    tm.srt( m_Scale, rotTM, m_Position );
    tm = tm*ctx.m_TM;
}

void JModelInstance::Render() 
{
    if (m_ModelID == -1) 
    {
        ResInit();
    }
    
    if (m_ModelID == -1) 
    {
        return;
    }

    //  modify transform by "attach-to" bone (FIXME)
    if (m_AttachInstance.size() > 0)
    {
        int boneID = -1; 
        Mat4 boneTM = Mat4::identity;
        if (m_AttachInstance.size() > 0)
        {
            JModelInstance* pModelInst = g_pObjectServer->FindObject<JModelInstance>( m_AttachInstance.c_str(), this );
            if (pModelInst)
            {
                m_AttachInstanceID = pModelInst->GetInstanceID();
                m_AttachBoneID = g_pModelServer->GetModelBoneID( pModelInst->GetModelID(), m_AttachBone.c_str() );
                g_pModelServer->GetBoneTM( m_AttachInstanceID, m_AttachBoneID, boneTM );
            }
        }

        Mat3 rot;
        Vec3 scale;
        boneTM.factorize( scale, rot, m_Position );
        //m_Scale = scale.x;
        m_Rotation = rot.to_euler();
        m_Rotation.x = RadToDeg( m_Rotation.x );
        m_Rotation.y = RadToDeg( m_Rotation.y );
        m_Rotation.z = RadToDeg( m_Rotation.z );
    }
    
    Mat4 curViewTM = g_pRenderServer->GetViewTM();
    Mat4 curProjTM = g_pRenderServer->GetProjTM();

    JModel* pModel = g_pModelServer->GetModel( m_ModelID );
    bool bHasCamera = false;
    if ((m_bCameraPlacement || m_bUseCamera)&& pModel)
    {
        Mat4 camViewTM;
        Mat4 camProjTM;
        JObjectIterator it( pModel );
        while (it)
        {
            JCamera* pCam = obj_cast<JCamera>( *it );
            if (pCam)
            {
                camViewTM = pCam->GetViewTM();
                camProjTM = pCam->GetProjTM();
                bHasCamera = true;
                break;
            }
            ++it;
        }
        if (bHasCamera)
        {
            float fovx, zn, zf, aspect;
            FactorPerspProjTM( camProjTM, fovx, zn, zf, aspect );
            zn += m_ZBias;
            zn = clamp( zn, 0.00001f, zf - 0.00001f );
            PerspProjTM( camProjTM, fovx, zn, zf, aspect );
            g_pRenderServer->SetViewTM( camViewTM );
            g_pRenderServer->SetProjTM( camProjTM );
        }
    }

    Mat4 tm;
    GetWorldTM( tm );
    g_pRenderServer->SetColorTint( m_Color );
    g_pModelServer->DrawModel( m_ModelID, tm, m_InstanceID );

    if (m_bHasShadow)
    {
        g_pModelServer->DrawShadow( m_InstanceID, tm );
    }

    if (bHasCamera && !m_bUseCamera)
    {
        g_pRenderServer->SetViewTM( curViewTM );
        g_pRenderServer->SetProjTM( curProjTM );
    }
} 

