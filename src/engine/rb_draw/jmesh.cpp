/***********************************************************************************/
//  File:   JMesh.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "buffer.h"
#include "jbone.h"
#include "jboneinstance.h"
#include "jmodel.h"
#include "aabox.h"
#include "jmesh.h"

/***********************************************************************************/
/*  JMesh implementation
/***********************************************************************************/
decl_class(JMesh);

JMesh::JMesh() :
        m_VDecl         ( GetStockVDecl( VertexType_XYZNUV ) ),
        m_FirstVByte    ( -1 ),
        m_NVertices     ( 0 ),
        m_FirstIndex    ( -1 ),
        m_NIndices      ( 0 ),
        m_PrimitiveType ( PrimitiveType_TriangleList ),
        m_MaterialID    ( -1 ),
        m_HostBoneID    ( -1 ),
        m_bDynamic      ( false ),
        m_pModel        ( NULL ),
        m_VBufID        ( -1 ),
        m_VBFirstByte   ( 0  ),
        m_IBID          ( -1 ),
        m_ShiftTM       ( Mat4::identity ),
        m_NormTM        ( Mat4::identity )
{   
}

void Skin1( const VertexW4* pSrc, VertexN* pDst, int nVert, const Mat4* pTM )
{
    for (int i = 0; i < nVert; i++)
    {
        const VertexW4& sv = pSrc[i];
        VertexN& dv = pDst[i];
        uint8_t boneIdx = sv.boneIdx[0];
        const Mat4& tm = pTM[boneIdx];
        dv.x = sv.x; dv.y = sv.y; dv.z = sv.z; 
        dv.nx = sv.nx; dv.ny = sv.ny; dv.nz = sv.nz; 
        dv.u = sv.u; dv.v = sv.v;

        tm.tmpt( *((Vec3*)&dv.x) );
        tm.tmvec( *((Vec3*)&dv.nx) );
    }
} // Skin1

inline float ByteToF( uint8_t b )
{
    return float( b )/255.0f;
}

static inline void WeightAdd( Mat4& m, const Mat4& ma, float wa, const Mat4& mb, float wb )
{
    m.e00 = ma.e00*wa + mb.e00*wb;
    m.e01 = ma.e01*wa + mb.e01*wb;
    m.e02 = ma.e02*wa + mb.e02*wb;

    m.e10 = ma.e10*wa + mb.e10*wb;
    m.e11 = ma.e11*wa + mb.e11*wb;
    m.e12 = ma.e12*wa + mb.e12*wb;

    m.e20 = ma.e20*wa + mb.e20*wb;
    m.e21 = ma.e21*wa + mb.e21*wb;
    m.e22 = ma.e22*wa + mb.e22*wb;

    m.e30 = ma.e30*wa + mb.e30*wb;
    m.e31 = ma.e31*wa + mb.e31*wb;
    m.e32 = ma.e32*wa + mb.e32*wb;

} // WeightAdd

void Skin2( const VertexW4* pSrc, VertexN* pDst, int nVert, const Mat4* pTM )
{
    Mat4 tm( Mat4::identity );
    for (int i = 0; i < nVert; i++)
    {
        const VertexW4& sv = pSrc[i];
        VertexN& dv = pDst[i];
        WeightAdd( tm, pTM[sv.boneIdx[0]], ByteToF( sv.weights[0] ), 
                       pTM[sv.boneIdx[1]], ByteToF( sv.weights[1] ) );
        dv.x = sv.x; dv.y = sv.y; dv.z = sv.z; 
        dv.nx = sv.nx; dv.ny = sv.ny; dv.nz = sv.nz; 
        dv.u = sv.u; dv.v = sv.v;

        tm.tmpt( *((Vec3*)&dv.x) );
        tm.tmvec( *((Vec3*)&dv.nx) );
    }
} // Skin2

static inline void WeightAdd( Mat4& m, const Mat4& ma, float wa, const Mat4& mb, float wb, const Mat4& mc, float wc )
{
    m.e00 = ma.e00*wa + mb.e00*wb + mc.e00*wc;
    m.e01 = ma.e01*wa + mb.e01*wb + mc.e01*wc;
    m.e02 = ma.e02*wa + mb.e02*wb + mc.e02*wc;

    m.e10 = ma.e10*wa + mb.e10*wb + mc.e10*wc;
    m.e11 = ma.e11*wa + mb.e11*wb + mc.e11*wc;
    m.e12 = ma.e12*wa + mb.e12*wb + mc.e12*wc;

    m.e20 = ma.e20*wa + mb.e20*wb + mc.e20*wc;
    m.e21 = ma.e21*wa + mb.e21*wb + mc.e21*wc;
    m.e22 = ma.e22*wa + mb.e22*wb + mc.e22*wc;

    m.e30 = ma.e30*wa + mb.e30*wb + mc.e30*wc;
    m.e31 = ma.e31*wa + mb.e31*wb + mc.e31*wc;
    m.e32 = ma.e32*wa + mb.e32*wb + mc.e32*wc;

} // WeightAdd

