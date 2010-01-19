/***********************************************************************************/
//  File:   JCamera.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "frustum.h"
#include "jcamera.h"

/***********************************************************************************/
/*  JCamera implementation
/***********************************************************************************/
decl_class(JCamera);
JCamera::JCamera()
{
    m_ZNear     = 10.0f;
    m_ZFar      = 50000.0f;
    m_FOVx      = c_HalfPI;
    m_Aspect    = 4.0f/3.0f;
    ValidateProjTM();

    m_WorldTM       = Mat4::identity;
    m_bOrtho        = false;
    m_PreviousVP    = Frame( 0, 0, 1024, 768 );
    m_bRestoreState = false;

    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );

    SetLocalExt( Frame( 0, 0, 1024, 768 ) );
} // JCamera::JCamera

void JCamera::SetViewDir( Vec3 dir )
{
    Vec3 right( m_WorldTM.e00, m_WorldTM.e01, m_WorldTM.e02 );
    Vec3 up   ( m_WorldTM.e10, m_WorldTM.e11, m_WorldTM.e12 );
    dir.ortonormalize( right, up );
    if (!dir.ortonormalize( right, up ))
    {
        right = dir.is_equal( Vec3::oZ ) ? Vec3::oX : Vec3::oZ;
        dir.ortonormalize( right, up );
    }

    m_WorldTM.e00 = right.x;
    m_WorldTM.e01 = right.y;
    m_WorldTM.e02 = right.z;

    m_WorldTM.e10 = up.x;
    m_WorldTM.e11 = up.y;
    m_WorldTM.e12 = up.z;

    m_WorldTM.e20 = dir.x;
    m_WorldTM.e21 = dir.y;
    m_WorldTM.e22 = dir.z;
} // JCamera::SetViewDir

void JCamera::SetViewUp( Vec3 up )
{
    Vec3 right( m_WorldTM.e00, m_WorldTM.e01, m_WorldTM.e02 );
    Vec3 dir  ( m_WorldTM.e20, m_WorldTM.e21, m_WorldTM.e22 );
    if (!up.ortonormalize( dir, right ))
    {
        dir = up.is_equal( Vec3::oZ ) ? Vec3::oX : Vec3::oZ;
        up.ortonormalize( dir, right );
    }

    m_WorldTM.e00 = right.x;
    m_WorldTM.e01 = right.y;
    m_WorldTM.e02 = right.z;

    m_WorldTM.e10 = up.x;
    m_WorldTM.e11 = up.y;
    m_WorldTM.e12 = up.z;

    m_WorldTM.e20 = dir.x;
    m_WorldTM.e21 = dir.y;
    m_WorldTM.e22 = dir.z;
} // JCamera::SetViewUp

void JCamera::Render()
{
    Frame ext = GetExt();
	if (ext.w > c_FltEpsilon)
	{
		float aspect = ext.w/ext.h;
        if (fabs( aspect - m_Aspect ) > c_FltEpsilon)
        {
            m_Aspect = aspect;
            ValidateProjTM();
        }
	}

    m_PreviousVP = g_pRenderServer->GetViewport();
    g_pRenderServer->SetProjTM( m_ProjTM );

    Mat4 viewTM;
    viewTM.inverse( m_WorldTM );
    g_pRenderServer->SetViewTM( viewTM );
    g_pRenderServer->SetWorldTM( Mat4::identity );

    g_pRenderServer->SetViewport( ext );
    g_pRenderServer->ClearDepthStencil( true, true );
} // JCamera::Render

void JCamera::PostRender()
{
    if (m_bRestoreState)
    {
        g_pRenderServer->SetViewport( m_PreviousVP );
    }
} // JCamera::PostRender

void JCamera::ValidateProjParam()
{
    if (m_bOrtho)
    {
        m_FOVx = 2.0f/m_ProjTM.e00;
        m_ZFar = (1.0f - m_ProjTM.e32)/m_ProjTM.e22;
    }
    else
    {
        m_FOVx = 2.0f*atanf( 1.0f/m_ProjTM.e00 );
        m_ZFar = m_ProjTM.e32/(1.0f - m_ProjTM.e22);
    }

    m_ZNear = -m_ProjTM.e32/m_ProjTM.e22;
    m_Aspect = m_ProjTM.e11/m_ProjTM.e00;
} // JCamera::ValidateParam

