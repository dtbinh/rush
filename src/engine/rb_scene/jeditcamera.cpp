/***********************************************************************************/
//  File:   JEditCamera.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JEditCamera.h"
#include "JCamera.h"
#include "windows.h"

/***********************************************************************************/
/*  JEditCamera implementation
/***********************************************************************************/
decl_class(JEditCamera);
JEditCamera::JEditCamera()
{
    m_HomeTilt      	= 0.0f;
    m_HomeZoom      	= 800.0f;
    m_HomeRoll      	= 0.0f; 

    m_Tilt          	= m_HomeTilt;
    m_Zoom          	= m_HomeZoom;
    m_Roll          	= m_HomeRoll;

    m_MinRoll       	= 1.0f;
    m_MaxRoll       	= -1.0f;
    m_MinTilt       	= -180;
    m_MaxTilt       	= 0;

    m_MaxZoom       	= 1200.0f;
    m_MinZoom       	= 300.0f;

    m_RollSpeed         = 0.2f;
    m_TiltSpeed         = 0.2f;
    m_ZoomSpeed         = 10.0f;

    m_RollDelta         = 0.0f;
    m_TiltDelta         = 0.0f;
    m_ZoomDelta         = 0.0f;

    m_LookAt            = Vec3::null;

    m_LookAtDelta       = Vec3( 0.0f, 0.0f, 0.0f );
    m_FocusDist         = 0.0f;
    m_bUseControlKey    = true;

    m_MousePos.x    	= -1;
    m_MousePos.y    	= -1;

    m_Timer.Start();
}  

void JEditCamera::Init()
{
	JCameraController::Init();
	Home();
}  

void JEditCamera::Home()
{
	m_Tilt = m_HomeTilt;
	m_Roll = m_HomeRoll;
	m_Zoom = m_HomeZoom;
}  

void JEditCamera::Render()
{
	JCamera* pCamera = GetCamera();
	if (!pCamera) return;

    //  scrolling lookAt point
    POINT pt;
    GetCursorPos( &pt );
    int mx = pt.x;
    int my = pt.y;

    m_Roll   += m_RollDelta;
    m_Tilt   += m_TiltDelta;
    m_Zoom   += m_ZoomDelta;
	m_LookAt += m_LookAtDelta;
	
	//  recalculate camera view/projection matrices
    m_RollDelta     = 0.0f;
    m_TiltDelta     = 0.0f;
    m_ZoomDelta     = 0.0f;
    m_LookAtDelta   = Vec3::null;

	Frame ext = GetExt();
    if (m_MinRoll <= m_MaxRoll) m_Roll = clamp( m_Roll,  m_MinRoll, m_MaxRoll );
    if (m_MinTilt <= m_MaxTilt) m_Tilt = clamp( m_Tilt,  m_MinTilt, m_MaxTilt );
    if (m_MinZoom <= m_MaxZoom) m_Zoom = clamp( m_Zoom,  m_MinZoom, m_MaxZoom );

    Vec3 cx( 1.0f, 0.0f, 0.0f );
    Vec3 cy( 0.0f, -1.0f, 0.0f );
    Vec3 cz( 0.0f, 0.0f, -1.0f );
    Mat3 bTM( cx, cy, cz );
    
    Mat3 rollTM, tiltTM, rotTM( bTM );
    tiltTM.rot( Vec3::oX, DegToRad( m_Tilt ) );
    rollTM.rot( Vec3::oZ, DegToRad( m_Roll ) );
    rotTM *= tiltTM;
    rotTM *= rollTM;
    Vec3 gdir( rotTM.e20, rotTM.e21, rotTM.e22 );

    gdir.normalize();
    Vec3 pos( gdir );
    pos.reverse();
	pos *= m_Zoom - pCamera->GetZNear();

    pos += m_LookAt;
    
    Mat4 cTM = Mat4( rotTM, pos );
    
    pCamera->SetWorldTM( cTM );
}  