void Skin3( const VertexW4* pSrc, VertexN* pDst, int nVert, const Mat4* pTM )
{
    Mat4 tm( Mat4::identity );
    for (int i = 0; i < nVert; i++)
    {
        const VertexW4& sv = pSrc[i];
        VertexN& dv = pDst[i];
        WeightAdd( tm, pTM[sv.boneIdx[0]], ByteToF( sv.weights[0] ), 
                       pTM[sv.boneIdx[1]], ByteToF( sv.weights[1] ), 
                       pTM[sv.boneIdx[2]], ByteToF( sv.weights[2] ) );

        dv.x = sv.x; dv.y = sv.y; dv.z = sv.z; 
        dv.nx = sv.nx; dv.ny = sv.ny; dv.nz = sv.nz; 
        dv.u = sv.u; dv.v = sv.v;

        tm.tmpt( *((Vec3*)&dv.x) );
        tm.tmvec( *((Vec3*)&dv.nx) );
    }
} // Skin3

void Skin4( const VertexW4* pSrc, VertexN* pDst, int nVert, const Mat4* pTM )
{
    for (int i = 0; i < nVert; i++)
    {
        const VertexW4& sv = pSrc[i];
        VertexN& dv = pDst[i];
        Mat4 tm1 = pTM[sv.boneIdx[0]];
        tm1 *= ByteToF( sv.weights[0] );
        Mat4 tm2 = pTM[sv.boneIdx[1]];
        tm2 *= ByteToF( sv.weights[1] );
        Mat4 tm3 = pTM[sv.boneIdx[2]];
        tm3 *= ByteToF( sv.weights[2] );
        Mat4 tm4 = pTM[sv.boneIdx[3]];
        tm4 *= ByteToF( sv.weights[3] );
        tm1 += tm2;
        tm1 += tm3;
        tm1 += tm4;

        dv.x = sv.x; dv.y = sv.y; dv.z = sv.z; 
        dv.nx = sv.nx; dv.ny = sv.ny; dv.nz = sv.nz; 
        dv.u = sv.u; dv.v = sv.v;

        tm1.tmpt( *((Vec3*)&dv.x) );
        tm1.tmvec( *((Vec3*)&dv.nx) );
    }
} // Skin4

void JMesh::ProcessSkin( JBoneInstance* pSkelInst, const Mat4& tm )
{
    const VertexW4* pSrc = (VertexW4*)(m_pModel->GetVBuf().GetData() + m_FirstVByte);
    VertexN*  pDst = &m_SkinnedVert[0];
    
    //  calculate transforms
    int nTM = m_BoneTM.size();
    for (int i = 0; i < nTM; i++)
    {
        m_BoneTM[i] = (m_BoneID[i] == -1) ? m_BoneOffsets[i] : 
                                            m_BoneOffsets[i]*pSkelInst[m_BoneID[i]].m_ModelTM;
    }

    //  put bone transforms into normalized mesh space
    Vec3 c = Vec3::null;
    Mat4 unnormTM;
    unnormTM.inverse( m_NormTM );
    for (int i = 0; i < nTM; i++)
    {
        m_BoneTM[i] = unnormTM*m_BoneTM[i];
        c += m_BoneTM[i].translation();
    }
    c *= 1.0f/float( nTM );
    m_ShiftTM.t( -c );
    for (int i = 0; i < nTM; i++)
    {
        m_BoneTM[i] = m_BoneTM[i]*m_ShiftTM;
    }
    m_ShiftTM.t( c );

    const Mat4* pTM = &m_BoneTM[0];
    int nClusters = m_SkinClusters.size();
    for (int i = 0; i < nClusters; i++)
    {
        JSkinCluster& cluster = m_SkinClusters[i];
        if (cluster.m_NWeights == 1)
        {
            Skin1( pSrc + cluster.m_FirstVert, pDst + cluster.m_FirstVert, cluster.m_NVert, pTM );
        }
        else if (cluster.m_NWeights == 2) 
        {
            Skin2( pSrc + cluster.m_FirstVert, pDst + cluster.m_FirstVert, cluster.m_NVert, pTM );
        }
        else if (cluster.m_NWeights == 3)
        {
            Skin3( pSrc + cluster.m_FirstVert, pDst + cluster.m_FirstVert, cluster.m_NVert, pTM );
        }
        else if (cluster.m_NWeights == 4)
        {
            Skin4( pSrc + cluster.m_FirstVert, pDst + cluster.m_FirstVert, cluster.m_NVert, pTM );
        }
    }
} // JMesh::ProcessSkin