void JCamera::ValidateProjTM()
{
    if (m_bOrtho)
    {
        OrthoProjTM( m_ProjTM, m_FOVx, m_ZNear, m_ZFar, m_Aspect );
    }
    else
    {
        PerspProjTM( m_ProjTM, m_FOVx, m_ZNear, m_ZFar, m_Aspect );
    }
} // JCamera::ValidateProjTM

bool JCamera::GetTM( JSpace src, JSpace dest, Mat4& tm ) const
{
    //  from world space
    if (src == Space_World)
    {
        switch (dest)
        {
        case Space_World:
            tm = Mat4::identity;
            break;
        case Space_View:
            tm.inverse( m_WorldTM );
            break;
        case Space_Projection:
            tm.inverse( m_WorldTM );
            tm *= m_ProjTM;
            break;
        case Space_Screen:
            {
                tm.inverse( m_WorldTM );
                tm *= m_ProjTM;
                Mat4 sTM;
                ProjToScreenTM( sTM, GetExt() );
                tm *= sTM;
            }
            break;
        default:
            return false;
        }
        return true;
    }

    //  from view space
    if (src == Space_View)
    {
        switch (dest)
        {
        case Space_World:
            tm = m_WorldTM;
            break;
        case Space_View:
            tm = Mat4::identity;
            break;
        case Space_Projection:
            tm = m_ProjTM;
            break;
        case Space_Screen:
            {
                tm = m_ProjTM;
                Mat4 sTM;
                ProjToScreenTM( sTM, GetExt() );
                tm *= sTM;
            }
            break;
        default:
            return false;
        }
        return true;
    }

    //  from projection space
    if (src == Space_Projection)
    {
        switch (dest)
        {
        case Space_World:
            tm.inverse( m_ProjTM );
            tm *= m_WorldTM;
            break;
        case Space_View:
            tm.inverse( m_ProjTM );
            break;
        case Space_Projection:
            tm = Mat4::identity;
            break;
        case Space_Screen:
            ProjToScreenTM( tm, GetExt() );
            break;
        default:
            return false;
        }
        return true;
    }

    //  from screen space
    if (src == Space_Screen)
    {
        switch (dest)
        {
        case Space_World:
            {
                ScreenToProjTM( tm, GetExt() );
                Mat4 ipTM;
                ipTM.inverse( m_ProjTM );
                tm *= ipTM;
                tm *= m_WorldTM;
            }
            break;
        case Space_View:
            {
                ScreenToProjTM( tm, GetExt() );
                Mat4 ipTM;
                ipTM.inverse( m_ProjTM );
                tm *= ipTM;
            }
            break;
        case Space_Projection:
            ScreenToProjTM( tm, GetExt() );
            break;
        case Space_Screen:
            tm = Mat4::identity;
            break;
        default:
            return false;
        }
        return true;
    }
    return false;
} // JCamera::GetTM

void JCamera::DrawBounds()
{
    Vec3 center = m_WorldTM.translation();
    Vec3 right( m_WorldTM.e00, m_WorldTM.e01, m_WorldTM.e02 );
    Vec3 up   ( m_WorldTM.e10, m_WorldTM.e11, m_WorldTM.e12 );
    Vec3 dir  ( m_WorldTM.e20, m_WorldTM.e21, m_WorldTM.e22 );

    const float c_HandleLen = 100.0f;
    g_pDrawServer->DrawLine( center, center + right*c_HandleLen, 0xFFFF0000, 0xFFFF0000 );
    g_pDrawServer->DrawLine( center, center + up*c_HandleLen,    0xFF00FF00, 0xFF00FF00 );
    g_pDrawServer->DrawLine( center, center + dir*c_HandleLen,   0xFF0000FF, 0xFF0000FF );

    Frustum frustum( GetViewTM()*GetProjTM() );
    g_pDrawServer->DrawFrustum( frustum, 0xFFFF5555, 0x11555555 );

    g_pDrawServer->Flush();

} // JCamera::DrawBounds






