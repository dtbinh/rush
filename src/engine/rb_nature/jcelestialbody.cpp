/***********************************************************************************/
//  File:   JCelestialBody.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JCelestialBody.h"
#include "Color.h"
#include "IModelServer.h"

/***********************************************************************************/
/*  JCelestialBody implementation
/***********************************************************************************/
decl_class(JCelestialBody);
JCelestialBody::JCelestialBody()
{
    m_ModelID       = -1;
    m_ShaderID      = -1;
    m_Model         = "sun";
    m_Latitude      = 10.0f;
    m_Longitude     = 10.0f;
    m_Distance      = 1000.0f;
    m_hColor        = -1;
    m_Scale         = 1.0f;
} // JCelestialBody::JCelestialBody

void JCelestialBody::SetModel( const char* name ) 
{ 
    m_Model = name; 
    m_ModelID = g_pModelServer->GetModelID( m_Model.c_str() );
} // JCelestialBody::SetModel

void JCelestialBody::Init()
{
    m_ModelID    = g_pModelServer->GetModelID( m_Model.c_str() );
    m_ShaderID   = g_pRenderServer->GetShaderID( "celestial" );

    m_hColor     = g_pRenderServer->GetConstantID( m_ShaderID, "g_Color" );
} // JCelestialBody:Init

void JCelestialBody::Render()
{
    g_pRenderServer->SetShader( m_ShaderID );
    ColorF color( m_Color );
    //g_pRenderServer->SetConstant( m_ShaderID, m_hColor, color.r, color.g, color.b, color.a );


    float lg = DegToRad( m_Longitude );
    float lt = DegToRad( m_Latitude  );
    Vec3 dir( -cosf( lg ), -sinf( lg ), -sinf( lt ) );
    dir.normalize();
    Vec3 up( 0.0f, 0.0f, 1.0f );
    Vec3 right;
    right.cross( up, dir );
    right.normalize();
    up.cross( dir, right );
    up.normalize();
    Mat4 camTM;
    camTM.inverse( g_pRenderServer->GetViewTM() );
    Vec3 pos( camTM.e30, camTM.e31, camTM.e32 );
    pos -= m_Distance*dir;
    Mat3 rot( right, up, dir );
    //rot.transpose();
    Mat4 dirTM;
    dirTM.srt( m_Scale, rot, pos );

    //dirTM = Mat4::identity;

    g_pModelServer->DrawModel( m_ModelID, dirTM );
} // JCelestialBody::Render