
//  File:   JLocation.cpp
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JDialog.h"
#include "JAnimation.h"
#include "JQuestGame.h"
#include "IModelServer.h"
#include "JObjectIterator.h"
#include "JLocation.h"


/***********************************************************************************/
/*  JLocation implementation
/***********************************************************************************/
decl_class(JLocation);
JLocation::JLocation()
{
    SetFocusable( false );
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );

    m_pAnmFadeIn    = NULL;
    m_CameraFOV     = 40.0f;
    m_CameraTilt    = 10.0f;
    m_CameraDist    = 150.0f;
    m_LightDir      = Vec3( 0.4f, 0.4, -0.8f );
    m_ShadowColor   = 0xFFAAAAAA;
} // JLocation::JLocation

void JLocation::Init()
{
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
} // JLocation::Init

void JLocation::OnShow()
{
    //  perform resource initialization for children
    JObjectIterator it( this );
    while (it)
    {
        (*it)->ResInit();
        ++it;
    }
} // JLocation::OnShow

void JLocation::SetupCamera()
{
    //  projection matrix
    Mat4 projTM, viewTM;
    const float c_CameraZNear = 0.5f;
    const float c_CameraZFar  = 1000.0f;
    PerspProjTM( projTM , DegToRad( m_CameraFOV ), c_CameraZNear, c_CameraZFar, GetExt().w/GetExt().h );
    
    Mat3 camTM( Vec3::oX, Vec3::oZ, -Vec3::oY );
    Mat3 rotTM( Vec3::oX, DegToRad( m_CameraTilt ) );
    camTM *= rotTM;
    Vec3 dir   = camTM.v2();
    Vec3 pos   = -dir*m_CameraDist;
    viewTM     = camTM;
    viewTM.e30 = pos.x;
    viewTM.e31 = pos.y;
    viewTM.e32 = pos.z;
    viewTM.inverse( viewTM );

    g_pRenderServer->SetProjTM( projTM );
    g_pRenderServer->SetViewTM( viewTM );
    g_pRenderServer->SetViewport( GetExt() );

} // JLocation::SetupCamera

void JLocation::Render()
{
    SetupCamera();
    g_pModelServer->SetShadowColor( m_ShadowColor );
    Vec3 lightDir = m_LightDir;
    lightDir.normalize();
    g_pModelServer->SetLightDir( lightDir );
} // JLocation::Render

void JLocation::PostRender()
{
} // JLocation::PostRender

void JLocation::OnClose()
{
    g_pQuestGame->StopAllSounds();

    if (!m_pAnmFadeIn)
    {
        m_pAnmFadeIn = g_pObjectServer->FindObject<JAnimation>( "LocationFadeIn", this );
    }

    if (m_pAnmFadeIn)
    {
        m_pAnmFadeIn->Play();
    }
} // JLocation::OnClose