void JEditCamera::OnMouse( JMouseEvent& e )
{
	JCamera* pCamera = GetCamera();
	if (!pCamera) return;

    if (GetKeyState( VK_MENU ) >= 0 && m_bUseControlKey)
    {
        m_MousePos = Vec2( e.MouseX(), e.MouseY() );
        return;
    }

    if (e.Action() == aKeyDown)  
    {
        if (e.MouseKey() == mkRight)
        {
            m_Mode = ViewMode_Zooming;
            CaptureMouse();
        }
        else if (e.MouseKey() == mkLeft)   
        {
            m_Mode = ViewMode_Rolling;
            CaptureMouse();
        }
        else if (e.MouseKey() == mkMiddle)
        {
            m_Mode = ViewMode_Panning;
            CaptureMouse();
        }
    }

    if (e.MouseKey() == mkNone)
    {
        m_Mode = ViewMode_Idle;
    }

    if (e.Action() == aKeyUp)   
    {
        m_Mode = ViewMode_Idle;
        CaptureMouse( false );
    }

    if (e.Action() == aMouseMove && m_Mode != ViewMode_Idle)
    {
         if (m_Mode == ViewMode_Rolling && 
             m_MousePos.x >=0.0f && m_MousePos.y >=0.0f)
         {
            float dy = e.MouseY() - m_MousePos.y;
            float dx = e.MouseX() - m_MousePos.x;
            m_RollDelta = dx*m_RollSpeed;
            m_TiltDelta = dy*m_TiltSpeed;
            e.Consume();
         }

         Mat4 vpTM, swTM;
         pCamera->GetTM( Space_View, Space_Projection, vpTM );
         pCamera->GetTM( Space_Screen, Space_World, swTM );

         if (m_Mode == ViewMode_Panning && 
             m_MousePos.x >=0.0f && m_MousePos.y >=0.0f)
         {
            float dist = tmax( m_FocusDist, pCamera->GetZNear() );
            Vec4 focusPt( 0.0f, 0.0f, dist, 1.0f );

            vpTM.tm( focusPt );
            focusPt.normW();

            Vec4 p0( m_MousePos.x, m_MousePos.y, focusPt.z, 1.0f );
            Vec4 p1( e.MouseX(),   e.MouseY(),   focusPt.z, 1.0f );
            
            swTM.tm( p0 );
            swTM.tm( p1 );
            p0.normW();
            p1.normW();
            
            Vec4 dp = p0 - p1;
            m_LookAtDelta = Vec3( dp.x, dp.y, dp.z );
            e.Consume();
         }

         if (m_Mode == ViewMode_Zooming && 
             m_MousePos.x >=0.0f && m_MousePos.y >=0.0f)
         {
            float dist = tmax( m_FocusDist, pCamera->GetZFar() );
            Vec4 focusPt( 0.0f, 0.0f, dist, 1.0f );

            vpTM.tm( focusPt );
            focusPt.normW();

            Vec4 p0( m_MousePos.x,  m_MousePos.y, focusPt.z, 1.0f );
            Vec4 p1( e.MouseX(),    e.MouseY(),   focusPt.z, 1.0f );

            float dx = e.MouseX() - m_MousePos.x;
            float dy = e.MouseY() - m_MousePos.y;
            
            swTM.tm( p0 );
            swTM.tm( p1 );
            p0.normW();
            p1.normW();
            
            Vec4 dp = p0 - p1;
            m_ZoomDelta = dp.norm();
            if (_fabs( dy ) > _fabs( dx ))
            {
                if (m_MousePos.y < e.MouseY()) m_ZoomDelta = -m_ZoomDelta;
            }
            else
            {
                if (m_MousePos.x < e.MouseX()) m_ZoomDelta = -m_ZoomDelta;
            }
            e.Consume();
         }
    }

    m_MousePos = Vec2( e.MouseX(), e.MouseY() );
}  

void JEditCamera::OnWheel( JWheelEvent& e )
{
    m_ZoomDelta = -m_ZoomSpeed*float( e.Delta() );
    m_Mode = ViewMode_Zooming;
    e.Consume();
}  