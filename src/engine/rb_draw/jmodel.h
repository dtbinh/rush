/***********************************************************************************/
//  File:   JModel.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JMODEL_H__
#define __JMODEL_H__

#include "Buffer.h"

#include "JBoneInstance.h"

class JMaterial;
class JMesh;
/***********************************************************************************/
//  Class:  JModel
//  Desc:   
/***********************************************************************************/
class JModel : public JObject
{
    Buffer                      m_Indices;      //  indices buffer
    Buffer                      m_Vertices;     //  static vertices buffer

    std::string                 m_GeomFile;     //  geometry file name

    std::vector<JBoneInstance>  m_SkelInst;     //  original skeleton pose
    std::vector<JMaterial*>     m_Materials;    //  list of materials

    std::vector<JMesh*>         m_Meshes;       //  list of meshes in model

    int                         m_IBufID;       //  index buffer ID
    int                         m_IBFirstByte;  //  first byte in the renderer's IB  
    int                         m_IBIteration;  //  current generation (drop stamp) for indices

    bool                        m_bDrawSkeleton;
    int                         m_ModelID;

public:
                    JModel          ();

    int             GetModelID      () const { return m_ModelID; }
    void            SetModelID      ( int id ) { m_ModelID = id; }

    JMaterial*      FindMaterial    ( JMaterial* pMtl );
    int             GetNMaterials   () const;
    int             GetNFaces       () const;
    int             GetNVertices    () const;
    int             GetNBones       () const { return m_SkelInst.size(); }
    int             GetNMeshes      () const { return m_Meshes.size(); }

    JMaterial*      GetMaterial     ( int idx ) const { return m_Materials[idx]; }
    JMesh*          GetMesh         ( int idx ) const { return m_Meshes[idx]; }
    JBoneInstance*  GetBoneInstance ( int idx ) { return &m_SkelInst[idx]; }
    JBoneInstance*  GetBoneInstances() { return m_SkelInst.size() > 0 ? &m_SkelInst[0] : NULL; }
    int             FindBoneInstance( const char* boneName );

    virtual void    Init            ();

    bool            IsDrawSkeleton  () const { return m_bDrawSkeleton; }
    void            SetDrawSkeleton ( bool bDraw ) { m_bDrawSkeleton = bDraw; }

    const Buffer&   GetVBuf         () const { return m_Vertices; }
    const Buffer&   GetIBuf         () const { return m_Indices;  }

    Buffer&         GetVBuf         () { return m_Vertices; }
    Buffer&         GetIBuf         () { return m_Indices;  }

    void            Clear           ();

    void            Serialize       ( OutStream& os ) const;
    bool            Unserialize     ( InStream& is );

    void            GatherChildren  ();
    void            GatherBones     ();
    void            Render          ( const Mat4& tm, JBoneInstance* pSkelInst = NULL, 
                                      bool bIgnoreShader = false, bool bUseCachedData = false );

    void            SetVertices     ( uint8_t* pVert, int numBytes ) { m_Vertices.SetData( pVert, numBytes ); }
    void            SetIndices      ( uint16_t* pIdx, int numIdx )    { m_Indices.SetData( (uint8_t*)pIdx, numIdx*2 ); }

    void            AddVertices     ( uint8_t* pVert, int numBytes ) { m_Vertices.AddData( pVert, numBytes ); }
    void            AddIndices      ( uint16_t* pIdx, int numIdx );

    void            Reserve         ( int nVertexBytes, int numIdx );

    const char*     GetGeomFile     () const { return m_GeomFile.c_str(); }
    void            UpdateSkeleton  ( const Mat4& tm, JBoneInstance* pSkelInst );

    expose( JModel )
    {
        parent( JObject );
        field( "DrawSkeleton", m_bDrawSkeleton );
        field( "GeomFile", m_GeomFile );
    }

private:
    void            GatherBones     ( JBone* pBone, int parentIdx = -1 );
    void            DrawSkeleton    ( const Mat4& tm, JBoneInstance* pSkelInst );
}; // class JModel

#endif //__JMODEL_H__