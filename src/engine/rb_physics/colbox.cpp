/***********************************************************************************/
//  File:   ColBox.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "AABox.h"
#include "ColBox.h"
#include "PhysicsServer.h"
#include "OdeUtil.h"

/***********************************************************************************/
/*  ColBox implementation
/***********************************************************************************/
decl_class(ColBox);
ColBox::ColBox()
{
    m_Extents = Vec3( 10.0f, 10.0f, 10.0f );
} // ColBox::ColBox

void ColBox::Synchronize( bool bFromSolver )
{
    ColGeom::Synchronize( bFromSolver );
    if (!bFromSolver)
    {
        float scale = GetBodyScale()*PhysicsServer::s_pInstance->GetWorldScale();
        dGeomBoxSetLengths( GetID(), m_Extents.x*scale, m_Extents.y*scale, m_Extents.z*scale );
    }
} // ColBox::Synchronize

void ColBox::DrawBounds()
{
    DWORD geomColor  = PhysicsServer::s_pInstance->GetGeomColor();
    DWORD linesColor = PhysicsServer::s_pInstance->GetGeomLinesColor();
    Mat4 tm = GetTM();
    g_pDrawServer->SetWorldTM( tm );
    g_pDrawServer->SetZEnable( true );
    g_pDrawServer->DrawBox( AABox( Vec3::null, m_Extents.x, m_Extents.y, m_Extents.z ),
                    linesColor, geomColor );
    g_pDrawServer->SetZEnable( false );
    g_pDrawServer->Flush();
} // ColBox::DrawBounds

dGeomID ColBox::CreateGeom( dSpaceID spaceID ) 
{
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    Vec3 ext = m_Extents;
    ext *= scale;
    return dCreateBox( spaceID, ext.x, ext.y, ext.z );
} // ColBox::CreateGeom

dMass ColBox::GetMass() const
{ 
    dMass m; 
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    Vec3 ext = m_Extents;
    ext *= scale;
    dMassSetBox( &m, GetDensity(), ext.x, ext.y, ext.z );
    return m; 
}