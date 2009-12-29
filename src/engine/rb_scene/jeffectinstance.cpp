/***********************************************************************************/
//  File:   JEffectInstance.cpp
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "IParticleServer.h"
#include "IModelServer.h"
#include "JAnimation.h"
#include "JModelInstance.h"
#include "JEffectInstance.h"

/***********************************************************************************/
/*  JEffectInstance implementation
/***********************************************************************************/
decl_class(JEffectInstance)

JEffectInstance::JEffectInstance()
{
    m_Effect            = "";
    m_Scale             = 1.0f;
    m_Alpha             = 1.0f;
    m_Position          = Vec3::null;
    m_Rotation          = Vec3::null;
    m_EffectID          = -1;
    m_InstanceID        = -1;
    m_Stamp             = 0xFFFFFFFF;
    m_PrevTime          = 0.0f;
    m_AttachBoneID      = -1;
    m_AttachInstanceID  = -1;
    m_bScreenSpace      = true;
} // JEffectInstance::JEffectInstance

void JEffectInstance::SetAttachInstance( const char* name )
{
    m_AttachInstance    = name;
    m_AttachBoneID      = -1;
    m_AttachInstanceID  = -1;
}

void JEffectInstance::SetAttachBone( const char* name )
{
    m_AttachBone    = name;
    m_AttachBoneID  = -1;
}

void JEffectInstance::OnPlay()
{
    if (m_EffectID == -1) m_EffectID = g_pParticleServer->GetEffectID( m_Effect.c_str() );
    if (m_EffectID == -1) return;
    if (m_InstanceID != -1) 
    {
        g_pParticleServer->DeleteInstance( m_InstanceID );
    }
    m_InstanceID = g_pParticleServer->CreateInstance( m_EffectID );
    m_Stamp = g_pParticleServer->GetStamp();
    m_PrevTime = GetStartTime();
} // JEffectInstance::OnPlay

void JEffectInstance::OnStop()
{
    if (m_Stamp == g_pParticleServer->GetStamp())
    {
        g_pParticleServer->DeleteInstance( m_InstanceID );
    }
    m_InstanceID = -1;
} // JEffectInstance::OnStop

void JEffectInstance::PostRender()
{
    JAnimation::PostRender();
} // JEffectInstance::PostRender

void JEffectInstance::Init()
{
    m_EffectID = g_pParticleServer->GetEffectID( m_Effect.c_str() );
} // JEffectInstance::Init

void JEffectInstance::Render()
{
    JAnimation::Render();
    if (!IsPlaying())
    {
        return;
    }

    //  modify transform by "attach-to" bone 
    if (m_AttachInstance.size() > 0)
    {
        int boneID = -1; 
        Mat4 boneTM = Mat4::identity;
        if (m_AttachInstance.size() > 0)
        {
            if (m_AttachInstanceID == -1)
            {
                JModelInstance* pModelInst = g_pObjectServer->FindObject<JModelInstance>( m_AttachInstance.c_str(), this );
                if (pModelInst)
                {
                    m_AttachInstanceID = pModelInst->GetInstanceID();
                    m_AttachBoneID = g_pModelServer->GetModelBoneID( pModelInst->GetModelID(), m_AttachBone.c_str() );
                }
            }
            g_pModelServer->GetBoneTM( m_AttachInstanceID, m_AttachBoneID, boneTM );
        }

        Mat3 rot;
        Vec3 scale;
        boneTM.factorize( scale, rot, m_Position );

        if (m_bScreenSpace)
        {
            Mat4 tm; 
            ProjToScreenTM( tm, g_pRenderServer->GetViewport() );
            if (g_pRenderServer)
            {
                tm = g_pRenderServer->GetViewTM()*g_pRenderServer->GetProjTM()*tm;
                tm.tmproj( m_Position );
            }
        }
    }

    float cTime = GetContext().m_Time;
    if (m_Stamp == g_pParticleServer->GetStamp())
    {
        Vec3 pos = m_Position + GetContext().m_Pos;
        if (cTime - m_PrevTime > c_RestartTimeout)
        {
            Stop();
            Play();
            cTime = 0.0f;
            if (GetCurLoop() == 0) 
            {
                cTime += GetStartTime();
            }
            m_PrevTime = cTime;
        }
        if (!GetContext().m_bStateOnly)
        {
            g_pParticleServer->Render( m_InstanceID, cTime, pos, m_Scale, m_Alpha );
        }
        m_PrevTime = cTime;
    }
    else
    {
        Stop();
    }
} // JEffectInstance::Render