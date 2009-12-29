/***********************************************************************************/
//  File:   JSky.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JSky.h"
#include "IModelServer.h"

/***********************************************************************************/
/*  JSky implementation
/***********************************************************************************/
decl_class(JSky);
JSky::JSky()
{
    m_DomeID        = -1;
    m_DomeShaderID  = -1;
    m_DomeModel     = "skydome";
} // JSky::JSky

void JSky::SetDomeModel( const char* name ) 
{ 
    m_DomeModel = name; 
    m_DomeID = g_pModelServer->GetModelID( m_DomeModel.c_str() );
} // JSky::SetDomeModel

void JSky::Init()
{
    m_DomeID = g_pModelServer->GetModelID( m_DomeModel.c_str() );
    m_DomeShaderID = g_pRenderServer->GetShaderID( "skydome" );
} // JSky:Init

void JSky::Render()
{
    g_pRenderServer->SetShader( m_DomeShaderID );
    g_pModelServer->DrawModel( m_DomeID, Mat4::identity );
} // JSky::Render