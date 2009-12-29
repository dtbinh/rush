/***********************************************************************************/
//  File:   Mesh.h
//  Date:   3.09.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "TriMesh2.h"
#include "VertexDeclaration.h"

/***********************************************************************************/
//  Class:  Mesh
//  Desc:   
/***********************************************************************************/
class Mesh
{
    std::vector<Face>   m_Faces;
    std::vector<BYTE>   m_VertexData;
    VertexDeclaration   m_VDecl;

    //  cached strides
    int                 m_dRHW;        
    int                 m_dNormal;     
    int                 m_dTangent;    
    int                 m_dBinormal;   
    int                 m_dUV;         
    int                 m_dBlendWeight;
    int                 m_dBlendIdx;   
    int                 m_dDiffuse;    
    int                 m_dSpecular;   

public:
                        Mesh            ();
    void                Clear           ();
    void                ClearFaces      ();
    void                ClearVerts      ();

    void                Create          ( const VertexDeclaration& vd );
    void                AddFace         ( const Face& f ) { m_Faces.push_back( f ); }
    void                AddFace         ( int va, int vb, int vc ) { m_Faces.push_back( Face( va, vb, vc ) ); }
    
    void                AddVert         ( const Vec3& pos );

    const Vec3&         GetPos          ( int idx ) const;
    float               GetRHW          ( int idx ) const;
    const Vec3&         GetNormal       ( int idx ) const;
    const Vec3&         GetTangent      ( int idx ) const;
    const Vec3&         GetBinormal     ( int idx ) const;
    const Vec2&         GetUV           ( int idx ) const;
    const Vec2&         GetUV           ( int idx, int uvPair ) const;
    float               GetBlendWeight  ( int idx, int wIdx ) const;
    BYTE                GetBlendIdx     ( int idx, int wIdx ) const;
    DWORD               GetDiffuse      ( int idx ) const;
    DWORD               GetSpecular     ( int idx ) const;


    bool                SetPos          ( int idx, const Vec3& pos );
    bool                SetRHW          ( int idx, float rhw );
    bool                SetRHW          ( float rhw );
    bool                SetNormal       ( int idx, const Vec3& normal );
    bool                SetTangent      ( int idx, const Vec3& tangent );
    bool                SetBinormal     ( int idx, const Vec3& binormal );
    bool                SetUV           ( int idx, const Vec2& uv ) const;
    bool                SetUV           ( int idx, int uvPair, const Vec2& uv ) const;
    bool                SetBlendWeight  ( int idx, int wIdx, float w ) const;
    bool                SetBlendIdx     ( int idx, int wIdx, BYTE blendIdx ) const;
    bool                SetDiffuse      ( int idx, DWORD diffuse ) const;
    bool                SetSpecular     ( int idx, DWORD specular ) const;
    void                SetNFaces       ( int nFaces );
    void                SetNVerts       ( int nVerts );

    int                 GetNVert        () const 
    { 
        if (GetStride() == 0) return 0;
        return (int)(m_VertexData.size()/GetStride()); 
    } 
    int                 GetStride       () const { return m_VDecl.m_VertexSize; }
    int                 GetNFaces       () const { return (int)m_Faces.size(); } 
    const Face&         GetFace         ( int idx ) const { return m_Faces[idx]; } 


}; // class Mesh

#endif //__MESH_H__