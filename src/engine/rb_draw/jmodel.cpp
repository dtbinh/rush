/***********************************************************************************/
//  File:   JModel.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "AABox.h"
#include "JMesh.h"
#include "JMaterial.h"
#include "JBone.h"
#include "FStream.h"
#include "Path.h"
#include "IFileServer.h"
#include "IPersistServer.h"
#include "JObjectIterator.h"
#include "Buffer.h"
#include "JModel.h"


/***********************************************************************************/
/*  JModel implementation
/***********************************************************************************/
decl_class(JModel);
JModel::JModel()
{
    m_IBFirstByte   = 0;
    m_IBIteration   = -1;
    m_IBufID        = -1;
    m_ModelID       = -1;
    m_bDrawSkeleton = false;
} // JModel::JModel

void JModel::Init()
{
    m_IBFirstByte  = 0;
    m_IBIteration  = -1;
    m_IBufID       = -1;
    m_IBufID       = g_pRenderServer->GetIBID( "DrawSystemStatic" );
    GatherChildren();
} // JModel::Init

JMaterial* JModel::FindMaterial( JMaterial* pMtl )
{
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JMaterial* pcMtl = obj_cast<JMaterial>( GetChild( i ) );
        if (!pcMtl) continue;
        if (pMtl->IsEqual( pcMtl )) return pcMtl;
    }
    return NULL;
} // JModel::HasMaterial

int JModel::GetNMaterials() const
{
    int nM = 0;
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JMaterial* pcMtl = obj_cast<JMaterial>( GetChild( i ) );
        if (pcMtl) nM++;
    }
    return nM;
} // JModel::GetNMaterials

int JModel::GetNFaces() const
{
    return m_Indices.GetSize()/(3*sizeof( WORD ));
} // JModel::GetNFaces

int JModel::GetNVertices() const
{
    int nVert = 0;
    for (int i = 0; i < m_Meshes.size(); i++)
    {
        nVert += m_Meshes[i]->GetNVert();
    }
    return nVert;
} // JModel::GetNVertices

void JModel::Clear()
{
    m_IBFirstByte  = 0;
    m_IBIteration  = -1;
    m_IBufID       = -1;

    RemoveChildren();
    m_Vertices.Clear();
    m_Indices.Clear();
} // JModel::Clear

void JModel::Serialize( OutStream& os ) const
{
    m_Indices.Write( os );
    m_Vertices.Write( os );
}

bool JModel::Unserialize( InStream& is )
{
    bool bOK = true;
    bOK &= m_Indices.Read( is );
    bOK &= m_Vertices.Read( is );
    return bOK;
}

void JModel::GatherBones()
{
    m_SkelInst.clear();
    //  count total number of bones
    int numBones = 0;
    JObjectIterator it( this );
    while (it)
    {
        if (obj_cast<JBone>( *it )) numBones++;
        ++it;
    }
    m_SkelInst.clear();
    m_SkelInst.reserve( numBones );

    const int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JBone* pBone = obj_cast<JBone>( GetChild( i ) );
        GatherBones( pBone );
    }

    assert( numBones == m_SkelInst.size() );
} // JModel::GatherBones

void JModel::GatherBones( JBone* pBone, int parentIdx )
{
    if (!pBone) return;
    m_SkelInst.push_back( JBoneInstance() );
    JBoneInstance& binst = m_SkelInst.back();

    binst.m_LocalTM   = pBone->GetTransform();
    binst.m_ParentIdx = parentIdx;
    binst.m_pBone     = pBone;
    binst.m_Index     = m_SkelInst.size() - 1;

    int nCh = pBone->GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JBone* pCurChild = obj_cast<JBone>( pBone->GetChild( i ) );
        GatherBones( pCurChild, binst.m_Index );
    }
} // JModel::GatherBones

void JModel::GatherChildren()
{
    m_Materials.clear();
    m_Meshes.clear();

    GatherBones();
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JMaterial* pMaterial = obj_cast<JMaterial>( GetChild( i ) );
        if (pMaterial) 
        {
            m_Materials.push_back( pMaterial );
            pMaterial->Init();
        }
    }
    
    //  calculate rest pose world transforms for bones
    UpdateSkeleton( Mat4::identity, NULL );
    for (int i = 0; i < nCh; i++)
    {
        JMesh* pMesh = obj_cast<JMesh>( GetChild( i ) );
        if (!pMesh) continue;
        //  find material index
        const char* mtlName = pMesh->GetMaterialName();
        int idx = -1;
        for (int j = 0; j < m_Materials.size(); j++)
        {
            if (!stricmp( m_Materials[j]->GetName(), mtlName )) 
            {
                idx = j;
                break;
            }
        }
        pMesh->SetMaterialID( idx );
        pMesh->SetModel( this );
        pMesh->Init();
        pMesh->LinkBones( &m_SkelInst[0], m_SkelInst.size() );
        pMesh->SetModel( this );
        m_Meshes.push_back( pMesh );
    }
} // JModel::GatherChildren

