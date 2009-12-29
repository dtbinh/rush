/***********************************************************************************/
//  File:   JSkyPlane.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JSkyPlane.h"
#include "IModelServer.h"

/***********************************************************************************/
/*  JSkyPlane implementation
/***********************************************************************************/
decl_class(JSkyPlane);
JSkyPlane::JSkyPlane()
{
    m_PlaneID        = -1;
    m_PlaneShaderID  = -1;
    m_PlaneModel     = "skyplane";
} // JSkyPlane::JSkyPlane

void JSkyPlane::SetPlaneModel( const char* name ) 
{ 
    m_PlaneModel = name; 
    m_PlaneID = g_pModelServer->GetModelID( m_PlaneModel.c_str() );
} // JSkyPlane::SetPlaneModel

void JSkyPlane::Init()
{
    m_PlaneID = g_pModelServer->GetModelID( m_PlaneModel.c_str() );
    m_PlaneShaderID = g_pRenderServer->GetShaderID( "skyplane" );
} // JSkyPlane:Init

void JSkyPlane::Render()
{
    g_pRenderServer->SetShader( m_PlaneShaderID );
    g_pModelServer->DrawModel( m_PlaneID, Mat4::identity );
} // JSkyPlane::Render