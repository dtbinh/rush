/***********************************************************************************/
//  File:   ColMesh.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLMESH_H__
#define __COLMESH_H__

#include "ColGeom.h"

/***********************************************************************************/
//  Class:  ColMesh
//  Desc:   
/***********************************************************************************/
class ColMesh : public ColGeom
{
    JVec3List           m_Vertices;
    JVec3List           m_CachedVertices;

    JIntList            m_Indices;
    dMass               m_MeshMass;
    dTriMeshDataID      m_TriMeshDataID;

public:
                        ColMesh     ();
    virtual dGeomID     CreateGeom  ( dSpaceID spaceID );
    virtual bool        IsPlaceable () const { return false; }
    virtual void        Synchronize ( bool bFromSolver );

    virtual void        Serialize   ( OutStream& os ) const;
    virtual bool        Unserialize ( InStream& is );

    virtual void        DrawBounds  ();
    virtual dMass       GetMass     () const;

    void                AddVertex   ( const Vec3& v ) { m_Vertices.push_back( v ); }
    void                AddPoly     ( int a, int b, int c )
    {
        m_Indices.push_back( a );
        m_Indices.push_back( b );
        m_Indices.push_back( c );
    }


    expose( ColMesh )
    {
        parent( ColGeom );
    }
}; // class ColMesh

#endif //__ColMesh_H__