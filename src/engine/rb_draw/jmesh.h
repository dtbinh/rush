/***********************************************************************************/
//  File:   JMesh.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JMESH_H__
#define __JMESH_H__

#include "jskincluster.h"
#include "staticarray.h"

enum_beg(PrimitiveType)
enum_val( "Unknown",        PrimitiveType_Unknown         ),
enum_val( "PointList",      PrimitiveType_PointList       ),
enum_val( "LineList",       PrimitiveType_LineList        ),
enum_val( "LineStrip",      PrimitiveType_LineStrip       ),
enum_val( "TriangleList",   PrimitiveType_TriangleList    ),
enum_val( "TriangleStrip",  PrimitiveType_TriangleStrip   ),
enum_val( "TriangleFan",    PrimitiveType_TriangleFan     ),
enum_val( "QuadList",       PrimitiveType_QuadList        )
enum_end(PrimitiveType)

enum_beg(VertexType)
enum_val( "Unknown",  VertexType_Unknown    ),
enum_val( "XYZDUV2",  VertexType_XYZDUV2    ),
enum_val( "XYZWDSUV", VertexType_XYZWDSUV   ),
enum_val( "XYZDUV",   VertexType_XYZDUV     ),
enum_val( "XYZNUV",   VertexType_XYZNUV     ),
enum_val( "XYZUV",    VertexType_XYZUV      ),
enum_val( "XYZD",     VertexType_XYZD       ),
enum_val( "XYZDNUV",  VertexType_XYZDNUV    ),
enum_val( "XYZNUVW4", VertexType_XYZNUVW4   )
enum_end(VertexType)

class Buffer;
class JBoneInstance;
const int c_MaxBonesPerVertex = 4;
typedef static_array<JSkinCluster, c_MaxBonesPerVertex> JSkinClusterArray;
/***********************************************************************************/
//  Class:  JMesh
//  Desc:
/***********************************************************************************/
class JMesh : public JObject
{
    VertexDeclaration       m_VDecl;
    int                     m_FirstVByte;
    int                     m_NVertices;

    int                     m_FirstIndex;
    int                     m_NIndices;

    PrimitiveType           m_PrimitiveType;

    JString                 m_MaterialName;
    int                     m_MaterialID;       //  shader index in model's material palette

    JString                 m_HostBone;         //  the host bone this geometry is bound to
    JStringList             m_BoneList;         //  in case of skinned mesh list of bones mesh is bound to

    int                     m_HostBoneID;
    std::vector<int>        m_BoneID;
    std::vector<Mat4>       m_BoneOffsets;
    std::vector<Mat4>       m_BoneTM;
    JSkinClusterArray       m_SkinClusters;
    std::vector<VertexN>    m_SkinnedVert;
    Mat4                    m_ShiftTM;

    bool                    m_bDynamic;
    int                     m_IBID;
    AABox                   m_AABB;
    Mat4                    m_NormTM;


    int                     m_VBufID;       //  vertex buffer ID
    int                     m_VBFirstByte;  //  first byte in the renderer's VB for static vertices
    int                     m_VBIteration;  //  current generation (drop stamp) for static vertices

    JModel*                 m_pModel;

public:
                            JMesh               ();

    void                    Render              ( int firstIByte, JBoneInstance* pSkelInst,
                                                  const Mat4& tm, bool bUseCachedData = false );

    const char*             GetMaterialName     () const { return m_MaterialName.c_str(); }
    void                    SetHostBone         ( const char* name ) { m_HostBone = name; }
    void                    SetHostBoneID       ( int idx ) { m_HostBoneID = idx; }
    void                    SetMaterialID       ( int idx ) { m_MaterialID = idx; }
    void                    AddBone             ( const char* name ) { m_BoneList.push_back( name ); }
    void                    ClearBones          () { m_BoneList.clear(); }

    int                     GetHostBoneID       () const { return m_HostBoneID; }
    int                     GetMaterialID       () const { return m_MaterialID; }
    int                     GetNVert            () const { return m_NVertices; }
    int                     GetNIndices         () const { return m_NIndices; }

    bool                    GetComponents       ( Buffer& buf, VertexComponent component ) const;
    bool                    GetIndices          ( Buffer& buf ) const;
    bool                    SetIndices          ( const Buffer& buf );

    const VertexDeclaration& GetVDecl           () const { return m_VDecl; }
    int                     GetFirstVByte       () const { return m_FirstVByte; }
    int                     GetFirstIndex       () const { return m_FirstIndex; }

    bool                    IsDynamic           () const { return m_bDynamic; }
    void                    SetDynamic          ( bool bDynamic ) { m_bDynamic = bDynamic; }
    virtual void            Process             ( int firstVertIdx ) {}

    void                    ClearSkinClusters   () { m_SkinClusters.clear(); }
    bool                    AddSkinCluster      ( int firstV, int numV, int numWeights );
    int                     GetNSkinClusters    () const { return m_SkinClusters.size(); }

    virtual bool            LinkBones           ( JBoneInstance* pBones, int nBones );
    const char*             GetHostBone         () const { return m_HostBone.c_str(); }
    int                     GetNSkinBones       () const { return m_BoneList.size(); }

    void                    SetVertexRange      ( int firstByte, int numVert );
    void                    SetIndexRange       ( int firstIdx, int numIdx );
    void                    SetMaterial         ( const char* name ) { m_MaterialName = name; }
    void                    SetVDecl            ( const VertexDeclaration& vdecl ) { m_VDecl = vdecl; }

    VertexType              GetVertexType       () const { return (VertexType)m_VDecl.m_TypeID; }
    void                    SetVertexType       ( VertexType type ) { m_VDecl = GetStockVDecl( type ); }

    void                    CacheVB             ();
    void                    SetModel            ( JModel* pModel ) { m_pModel = pModel; }
    bool                    IsSkinned           () const
    {
        return (m_VDecl.m_TypeID == VertexType_XYZNUVW4 && m_BoneID.size());
    }

    virtual void            Init                ();
    void                    ProcessSkin         ( JBoneInstance* pSkelInst, const Mat4& tm );

    expose( JMesh )
    {
        parent( JObject );
        field( "Material",      m_MaterialName  );
        field( "FirstVByte",    m_FirstVByte    );
        field( "NumVertices",   m_NVertices     );

        field( "FirstIndex",    m_FirstIndex    );
        field( "NumIndices",    m_NIndices      );

        field( "PrimitiveType", m_PrimitiveType );
        prop ( "VertexType",    GetVertexType, SetVertexType );
        field( "HostBone",      m_HostBone      );
        field( "BoneList",      m_BoneList      );
        field( "Dynamic",       m_bDynamic      );
        field( "NormTM",        m_NormTM        );

    }

private:
    int FindBoneInstance( const char* boneName, JBoneInstance* pBones, int nBones ) const;
}; // class JMesh

#endif //__JMESH_H__