void JMesh::Render( int firstIByte, JBoneInstance* pSkelInst, const Mat4& tm, bool bUseCachedData )
{
    if (!m_pModel) return;
    if (IsSkinned())
    {
        if (!bUseCachedData)
        {
            ProcessSkin( pSkelInst, tm );
        }
        
        const int stride = sizeof( VertexN );
        int numBytes = stride*m_NVertices;
        m_VBIteration = -1;
        Mat4 worldTM = m_ShiftTM*tm;
        g_pRenderServer->SetWorldTM( worldTM );
        g_pRenderServer->CacheVB( m_VBufID, (uint8_t*)&m_SkinnedVert[0], numBytes, stride, m_VBIteration, m_VBFirstByte );
        
        g_pRenderServer->SetVB( m_VBufID, VertexType_XYZNUV, 0 );
        int baseIndex = m_VBFirstByte/stride;
        g_pRenderServer->SetIB( m_IBID, baseIndex );
        g_pRenderServer->Draw( m_FirstIndex + firstIByte/2, m_NIndices, 
                   m_VBFirstByte/stride, m_NVertices, m_PrimitiveType );
    }
    else
    {
        g_pRenderServer->SetWorldTM( tm );
        g_pRenderServer->SetVB( m_VBufID, m_VDecl.m_TypeID );
        int baseIndex = m_VBFirstByte/m_VDecl.m_VertexSize;
        g_pRenderServer->SetIB( m_IBID, baseIndex );

        g_pRenderServer->Draw(  m_FirstIndex + firstIByte/2, 
                    m_NIndices, 
                    m_VBFirstByte/m_VDecl.m_VertexSize, 
                    m_NVertices, 
                    m_PrimitiveType );
    }
} // JMesh::Render

void JMesh::Init()
{
    if (!g_pRenderServer) return;

    int vertTypeID = g_pRenderServer->GetVDeclID( m_VDecl );
    if (vertTypeID >= 0)
    {
        m_VDecl.m_TypeID = vertTypeID;
    }

    m_IBID = g_pRenderServer->GetIBID( "DrawSystemStatic" );
    
    AABox aabb;
    aabb.Invalidate();
    if (m_VDecl.m_TypeID == VertexType_XYZNUVW4 && m_pModel)
    {
        //  create skin clusters
        VertexW4* pVert = (VertexW4*)(m_pModel->GetVBuf().GetData() + m_FirstVByte);
        m_SkinClusters.clear();
        int curNW = -1;
        int startVert = 0;
        for (int i = 0; i <= m_NVertices; i++)
        {
            if (i != m_NVertices) 
            {
                aabb.AddPoint( Vec3( pVert[i].x, pVert[i].y, pVert[i].z ) );
                if (pVert[i].GetNBones() == curNW) continue;
            }
            if (i > startVert)
            {
                int nClusterVert = i - startVert;
                m_SkinClusters.push_back( JSkinCluster( startVert, nClusterVert, curNW ) );
            }
            if (i == m_NVertices) break;
            curNW = pVert[i].GetNBones();
            startVert = i;
        }

        //  normalize mesh point coordinates
        Vec3 sc = aabb.GetExt();

        //  ensure none of the sides is zero
        float scMax = tmax( sc.x, sc.y, sc.z );
        if (sc.x < c_FltEpsilon) sc.x = scMax;
        if (sc.y < c_FltEpsilon) sc.y = scMax;
        if (sc.z < c_FltEpsilon) sc.z = scMax;

        sc.x = 2.0f/sc.x;
        sc.y = 2.0f/sc.y;
        sc.z = 2.0f/sc.z;
        Vec3 c  = aabb.GetCenter();
        Mat4 normTM = Mat4::identity;
        normTM.translation() = -c;
        Mat4 scTM;
        scTM.st( sc, Vec3::null );
        normTM *= scTM;
        for (int i = 0; i < m_NVertices; i++)
        {
            Vec3& pos  = *((Vec3*)&pVert[i]);
            Vec3& norm = *((Vec3*)&pVert[i].nx);
            normTM.tmpt( pos );
            normTM.tmvec( norm );
        }
        
        m_NormTM *= normTM;


        m_VBufID = g_pRenderServer->GetVBID( "DrawSystemDynamic" );
        m_SkinnedVert.resize( m_NVertices );
        m_AABB = aabb;
    }
    else
    {
        m_VBufID = g_pRenderServer->GetVBID( "DrawSystemStatic" );
    }
} // JMesh::Init

