/***********************************************************************************/
//  File:   JAnimInstance.cpp
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JAnimation.h"
#include "JModelInstance.h"
#include "JAnimInstance.h"
#include "IModelServer.h"

/***********************************************************************************/
/*  JAnimInstance implementation
/***********************************************************************************/
decl_class(JAnimInstance)
JAnimInstance::JAnimInstance()
{
    m_Weight        = 1.0f;
    m_AnimID        = -1;
    m_ModelID       = -1;
    m_InstanceID    = -1;
    m_SectionID     = -1;
    m_BlendInTime   = 0.0f;
    m_pInstance     = NULL;
} // JAnimInstance::JAnimInstance

void JAnimInstance::Init()
{
    JAnimation::Init();
    if (m_Model.size() > 0)
    {
        m_ModelID = g_pModelServer->GetModelID( m_Model.c_str() );
        if (m_ModelID == -1)
        {
            rlog.warn( "Could not load model %s", m_Model.c_str() );
        }
    }

    if (m_Anim.size())
    {
        m_AnimID = g_pModelServer->GetAnimID( m_Anim.c_str() );
        if (m_AnimID == -1)
        {
            rlog.warn( "Could not load animation %s", m_Anim.c_str() );
        }
    }
    
    if (m_Instance.size() == 0)
    {
        m_InstanceID = g_pModelServer->InstanceModel( m_ModelID );
    }

    if (GetDuration() == 0.0f) 
    {
        SetDuration( g_pModelServer->GetAnimDuration( m_AnimID )*GetTempo() );
    }
    
    if (m_Section.size() > 0 && m_SectionID == -1)
    {
        m_SectionID = g_pModelServer->GetAnimSectionID( m_AnimID, m_Section.c_str() );
        SetDuration( g_pModelServer->GetAnimDuration( m_AnimID, m_SectionID )*GetTempo() );
    }
} // JAnimInstance::ResInit

void JAnimInstance::SetModel( const char* name )
{
    m_Model = name;
    m_ModelID = -1;
    m_ModelID = g_pModelServer->GetModelID( m_Model.c_str() );
    if (m_ModelID == -1)
    {
        rlog.warn( "Could not load model %s", m_Model.c_str() );
    }
}

void JAnimInstance::SetAnim( const char* name )
{
    m_Anim = name;
    m_AnimID = -1;
    
    if (m_Anim.size())
    {
        m_AnimID = g_pModelServer->GetAnimID( m_Anim.c_str() );
        if (m_AnimID == -1)
        {
            rlog.warn( "Could not load animation %s", m_Anim.c_str() );
            return;
        }
        SetSection( m_Section.c_str() );
    }
}

void JAnimInstance::SetSection( const char* name )
{
    m_Section = name;
    //  FIXME:
    if (m_AnimID == -1)
    {
        SetAnim( m_Anim.c_str() );
    }

    m_SectionID = g_pModelServer->GetAnimSectionID( m_AnimID, m_Section.c_str() );
    SetDuration( g_pModelServer->GetAnimDuration( m_AnimID, m_SectionID )*GetTempo() );
}

// FIXME:
void JAnimInstance::EnsureInited()
{
    if (m_ModelID == -1 || (m_AnimID == -1 && m_Anim.size() > 0))
    {
        ResInit();
    }
    
    if (m_Section.size() > 0 && m_SectionID == -1)
    {
        m_SectionID = g_pModelServer->GetAnimSectionID( m_AnimID, m_Section.c_str() );
        SetDuration( g_pModelServer->GetAnimDuration( m_AnimID, m_SectionID )*GetTempo() );
    }

    if (m_Instance.size() > 0)
    {
        if (m_pInstance == NULL)
        {
            m_pInstance = g_pObjectServer->FindObject<JModelInstance>( m_Instance.c_str(), this );
        }
        if (m_pInstance)
        {
            m_InstanceID = m_pInstance->GetInstanceID();
        }
    }
}

void JAnimInstance::OnPlay()
{
} // JAnimInstance::OnPlay

void JAnimInstance::Render()
{
    JAnimation::Render();
    if (!IsPlaying())
    {
        return;
    }
    
    EnsureInited();
    
    JAnmContext& ctx = GetContext();
    if (ctx.m_bStateOnly)
    {
        return;
    }

    float weight = m_Weight;
    float cTime = GetCurTime();
    if (m_BlendInTime > 0.0f)
    {
        float blendFactor = cTime/m_BlendInTime;
        blendFactor = clamp( blendFactor, 0.0f, 1.0f );
        weight *= blendFactor;
    }

    g_pModelServer->AnimateModel( m_ModelID, Mat4::identity, m_AnimID, cTime, m_InstanceID, weight, m_SectionID );
} // JAnimInstance::Render

