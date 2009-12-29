/***********************************************************************************/
//  File:   ColCylinder.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ColCylinder.h"
#include "PhysicsServer.h"
#include "OdeUtil.h"

/***********************************************************************************/
/*  ColCylinder implementation
/***********************************************************************************/
decl_class(ColCylinder);
ColCylinder::ColCylinder()
{
    m_Height      = 200.0f;
    m_Radius      = 50.0f;
} // ColCylinder::ColCapsule

void ColCylinder::Synchronize( bool bFromSolver )
{
    ColGeom::Synchronize( bFromSolver );
    if (!bFromSolver)
    {
        float scale = GetBodyScale()*PhysicsServer::s_pInstance->GetWorldScale();
        dGeomCylinderSetParams( GetID(), m_Radius*scale, m_Height*scale );
    }
} // ColCylinder::Synchronize

void ColCylinder::DrawBounds()
{
    DWORD geomColor  = PhysicsServer::s_pInstance->GetGeomColor();
    DWORD linesColor = PhysicsServer::s_pInstance->GetGeomLinesColor();

    Mat4 tm = GetTM();
    g_pDrawServer->SetWorldTM( tm );
    float hf = m_Height*0.5f;
    g_pDrawServer->SetZEnable( true );
    g_pDrawServer->DrawCylinder( Vec3( 0.0f, 0.0f, -hf ), m_Radius, m_Height, linesColor, geomColor, true );
    g_pDrawServer->SetZEnable( false );
    g_pDrawServer->Flush();
} // ColCylinder::DrawBounds

dGeomID ColCylinder::CreateGeom( dSpaceID spaceID ) 
{
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    return dCreateCylinder( spaceID, m_Radius*scale, m_Height*scale );
} // ColCylinder::CreateGeom

dMass ColCylinder::GetMass() const
{ 
    dMass m; 
    Convert( m.c, Vec4( 0, 0, 0, 1 ) );
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    dMassSetCylinder( &m, GetDensity(), 3, m_Radius*scale, m_Height*scale );
    return m; 
}