void JMesh::CacheVB()
{
    if (IsSkinned() || !m_pModel) return;
    uint8_t* pVert = m_pModel->GetVBuf().GetData() + m_FirstVByte;
    int stride = m_VDecl.m_VertexSize;
    g_pRenderServer->CacheVB( m_VBufID, pVert, m_NVertices*stride, stride, m_VBIteration, m_VBFirstByte );
} // JMesh::CacheVB

void JMesh::SetVertexRange( int firstByte, int numVert ) 
{ 
    m_FirstVByte = firstByte; 
    m_NVertices = numVert; 
} // JMesh::SetVertexRange

void JMesh::SetIndexRange( int firstIdx, int numIdx ) 
{
    m_FirstIndex = firstIdx;
    m_NIndices   = numIdx;
} // JMesh::SetIndexRange

bool JMesh::AddSkinCluster( int firstV, int numV, int numWeights )
{
    m_SkinClusters.push_back( JSkinCluster( firstV, numV, numWeights ) );
    return true;
} // JMesh::AddSkinCluster

int JMesh::FindBoneInstance( const char* boneName, JBoneInstance* pBones, int nBones ) const
{
    for (int i = 0; i < nBones; i++)
    {
        if (!stricmp( pBones[i].m_pBone->GetName(), boneName )) return i;
    }
    return -1;
} // JMesh::FindBoneInstance

bool JMesh::LinkBones( JBoneInstance* pBones, int nBones )
{
    m_HostBoneID = FindBoneInstance( GetHostBone(), pBones, nBones );
    m_BoneID.clear();
    m_BoneOffsets.clear();
    
    const Mat4& hostTM = (m_HostBoneID == -1) ? Mat4::identity : pBones[m_HostBoneID].m_ModelTM;

    int nSkinBones = m_BoneList.size();
    if (nSkinBones > 0)
    {
        m_BoneID.clear(); 
        for (int i = 0; i < nSkinBones; i++)
        {
            int boneID = FindBoneInstance( m_BoneList[i].c_str(), pBones, nBones );

            m_BoneID.push_back( boneID );
            Mat4 tm = Mat4::identity;
            if (boneID != -1)
            {
                tm.inverse( pBones[boneID].m_ModelTM );
                //  after inversion transform matrix is not guaranteed to have 1.0 at lower left corner,
                //  so we normalize it
                tm *= 1.0f/tm.e33; 
            }
            Mat4 offsTM = hostTM*tm;
            m_BoneOffsets.push_back( offsTM );
        }
    }
    m_BoneTM.resize( m_BoneID.size() );
    return (m_HostBoneID != -1);
} // JMesh::LinkBones

bool JMesh::GetComponents( Buffer& buf, VertexComponent component ) const
{
    if (!m_pModel) return false;
    if (m_NVertices == 0)
    {
        buf.Resize( 0 );
        return true;
    }
    int elemIdx = m_VDecl.GetElementIdx( component );
    if (elemIdx == -1) return false;
    const Buffer& vbuf = m_pModel->GetVBuf();
    if (vbuf.GetSize() < m_FirstVByte + m_NVertices*m_VDecl.m_VertexSize)
    {
        return false;
    }
    const VertexElement& vElem = m_VDecl.m_Element[elemIdx];
    int compSize = GetVertexElementSize( vElem.m_Type );
    if (compSize == 0) return false;
    const uint8_t* pVert = vbuf.GetData() + m_FirstVByte + vElem.m_Offset;
    buf.Resize( m_NVertices*compSize );
    uint8_t* pDst = buf.GetData();
    int vertSize = m_VDecl.m_VertexSize;
    for (int i = 0; i < m_NVertices; i++)
    {
        memcpy( pDst, pVert, compSize );
        pDst  += compSize;
        pVert += vertSize;
    }
    return true;
} // JMesh::GetComponents

bool JMesh::GetIndices( Buffer& buf ) const
{
    if (!m_pModel) return false;
    if (m_NIndices == 0)
    {
        buf.Resize( 0 );
        return true;
    }
    const Buffer& ibuf = m_pModel->GetIBuf();
    if (ibuf.GetSize() < (m_FirstIndex + m_NIndices)*2)
    {
        return false;
    }
    buf.Resize( m_NIndices*2 );
    memcpy( buf.GetData(), ibuf.GetData() + m_FirstIndex*2, m_NIndices*2 );
    return true;
} // JMesh::GetIndices

bool JMesh::SetIndices( const Buffer& buf )
{
    if (!m_pModel) return false;
    Buffer& ibuf = m_pModel->GetIBuf();
    if (ibuf.GetSize() < (m_FirstIndex + m_NIndices)*2 || buf.GetSize() < m_NIndices*2)
    {
        return false;
    }
    memcpy( ibuf.GetData() + m_FirstIndex*2, buf.GetData(), m_NIndices*2 );
    return true;
} // JMesh::SetIndices