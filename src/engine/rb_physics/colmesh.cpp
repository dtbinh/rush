/***********************************************************************************/
//  File:   ColMesh.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ColMesh.h"
#include "PhysicsServer.h"

/***********************************************************************************/
/*  ColMesh implementation
/***********************************************************************************/
decl_class(ColMesh);
ColMesh::ColMesh()
{
    dMassSetZero( &m_MeshMass );
    m_TriMeshDataID = 0;
}

void ColMesh::Synchronize( bool bFromSolver )
{
    ColGeom::Synchronize( bFromSolver );
    if (!bFromSolver)
    {
    
    }
} // ColMesh::Synchronize

dGeomID ColMesh::CreateGeom( dSpaceID spaceID ) 
{
    float scale = GetBodyScale()*PhysicsServer::s_pInstance->GetWorldScale();
    m_TriMeshDataID = dGeomTriMeshDataCreate();
    int nVert = m_Vertices.size();
    m_CachedVertices.resize( nVert );
    for (int i = 0; i < nVert; i++)
    {
        m_CachedVertices[i] = scale*m_Vertices[i];
    }

    dGeomTriMeshDataBuildSingle( m_TriMeshDataID, &m_CachedVertices[0], 3*sizeof( float ), 
        nVert, (int*)&m_Indices[0], m_Indices.size(), 3*sizeof( int ) );
    dGeomID id = dCreateTriMesh( spaceID, m_TriMeshDataID, 0, 0, 0 );

    // remember the mesh's dTriMeshDataID on its userdata for convenience.
    dGeomSetData( id, m_TriMeshDataID );

    dMassSetTrimesh( &m_MeshMass, GetDensity(), id );
    return id;
} // ColMesh::CreateGeom

dMass ColMesh::GetMass() const
{ 
    return m_MeshMass; 
} // ColMesh::GetMass

void ColMesh::DrawBounds()
{
    Mat4 tm = GetTM();
    g_pDrawServer->SetWorldTM( tm );
    DWORD clrFill  = PhysicsServer::s_pInstance->GetGeomColor();
    DWORD clrLine  = PhysicsServer::s_pInstance->GetGeomLinesColor();
    int nFaces = m_Indices.size()/3;
    for (int i = 0; i < nFaces; i++)
    {
        const Vec3& a = m_Vertices[m_Indices[i*3]];
        const Vec3& b = m_Vertices[m_Indices[i*3 + 1]];
        const Vec3& c = m_Vertices[m_Indices[i*3 + 2]];
        g_pDrawServer->DrawPoly( a, b, c, clrFill, clrFill, clrFill );
        g_pDrawServer->DrawLine( a, b, clrLine, clrLine );
        g_pDrawServer->DrawLine( b, c, clrLine, clrLine );
        g_pDrawServer->DrawLine( c, a, clrLine, clrLine );
    }
    g_pDrawServer->Flush();
} 

void ColMesh::Serialize( OutStream& os ) const
{
    os << m_Indices << m_Vertices;
}

bool ColMesh::Unserialize( InStream& is )
{
    is >> m_Indices >> m_Vertices;
    return true;
}