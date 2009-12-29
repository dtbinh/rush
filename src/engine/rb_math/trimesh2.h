/***********************************************************************************/
//  File:   TriMesh2.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TRIMESH2_H__
#define __TRIMESH2_H__

#include <vector>
#include "Vec2.h"

struct Face
{
    int     va, vb, vc;

    Face() {}
    Face( int a, int b, int c ) : va( a ), vb( b ), vc( c ) {}
}; // struct Face

/***********************************************************************************/
//  Class:  TriMesh2
//  Desc:   
/***********************************************************************************/
class TriMesh2
{
    std::vector<Face>   m_Faces;
    std::vector<Vec2>   m_Verts;
public:

    void                Clear       ();
    void                ClearFaces  ();
    void                ClearVerts  ();

    void                AddFace     ( const Face& f ) { m_Faces.push_back( f ); }
    void                AddFace     ( int va, int vb, int vc ) { m_Faces.push_back( Face( va, vb, vc ) ); }
    void                AddVert     ( const Vec2& v ) { m_Verts.push_back( v ); }

    int                 GetNVerts   () const { return (int)m_Verts.size(); } 
    int                 GetNFaces   () const { return (int)m_Faces.size(); } 
    const Vec2&         GetVert     ( int idx ) const { return m_Verts[idx]; } 
    const Face&         GetFace     ( int idx ) const { return m_Faces[idx]; } 
    bool                PtIn        ( const Vec2& pt ) const;
    
    bool                IndicesAreCorrect();

}; // class TriMesh2

#endif //__TRIMESH2_H__