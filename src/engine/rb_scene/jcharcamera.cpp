/***********************************************************************************/
//  File:   JCharCamera.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"

#include "JWeakRef.h"
#include "JCharCamera.h"
#include "JCamera.h"
#include "JModelInstance.h"
#include "IWindowServer.h"
#include "IModelServer.h"

/***********************************************************************************/
/*  JCharCamera implementation
/***********************************************************************************/
decl_class(JCharCamera);
JCharCamera::JCharCamera()
{
    m_TrackPoint    = Vec3::null;
    m_TrackDist     = 10.0f;
    m_RotateSpeed   = 0.01f;

    m_Heading       = 0.0f;
    m_Pitch         = 0.0f;
    m_pModelInst    = NULL;

    m_HeadingDelta  = 0.0f;
    m_PitchDelta    = 0.0f;

    m_MinPitch      = -45.0f;
    m_MaxPitch      = 90.0f;
    m_MinHeading    = -100000.0f;
    m_MaxHeading    = 100000.0f;

    m_MousePos      = Vec2::null;
} // JCharCamera::JCharCamera

void JCharCamera::Init()
{
    JCameraController::Init();
    m_Timer.Start();
    m_pModelInst = g_pObjectServer->FindObject<JModelInstance>( m_ModelInstance.c_str(), this );
}

void JCharCamera::Render()
{
	JCamera* pCamera = GetCamera();
	if (!pCamera) return;

    m_Pitch     += m_PitchDelta;
    m_Heading   += m_HeadingDelta;
    
    m_Pitch   = clamp( m_Pitch,     m_MinPitch, m_MaxPitch );
    m_Heading = clamp( m_Heading,   m_MinHeading, m_MaxHeading );
    
    m_PitchDelta    = 0.0f;
    m_HeadingDelta  = 0.0f;
    
    //  get position of the bone tracked
    Mat4 boneTM = Mat4::identity;
    if (m_pModelInst)
    {
        int modelID = m_pModelInst->GetModelID();
        int boneID = g_pModelServer->GetModelBoneID( modelID, m_TrackBone.c_str() );
        g_pModelServer->GetBoneTM( m_pModelInst->GetInstanceID(), boneID, boneTM );
    }

    Vec3 lookAt = m_TrackPoint;
    lookAt += boneTM.translation();
    
    float theta = DegToRad( m_Heading );
    float phi   = DegToRad( m_Pitch );
    Vec3 camPos;
    camPos.x = m_TrackDist*cosf( theta )*sinf( phi );
    camPos.y = m_TrackDist*sinf( theta )*sinf( phi );
    camPos.z = m_TrackDist*cosf( phi );
    camPos += m_TrackPoint;
    camPos += boneTM.translation();

    Vec3 dir = lookAt - camPos;
    dir.normalize();

    Vec3 up = Vec3::oZ;
    Vec3 right;

    right.cross( up, dir );
    right.normalize();
    up.cross( dir, right );
    up.normalize();

    Mat4 camTM( right, up, dir, camPos );
    Mat4 viewTM;
    viewTM.inverse( camTM );

    pCamera->SetViewTM( viewTM );
    
} // JCharCamera::Render

void JCharCamera::OnMouse( JMouseEvent& e )
{
	JCamera* pCamera = GetCamera();
	if (!pCamera) return;

    int mX = e.MouseX();
    int mY = e.MouseY();
    if (e.Action() == aMouseMove)
    {
        Vec2 mouseDelta = Vec2( e.MouseX(), e.MouseY() ) - m_MousePos;
        Vec4 mouse1( e.MouseX(), e.MouseY(), 1.0f, 1.0f );
        Vec4 mouse2( m_MousePos.x, m_MousePos.y, 1.0f, 1.0f );
        
        Mat4 spTM;
        pCamera->GetTM( Space_Screen, Space_Projection, spTM );

        spTM.tm( mouse1 );
        spTM.tm( mouse2 );
        mouse1.normW();
        mouse2.normW();

        m_PitchDelta    = (mouse1.y - mouse2.y)*m_RotateSpeed;
        m_HeadingDelta  = (mouse1.x - mouse2.x)*m_RotateSpeed;

        const int c_MouseGutter = 5;
        Frame ext = GetExt();
        if (mX < ext.x + c_MouseGutter)   
        {
            mX = ext.r() - c_MouseGutter;
            g_pWindowServer->SetMousePos( Vec2( mX, mY ) );
        }
        if (mX > ext.r() - c_MouseGutter) 
        {
            mX = ext.x + c_MouseGutter;
            g_pWindowServer->SetMousePos( Vec2( mX, mY ) );
        }
        if (mY < ext.y + c_MouseGutter)   
        {
            mY = ext.b() - c_MouseGutter;
            g_pWindowServer->SetMousePos( Vec2( mX, mY ) );
        }
        if (mY > ext.b() - c_MouseGutter) 
        {
            mY = ext.y + c_MouseGutter;
            g_pWindowServer->SetMousePos( Vec2( mX, mY ) );
        }
    }

    m_MousePos = Vec2( mX, mY );
} // JCharCamera::OnMouse

void JCharCamera::OnWheel( JWheelEvent& e )
{
} // JCharCamera::OnWheel