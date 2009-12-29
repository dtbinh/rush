/***********************************************************************************/
//  File:   JDirLight.cpp
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JLight.h"
#include "JDirLight.h"

/***********************************************************************************/
/*  JDirLight implementation
/***********************************************************************************/
decl_class(JDirLight);
JDirLight::JDirLight()
{
    m_Latitude  = 5.0f;
    m_Longitude = -120.0f;
    m_Diffuse   = 0xFFFFFFFF;
    m_Specular  = 0xFFFFFFFF;
} // JDirLight::JDirLight

void JDirLight::Render()
{
    float lg = DegToRad( m_Longitude );
    float lt = DegToRad( m_Latitude  );
    Vec3 dir( -cosf( lg ), -sinf( lg ), -sinf( lt ) );
    dir.normalize();

    m_ID = g_pRenderServer->AddDirLight( dir, m_Diffuse, m_Specular, m_bPerPixel );
} // JDirLight::Render