int JModel::FindBoneInstance( const char* boneName )
{
    for (int i = 0; i < m_SkelInst.size(); i++)
    {
        if (!stricmp( m_SkelInst[i].m_pBone->GetName(), boneName )) return i;
    }
    return -1;
} // JModel::FindBoneInstance

void JModel::DrawSkeleton( const Mat4& tm, JBoneInstance* pSkelInst )
{
    g_pRenderServer->SetWorldTM( Mat4::identity );
    int nBones = m_SkelInst.size();
    if (!pSkelInst && nBones > 0) pSkelInst = &m_SkelInst[0];
    for (int i = 0; i < nBones; i++)
    {
        JBoneInstance& bi = pSkelInst[i];
        if (bi.m_ParentIdx != -1) 
        {
            const Vec3& pos = pSkelInst[bi.m_ParentIdx].m_ModelTM.translation();
            g_pDrawServer->DrawLine( pos, bi.m_ModelTM.translation(), 0xFFFFFF00, 0xFFFFFF00 );
        }
        else
        {
            const float c_HandleSize = 0.05f;
            g_pDrawServer->DrawBox( AABox( bi.m_ModelTM.translation(), c_HandleSize ), 0xFFFFFF00, 0x22FFFF00 );
        }
    }
    g_pDrawServer->Flush();
} // JModel::DrawSkeleton

void JModel::UpdateSkeleton( const Mat4& tm, JBoneInstance* pSkelInst )
{
    int nBones = m_SkelInst.size();
    if (!pSkelInst && nBones > 0) pSkelInst = &m_SkelInst[0];
    for (int i = 0; i < nBones; i++)
    {
        JBoneInstance& bi = pSkelInst[i];
        if (bi.m_ParentIdx == -1) 
        {
            bi.m_ModelTM = bi.m_LocalTM*tm;
        }
        else
        {
            JBoneInstance& pbi = pSkelInst[bi.m_ParentIdx];
            bi.m_ModelTM = bi.m_LocalTM*pbi.m_ModelTM;
        }
    }
} // JModel::UpdateSkeleton

void JModel::Render( const Mat4& tm, JBoneInstance* pSkelInst, bool bIgnoreShader, bool bUseCachedData )
{
    if (m_IBufID == -1)
    {
        Init();
    }

    //  check geometry caching status, update if needed
    g_pRenderServer->CacheIB( m_IBufID, m_Indices.GetData(),  m_Indices.GetSize(),  m_IBIteration, m_IBFirstByte );

    int nBones = m_SkelInst.size();
    //  calculate world-space transforms for bones 
    if (!pSkelInst && nBones > 0) pSkelInst = &m_SkelInst[0];
    UpdateSkeleton( Mat4::identity, pSkelInst );

    //  cycle through geometries and render them
    int nGeom = m_Meshes.size();
    int cMtl = -1;
    for (int i = 0; i < nGeom; i++)
    {
        JMesh* pMesh = m_Meshes[i];
        if (!pMesh->IsVisible()) 
        {
            continue;
        }
        pMesh->CacheVB();
        int mtlID = pMesh->GetMaterialID();
        if (mtlID >= 0 && mtlID != cMtl)
        {
            JMaterial* pMtl = m_Materials[mtlID];
            pMtl->Render( bIgnoreShader );
            cMtl = mtlID;
        }
        Mat4 worldTM = tm;
        int hostBoneID = pMesh->GetHostBoneID();
        if (hostBoneID >= 0 && pMesh->GetNSkinBones() == 0)
        {
            JBoneInstance& hb = pSkelInst[hostBoneID];
            if (!hb.m_bVisible)
            {
                continue;
            }
            Mat4 rootTM = hb.m_ModelTM*tm;
            worldTM = rootTM;
        }
        
        pMesh->Render( m_IBFirstByte, pSkelInst, worldTM, bUseCachedData );
    }

    if (m_bDrawSkeleton)
    {
        DrawSkeleton( tm, pSkelInst );
    }

} // JModel::Render

void JModel::AddIndices( WORD* pIdx, int numIdx )  
{ 
    m_Indices.AddData( (BYTE*)pIdx, numIdx*2 ); 
} // JModel::AddIndices

void JModel::Reserve( int nVertexBytes, int numIdx )
{
    m_Vertices.Resize( nVertexBytes );
    m_Indices.Resize( numIdx*2 );
} // JModel::Reserve
