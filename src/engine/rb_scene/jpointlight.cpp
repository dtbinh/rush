/***********************************************************************************/
//  File:   JPointLight.cpp
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JLight.h"
#include "JPointLight.h"

/***********************************************************************************/
/*  JPointLight implementation
/***********************************************************************************/
decl_class(JPointLight);
JPointLight::JPointLight()
{
    m_Position = Vec3( 0.0f, 0.0f, 0.0f );
    m_Radius   = 10.0f;
    m_Falloff  = 1.0f;
    m_Diffuse  = 0xFFFFFFFF;
    m_Specular = 0xFFFFFFFF;
} // JPointLight::JPointLight

void JPointLight::Render()
{
    m_ID = g_pRenderServer->AddPointLight( m_Position, m_Radius, m_Falloff, m_Diffuse, m_Specular, m_bPerPixel );
} // JPointLight::Render

