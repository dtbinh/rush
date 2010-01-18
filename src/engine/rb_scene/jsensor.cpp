/***********************************************************************************/
//  File:   JSensor.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JSensor.h"
#include "IWindowServer.h"
#include "Mat4.h"

/***********************************************************************************/
/*  JSensor implementation
/***********************************************************************************/
decl_class(JSensor);
JSensor::JSensor()
{
    m_Position          = Vec3::null;
    m_bScreenSpace      = true;
    m_Radius            = 1.0f;
    m_AttachBoneID      = -1;
    m_AttachInstanceID  = -1;
} // JSensor::JSensor

void JSensor::SetAttachInstance( const char* name )
{
    m_AttachInstance    = name;
    m_AttachBoneID      = -1;
    m_AttachInstanceID  = -1;
}

void JSensor::SetAttachBone( const char* name )
{
    m_AttachBone    = name;
    m_AttachBoneID  = -1;
}

void JSensor::DrawBounds()
{
    
} // JSensor::DrawBounds

void JSensor::SetScreenSpace( bool bSet )
{
    if (bSet == m_bScreenSpace)
    {
        return;
    }
    m_bScreenSpace = bSet;
    Mat4 tm; 
    ProjToScreenTM( tm, g_pRenderServer->GetViewport() );
    if (g_pRenderServer)
    {
        tm = g_pRenderServer->GetViewTM()*g_pRenderServer->GetProjTM()*tm;
    }
    if (!m_bScreenSpace)
    {
        tm = inverse( tm );
    }
    tm.tmproj( m_Position );
} // JSensor::SetScreenSpace

bool JSensor::PtIn( int px, int py ) const
{
    int dx = px - (int)m_Position.x;
    int dy = py - (int)m_Position.y;
    bool bIn = dx*dx + dy*dy < m_Radius*m_Radius;
    return bIn;
} // JSensor::PtIn