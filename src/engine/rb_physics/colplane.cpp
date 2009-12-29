/***********************************************************************************/
//  File:   ColPlane.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ColPlane.h"
#include "PhysicsServer.h"

/***********************************************************************************/
/*  ColPlane implementation
/***********************************************************************************/
decl_class(ColPlane);
ColPlane::ColPlane()
{
    m_Plane = Plane::xOy;
}

dGeomID ColPlane::CreateGeom( dSpaceID spaceID ) 
{
    float scale = PhysicsServer::s_pInstance->GetWorldScale();
    Plane pl( m_Plane );
    pl.Normal().normalize();
    return dCreatePlane( spaceID, pl.a, pl.b, pl.c, -pl.d*scale );
} // ColPlane::CreateGeom