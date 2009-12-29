/***********************************************************************************/
//  File:   ColSphere.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ColSphere.h"
#include "PhysicsServer.h"
#include "OdeUtil.h"

/***********************************************************************************/
/*  ColSphere implementation
/***********************************************************************************/
decl_class(ColSphere);
ColSphere::ColSphere()
{
    m_Radius = 100.0f;
}

void ColSphere::Synchronize( bool bFromSolver )
{
    ColGeom::Synchronize( bFromSolver );
    if (!bFromSolver)
    {
        float scale = GetBodyScale()*PhysicsServer::s_pInstance->GetWorldScale();
        dGeomSphereSetRadius( GetID(), m_Radius*scale );
    }
} // ColSphere::Synchronize

void ColSphere::DrawBounds()
{
    DWORD geomColor  = PhysicsServer::s_pInstance->GetGeomColor();
    DWORD linesColor = PhysicsServer::s_pInstance->GetGeomLinesColor();

    Mat4 tm = GetTM();
    g_pDrawServer->SetWorldTM( tm );
    g_pDrawServer->SetZEnable( true );
    g_pDrawServer->DrawSphere( Vec3::null, m_Radius, linesColor, geomColor );
    g_pDrawServer->SetZEnable( false );
    g_pDrawServer->Flush();
} // ColSphere::DrawBounds

dGeomID ColSphere::CreateGeom( dSpaceID spaceID ) 
{
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    return dCreateSphere( spaceID, m_Radius*scale );
} // ColSphere::CreateGeom

dMass ColSphere::GetMass() const
{ 
    dMass m; 
    Convert( m.c, Vec4( 0, 0, 0, 1 ) );
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    dMassSetSphere( &m, GetDensity(), m_Radius*scale );
    return m; 
}

