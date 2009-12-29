/***********************************************************************************/
//  File:   ColCapsule.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ColCapsule.h"
#include "PhysicsServer.h"
#include "OdeUtil.h"

/***********************************************************************************/
/*  ColCapsule implementation
/***********************************************************************************/
decl_class(ColCapsule);
ColCapsule::ColCapsule()
{
    m_Height      = 200.0f;
    m_Radius      = 50.0f;
} // ColCapsule::ColCapsule

void ColCapsule::Synchronize( bool bFromSolver )
{
    ColGeom::Synchronize( bFromSolver );
    if (!bFromSolver)
    {
        float scale = GetBodyScale()*PhysicsServer::s_pInstance->GetWorldScale();
        float height = m_Height - m_Radius*2.0f;
        dGeomCapsuleSetParams( GetID(), m_Radius*scale, height*scale );
    }
} // ColCapsule::Synchronize

void ColCapsule::DrawBounds()
{
    DWORD geomColor  = PhysicsServer::s_pInstance->GetGeomColor();
    DWORD linesColor = PhysicsServer::s_pInstance->GetGeomLinesColor();

    Mat4 tm = GetTM();
    g_pDrawServer->SetWorldTM( tm );
    float height = m_Height - m_Radius*2.0f;
    float hf = height*0.5f;
    g_pDrawServer->SetZEnable( true );
    g_pDrawServer->DrawCylinder( Vec3( 0.0f, 0.0f, -hf ), m_Radius, height, linesColor, geomColor, false );
    g_pDrawServer->DrawSpherePart( Vec3( 0.0f, 0.0f, -hf ), m_Radius, 0.0f, 360.0f, 90.0f, 90.0f, linesColor, geomColor );
    g_pDrawServer->DrawSpherePart( Vec3( 0.0f, 0.0f,  hf ), m_Radius, 0.0f, 360.0f, 0.0f,  90.0f, linesColor, geomColor );
    g_pDrawServer->SetZEnable( false );
    g_pDrawServer->Flush();
} // ColCapsule::DrawBounds

dGeomID ColCapsule::CreateGeom( dSpaceID spaceID ) 
{
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    float height = m_Height - m_Radius*2.0f;
    return dCreateCapsule( spaceID, m_Radius*scale, height*scale );
} // ColCapsule::CreateGeom

dMass ColCapsule::GetMass() const
{ 
    dMass m; 
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    float height = m_Height - m_Radius*2.0f;
    dMassSetCapsule( &m, GetDensity(), 3, m_Radius*scale, height*scale );
    return m; 
}