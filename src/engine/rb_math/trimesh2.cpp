/***********************************************************************************/
//  File:   TriMesh2.cpp
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "trimesh2.h"
#include "tri2.h"

/***********************************************************************************/
/*  TriMesh2 implementation
/***********************************************************************************/
void TriMesh2::Clear()
{
    m_Faces.clear();
    m_Verts.clear();
}

bool TriMesh2::IndicesAreCorrect()
{
    for (int i = 0; i < m_Faces.size(); i++)
    {
        const Face& f = m_Faces[i];
        if (f.va < 0 || f.va >= m_Verts.size()) return false;
        if (f.vb < 0 || f.vb >= m_Verts.size()) return false;
        if (f.vc < 0 || f.vc >= m_Verts.size()) return false;
    }
    return true;
} // TriMesh2::IndicesAreCorrect

bool TriMesh2::PtIn( const Vec2& pt ) const
{
    int nF = m_Faces.size();
    for (int i = 0; i < nF; i++)
    {
        const Face& f = m_Faces[i];
        Tri2 tri( m_Verts[f.va], m_Verts[f.vb], m_Verts[f.vc] );
        if (tri.contains( pt ))
        {
            return true;
        }
    }
    return false;
} // TriMesh2::PtIn

