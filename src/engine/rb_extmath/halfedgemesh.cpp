//****************************************************************************/
//  File:	HalfEdgeMesh.cpp
//  Desc:	Implementation of class HalfEdgeMesh
/*	Author:	Ruslan Shestopalyuk
/*	Date:	17-02-2006
//****************************************************************************/
#include "stdafx.h"
#include "HalfEdgeMesh.h"
#include "Voronoi.h"
#include <algorithm>

//****************************************************************************/
/*  HalfEdgeMesh implementation
//****************************************************************************/
bool HalfEdgeMesh::CreateVoronoi( const Vec2* sites, int nSites, const Frame& bounds )
{
    const float c_WeldEpsilon = 1e-3f;
    Clear();
    Voronoi voronoi;
    voronoi.Generate( sites, nSites, bounds );
    Vec2 a, b;
    while (voronoi.GetNextEdge( a, b ))
    {
        if( a.dist2( b ) < c_WeldEpsilon) continue;
        int hA = GetVertID( a, c_WeldEpsilon );
        int hB = GetVertID( b, c_WeldEpsilon );
        if (hA == 0) hA = AddVert( a );
        if (hB == 0) hB = AddVert( b );
        AddEdge( hA, hB );
    }
    //  add corner vertices
    Vec2 lt( bounds.x,   bounds.y   );
    Vec2 rt( bounds.r(), bounds.y   );
    Vec2 lb( bounds.x,   bounds.b() );
    Vec2 rb( bounds.r(), bounds.b() );
    int hlt = AddVert( lt );
    int hrt = AddVert( rt );
    int hlb = AddVert( lb );
    int hrb = AddVert( rb );

    //  find all border points
    std::vector<HESVert*> lPt;
    std::vector<HESVert*> rPt;
    std::vector<HESVert*> tPt;
    std::vector<HESVert*> bPt;
    HESVert* pVert = m_pVerts;
    while (pVert)
    {
        const Vec2& pt = pVert->m_Pos;
        if (_fabs( pt.x - lt.x ) < c_WeldEpsilon) lPt.push_back( pVert );
        if (_fabs( pt.x - rt.x ) < c_WeldEpsilon) rPt.push_back( pVert );
        if (_fabs( pt.y - lt.y ) < c_WeldEpsilon) tPt.push_back( pVert );
        if (_fabs( pt.y - lb.y ) < c_WeldEpsilon) bPt.push_back( pVert );
        pVert = pVert->m_pNextSibling;
    }
    std::sort( lPt.begin(), lPt.end(), HESVert::CompareByY );
    std::sort( rPt.begin(), rPt.end(), HESVert::CompareByY );
    std::sort( tPt.begin(), tPt.end(), HESVert::CompareByX );
    std::sort( bPt.begin(), bPt.end(), HESVert::CompareByX );
    for (int i = 1; i < lPt.size(); i++)
    {
        AddEdge( reinterpret_cast<int>( lPt[i - 1] ), 
                 reinterpret_cast<int>( lPt[i] ) );
    }
    for (int i = 1; i < rPt.size(); i++)
    {
        AddEdge( reinterpret_cast<int>( rPt[i - 1] ), 
                 reinterpret_cast<int>( rPt[i] ) );
    }
    for (int i = 1; i < tPt.size(); i++)
    {
        AddEdge( reinterpret_cast<int>( tPt[i - 1] ), 
                 reinterpret_cast<int>( tPt[i] ) );
    }
    for (int i = 1; i < bPt.size(); i++)
    {
        AddEdge( reinterpret_cast<int>( bPt[i - 1] ), 
                 reinterpret_cast<int>( bPt[i] ) );
    }
    FindFaces();
    return true;
} // HalfEdgeMesh::CreateVoronoi

bool HalfEdgeMesh::GetEdge( int hEdge, Vec2& a, Vec2& b ) const
{
    if (hEdge == 0) return false;
    HESEdge* pEdge = reinterpret_cast<HESEdge*>( hEdge );
    a = pEdge->m_pSourceVert->m_Pos;
    b = pEdge->m_pOpposite->m_pSourceVert->m_Pos;
    return true;
} // HalfEdgeMesh::GetEdge

int HalfEdgeMesh::GetNextEdge( int hEdge ) const
{
    if (hEdge == 0) return 0;
    HESEdge* pEdge = reinterpret_cast<HESEdge*>( hEdge );
    return reinterpret_cast<int>( pEdge->m_pNextSibling );
} // HalfEdgeMesh::GetNextEdge

bool HalfEdgeMesh::GetFace( int hFace, PolyLine2& poly ) const
{
    if (hFace == 0) return false;
    HESFace* pFace = reinterpret_cast<HESFace*>( hFace );
    poly.Clear();
    HESEdge* pEdge = pFace->m_pFirstEdge;
    if (!pEdge) return false;
    do
    {
        poly.AddPoint( pEdge->m_pSourceVert->m_Pos, false );
        pEdge = pEdge->m_pOutEdge;
    }
    while (pEdge && pEdge != pFace->m_pFirstEdge);
    return true;
} // HalfEdgeMesh::GetFace

int HalfEdgeMesh::GetNextFace( int hFace ) const
{
    if (hFace == 0) return 0;
    HESFace* pFace = reinterpret_cast<HESFace*>( hFace );
    return reinterpret_cast<int>( pFace->m_pNextSibling );
} // HalfEdgeMesh::GetNextFace

Vec2 HalfEdgeMesh::GetVert( int hVert ) const
{
    if (hVert == 0) return Vec2::null;
    HESVert* pVert = reinterpret_cast<HESVert*>( hVert );
    return pVert->m_Pos;
} // HalfEdgeMesh::GetVert

int HalfEdgeMesh::GetNextVert( int hVert ) const
{
    if (hVert == 0) return 0;
    HESVert* pVert = reinterpret_cast<HESVert*>( hVert );
    return reinterpret_cast<int>( pVert->m_pNextSibling );
} // HalfEdgeMesh::GetNextVert

int HalfEdgeMesh::GetVertID( const Vec2& v, float bias ) const
{
    const float bias2 = bias*bias;
    HESVert* pVert = m_pVerts;
    while (pVert)
    {
        if (pVert->m_Pos.dist2( v ) < bias2) return reinterpret_cast<int>( pVert );
        pVert = pVert->m_pNextSibling;
    }
    return 0;
} // HalfEdgeMesh::GetVertID

int HalfEdgeMesh::AddFace()
{
    HESFace* pFace = m_FacePool.Allocate();
    pFace->m_pNextSibling = m_pFaces;
    pFace->m_pPrevSibling = NULL;
    if (m_pFaces) m_pFaces->m_pPrevSibling = pFace;
    m_pFaces = pFace;
    return reinterpret_cast<int>( pFace );
} // HalfEdgeMesh::AddFace

int HalfEdgeMesh::AddVert( const Vec2& v )
{
    HESVert* pVert = m_VertPool.Allocate();
    pVert->m_pNextSibling    = m_pVerts;
    pVert->m_pPrevSibling    = NULL;
    if (m_pVerts) m_pVerts->m_pPrevSibling = pVert;
    pVert->m_pOutEdge        = NULL;
    pVert->m_Pos             = v;
    m_pVerts = pVert;
    return reinterpret_cast<int>( pVert );
} // HalfEdgeMesh::AddVert

int HalfEdgeMesh::AddEdge()
{
    HESEdge* pEdge = m_EdgePool.Allocate();
    pEdge->m_pNextSibling    = m_pEdges;
    pEdge->m_pPrevSibling    = NULL;
    if (m_pEdges) m_pEdges->m_pPrevSibling = pEdge;
    pEdge->m_pFace           = NULL;
    pEdge->m_pOpposite       = NULL;
    pEdge->m_pOutEdge        = NULL;
    m_pEdges = pEdge;
    return reinterpret_cast<int>( pEdge );
} // HalfEdgeMesh::AddEdge

int HalfEdgeMesh::AddEdge( int va, int vb )
{
    if (va == 0 || vb == 0) return 0;
    HESVert* pVertA = reinterpret_cast<HESVert*>( va );
    HESVert* pVertB = reinterpret_cast<HESVert*>( vb );
    HESEdge* pEdge1 = m_EdgePool.Allocate();
    HESEdge* pEdge2 = m_EdgePool.Allocate();

    if (m_pEdges) m_pEdges->m_pPrevSibling = pEdge2;
    pEdge2->m_pPrevSibling   = pEdge1;
    pEdge1->m_pPrevSibling   = NULL;

    pEdge1->m_pNextSibling  = pEdge2;
    pEdge2->m_pNextSibling  = m_pEdges;
    m_pEdges = pEdge1;

    pEdge1->m_pSourceVert   = pVertA;
    pEdge2->m_pSourceVert   = pVertB;

    pEdge1->m_pFace         = NULL;
    pEdge2->m_pFace         = NULL;

    pEdge1->m_pOpposite     = pEdge2;
    pEdge2->m_pOpposite     = pEdge1;

    const Vec2& a = pVertA->m_Pos;
    const Vec2& b = pVertB->m_Pos;
    Vec2 ab = b - pVertA->m_Pos;
    Vec2 ba = -ab;
    ab.normalize();

    if (!pVertB->m_pOutEdge)
    {
        pVertB->m_pOutEdge  = pEdge2;
        pEdge1->m_pOutEdge  = pEdge2;
    }
    else
    {
        //  find outedge for the forward edge
        HESEdge* pStartEdge = pVertB->m_pOutEdge;
        pEdge1->m_pOutEdge  = pStartEdge;
        HESEdge* pCurEdge   = pStartEdge->GetNext1RingEdge();
        while (pCurEdge != pStartEdge)
        {
            Vec2 c = pEdge1->m_pOutEdge->GetEndVert()->m_Pos;
            Vec2 d = pCurEdge->GetEndVert()->m_Pos;
            Vec2 bc = c - b;
            Vec2 bd = d - b;
            if (ba.angle( bd ) < ba.angle( bc )) 
            {
                pEdge1->m_pOutEdge = pCurEdge;
            }
            pCurEdge = pCurEdge->GetNext1RingEdge();
            assert( pCurEdge );
        }
        //  redirect one's who pointed at out edge before
        pEdge1->m_pOutEdge->GetReferrer()->m_pOutEdge = pEdge2;
    }

    if (!pVertA->m_pOutEdge)
    {
       pVertA->m_pOutEdge   = pEdge1;
       pEdge2->m_pOutEdge   = pEdge1;
    }
    else
    {
        //  find outedge for the backward edge
        HESEdge* pStartEdge = pVertA->m_pOutEdge;
        pEdge2->m_pOutEdge  = pStartEdge;
        HESEdge* pCurEdge   = pStartEdge->GetNext1RingEdge();
        while (pCurEdge != pStartEdge)
        {
            Vec2 c = pEdge2->m_pOutEdge->GetEndVert()->m_Pos;
            Vec2 d = pCurEdge->GetEndVert()->m_Pos;
            Vec2 ac = c - a;
            Vec2 ad = d - a;
            if (ab.angle( ad ) < ab.angle( ac )) 
            {
                pEdge2->m_pOutEdge = pCurEdge;
            }
            pCurEdge = pCurEdge->GetNext1RingEdge();
            assert( pCurEdge );
        }
        //  redirect one's who pointed at out edge before
        pEdge2->m_pOutEdge->GetReferrer()->m_pOutEdge = pEdge1;
    }

    return reinterpret_cast<int>( pEdge1 );
} // HalfEdgeMesh::AddEdge

void HalfEdgeMesh::Clear()
{
    m_VertPool.Clear();
    m_EdgePool.Clear();
    m_FacePool.Clear();
    m_pVerts = NULL;
    m_pEdges = NULL;
    m_pFaces = NULL;
} // HalfEdgeMesh::Clear

void HalfEdgeMesh::FindFaces()
{
    m_FacePool.Clear();
    m_pFaces = NULL;
    HESEdge* pEdge = m_pEdges;
    while (pEdge)
    {
        if (pEdge->m_pFace == NULL)
        {
            HESFace* pFace = reinterpret_cast<HESFace*>( AddFace() );
            pFace->m_pFirstEdge = pEdge;
            do
            {
                pEdge->m_pFace = pFace;
                pEdge = pEdge->m_pOutEdge;
            }
            while (pEdge != pFace->m_pFirstEdge);
        }
        pEdge = pEdge->m_pNextSibling;
    }
} // HalfEdgeMesh::FindFaces

bool HalfEdgeMesh::PointInFace( int faceID, const Vec2& pt ) const
{
    if (faceID == 0) return false;
    HESFace* pFace = reinterpret_cast<HESFace*>( faceID );
    HESEdge* pEdge = pFace->m_pFirstEdge;
    int nWindings = 0;
    do
    {
        const Vec2& a = pEdge->m_pSourceVert->m_Pos;
        const Vec2& b = pEdge->m_pOpposite->m_pSourceVert->m_Pos;
        if (a.y <= pt.y)
        {
            if (b.y > pt.y)
            {
                if (pt.leftof( a, b ) > 0.0f) nWindings++;
            }
        }
        else
        {
            if (b.y <= pt.y)
            {
                if (pt.leftof( a, b ) < 0.0f) nWindings--;
            }
        }
        pEdge = pEdge->m_pOutEdge;
    }while (pEdge != pFace->m_pFirstEdge);
    return (nWindings > 0);
} // PointInFace

int HalfEdgeMesh::PickFace( const Vec2& pt ) const
{
    const HESFace* pFace = m_pFaces;
    while (pFace)
    {
        int faceID = reinterpret_cast<int>( pFace );
        if (PointInFace( faceID, pt )) return faceID;
        pFace = pFace->m_pNextSibling;
    }
    return 0;
} // HalfEdgeMesh::PickPoly

int HalfEdgeMesh::GetNFaceEdges( int faceID ) const
{
    FaceCirculator fc( faceID );
    int nEdges = 0;
    while (fc) { nEdges++; ++fc; }
    return nEdges;
} // HalfEdgeMesh::GetNFaceEdges

bool HalfEdgeMesh::MergeFaces( int fa, int fb )
{
    HESFace* pFaceA = reinterpret_cast<HESFace*>( fa );
    HESFace* pFaceB = reinterpret_cast<HESFace*>( fb );
    if (pFaceA == pFaceB || !pFaceA || !pFaceB) 
    {
        return false;
    }
    
    //  find edge chain to delete
    bool bFound = false;
    int nEdges = GetNFaceEdges( fa );
    HESEdge* pCurEdge = pFaceA->m_pFirstEdge;

    FaceCirculator bfc( fb );
    while (bfc)
    {
        HESFace* pFace = bfc.GetEdge()->m_pOpposite->m_pFace;
        if (pFace == pFaceA) 
        {
            pCurEdge = bfc.GetEdge()->m_pOpposite;
            bFound = true;
        }
        ++bfc;
    }
    if (!bFound) 
    {
        return false;
    }
    for (int i = 0; i < nEdges; i++)
    {
        if (pCurEdge->m_pOpposite->m_pFace != pFaceB) break;
        pCurEdge = pCurEdge->m_pOutEdge;
    }

    bFound = false;
    for (int i = 0; i < nEdges; i++)
    {
        HESEdge* pEdgeA = pCurEdge; 
        HESEdge* pEdgeB = pCurEdge->m_pOpposite; 
        pCurEdge = pCurEdge->m_pOutEdge;
        HESFace* pFace = pEdgeB->m_pFace;
        if (pFace != pFaceB) 
        {
            continue;
        }        
        assert( pFace != pFaceA );
        
        //  reassign outedges
        HESEdge* pPrevA       = pEdgeA->GetReferrer();
        HESEdge* pPrevB       = pEdgeB->GetReferrer();

        pPrevB->m_pOutEdge    = pEdgeA->m_pOutEdge;
        pPrevA->m_pOutEdge    = pEdgeB->m_pOutEdge;

        if (pPrevA == pEdgeB)
        {
            DestroyVert( pEdgeA->m_pSourceVert );
        }
        else
        {
            pEdgeA->m_pSourceVert->m_pOutEdge = pEdgeB->m_pOutEdge;
        }

        if (pPrevB == pEdgeA)
        {
            DestroyVert( pEdgeB->m_pSourceVert );
        }
        else
        {
            pEdgeB->m_pSourceVert->m_pOutEdge = pEdgeA->m_pOutEdge;
        }
        // reassign face's edge pointer, if needed
        if (pEdgeA == pFaceA->m_pFirstEdge)
        {
            pFaceA->m_pFirstEdge = pEdgeB->m_pOutEdge;
            if (pEdgeB->m_pOutEdge == pEdgeA)
            {
                // baaad...seek whole mesh to find the edge which can help
                HESEdge* pRefEdge = m_pEdges;
                while (pRefEdge)
                {
                    if (pRefEdge->m_pFace == pFaceA) 
                    {
                        if (pRefEdge != pEdgeA) 
                        {
                            pFaceA->m_pFirstEdge = pRefEdge;
                            break;
                        }
                    }
                    pRefEdge = pRefEdge->m_pNextSibling;
                }
            }
        }
        DestroyEdge( pEdgeA );
        DestroyEdge( pEdgeB );
        bFound = true;
    }
    
    DestroyFace( pFaceB );
    //  reassign references from deleted face
    HESEdge* pEdge = pFaceA->m_pFirstEdge;
    HESEdge* pStartEdge = pEdge;
    do
    {
        pEdge->m_pFace = pFaceA;
        pEdge = pEdge->m_pOutEdge;
    }while (pEdge != pStartEdge);
    return true;
} // HalfEdgeMesh::MergeFaces

void HalfEdgeMesh::DestroyFace( HESFace* pFace )
{
    if (m_pFaces == pFace) m_pFaces = pFace->m_pNextSibling;
    if (pFace->m_pPrevSibling) pFace->m_pPrevSibling->m_pNextSibling = pFace->m_pNextSibling;
    if (pFace->m_pNextSibling) pFace->m_pNextSibling->m_pPrevSibling = pFace->m_pPrevSibling;
    m_FacePool.Free( pFace );
} // HalfEdgeMesh::DestroyFace

void HalfEdgeMesh::DestroyEdge( HESEdge* pEdge )
{
    if (m_pEdges == pEdge) m_pEdges = pEdge->m_pNextSibling;
    if (pEdge->m_pPrevSibling) pEdge->m_pPrevSibling->m_pNextSibling = pEdge->m_pNextSibling;
    if (pEdge->m_pNextSibling) pEdge->m_pNextSibling->m_pPrevSibling = pEdge->m_pPrevSibling;
    m_EdgePool.Free( pEdge );
} // HalfEdgeMesh::DestroyEdge

void HalfEdgeMesh::DestroyVert( HESVert* pVert )
{
    if (m_pVerts == pVert) m_pVerts = pVert->m_pNextSibling;
    if (pVert->m_pPrevSibling) pVert->m_pPrevSibling->m_pNextSibling = pVert->m_pNextSibling;
    if (pVert->m_pNextSibling) pVert->m_pNextSibling->m_pPrevSibling = pVert->m_pPrevSibling;
    m_VertPool.Free( pVert );
} // HalfEdgeMesh::DestroyVert

bool HalfEdgeMesh::IsConsistent() const
{
    int nEdges = m_EdgePool.GetNElem();
    int nFaces = m_FacePool.GetNElem();
    int nVerts = m_VertPool.GetNElem();

    if (nEdges&1) return false;

    //  check Euler's formula
    if (nVerts - nEdges/2 + nFaces != 2) 
    {
        return false;
    }
    //  check edges
    HESEdge* pEdge = m_pEdges;
    while (pEdge)
    {
        HESEdge* pOpposite = pEdge->m_pOpposite;
        if (!pOpposite || pOpposite->m_pOpposite != pEdge) return false;
        if (!pEdge->m_pOutEdge) return false;
        if (pEdge->m_pPrevSibling && pEdge->m_pPrevSibling->m_pNextSibling != pEdge) return false;
        pEdge = pEdge->m_pNextSibling;
        nEdges--;
    }
    if (nEdges != 0) return false;

    //  check faces
    HESFace* pFace = m_pFaces;
    while (pFace)
    {
        HESEdge* pStartEdge = pFace->m_pFirstEdge;
        HESEdge* pCurEdge = pStartEdge;
        do
        {
            if (pCurEdge->m_pFace != pFace) return false;
            pCurEdge = pCurEdge->m_pOutEdge;
        }
        while (pCurEdge != pStartEdge);
        pFace = pFace->m_pNextSibling;
    }
    return true;
} // HalfEdgeMesh::IsConsistent

int HalfEdgeMesh::SplitEdge( int hEdge, float t )
{
    if (hEdge == 0) return 0;
    HESEdge* pEdgeA1 = reinterpret_cast<HESEdge*>( hEdge );
    HESEdge* pEdgeB2 = pEdgeA1->m_pOpposite;
    
    HESVert* pVertA  = pEdgeA1->m_pSourceVert; 
    HESVert* pVertB  = pEdgeB2->m_pSourceVert; 
    Vec2 a = pVertA->m_Pos;
    Vec2 b = pVertB->m_Pos;

    HESVert* pVert   = reinterpret_cast<HESVert*>( AddVert( a + t*(b - a) ) );
    HESEdge* pEdgeA2 = reinterpret_cast<HESEdge*>( AddEdge() );
    HESEdge* pEdgeB1 = reinterpret_cast<HESEdge*>( AddEdge() );

    pVert->m_pOutEdge = pEdgeA2;
    
    pEdgeA2->m_pOutEdge = pEdgeA1->m_pOutEdge;
    pEdgeB1->m_pOutEdge = pEdgeB2->m_pOutEdge;

    pEdgeA2->m_pFace = pEdgeA1->m_pFace;
    pEdgeB1->m_pFace = pEdgeB2->m_pFace;

    pEdgeA2->m_pSourceVert = pVert;
    pEdgeB1->m_pSourceVert = pVert;

    pEdgeA1->m_pOpposite = pEdgeB1;
    pEdgeA2->m_pOpposite = pEdgeB2;
    pEdgeB1->m_pOpposite = pEdgeA1;
    pEdgeB2->m_pOpposite = pEdgeA2;

    pEdgeA1->m_pOutEdge = pEdgeA2;
    pEdgeB2->m_pOutEdge = pEdgeB1;
    
    return reinterpret_cast<int>( pVert );
} // HalfEdgeMesh::SplitEdge

void HalfEdgeMesh::FractalizeEdge( int hEdge, float minLen, float magnitude, float uniformity )
{
    if (hEdge == 0) return;
    HESEdge* pEdge = reinterpret_cast<HESEdge*>( hEdge );
    float len = pEdge->GetLength();
    if (len <= minLen) return;

    //  find new vertex position
    float dt = 0.5*(1.0f - uniformity);
    dt = clamp( dt, 0.0f, 0.5f );
    float t = frand( 0.5f - dt, 0.5f + dt );
    float shift = frand( -magnitude, magnitude );
    const Vec2& a = pEdge->m_pSourceVert->m_Pos;
    const Vec2& b = pEdge->m_pOpposite->m_pSourceVert->m_Pos;
    Vec2 dir = pEdge->GetDir().normal();
    Vec2 c = a + t*(b - a) + dir*shift;

    //  check whether this new position is valid
    bool bValid = true;
    HESEdge* pCurEdge = pEdge->m_pOutEdge;
    Seg2 ac( a, c );
    Seg2 bc( b, c );
    while (pCurEdge != pEdge)
    {
        Vec2 ea, eb;
        pCurEdge->GetPoints( ea, eb );
        Seg2 seg( ea, eb );
        if (seg.intersects( ac, false ) || seg.intersects( bc, false )) 
        {
            bValid = false;
        }
        pCurEdge = pCurEdge->m_pOutEdge;
    }

    pCurEdge = pEdge->m_pOpposite->m_pOutEdge;
    while (pCurEdge != pEdge->m_pOpposite)
    {
        Vec2 ea, eb;
        pCurEdge->GetPoints( ea, eb );
        Seg2 seg( ea, eb );
        if (seg.intersects( ac, false ) || seg.intersects( bc, false )) 
        {
            bValid = false;
        }
        pCurEdge = pCurEdge->m_pOutEdge;
    }
    
    //  insert new vertex
    int vertID = SplitEdge( hEdge, t );
    HESVert* pVert = reinterpret_cast<HESVert*>( vertID );
    if (bValid)
    {
        pVert->m_Pos = c;
    }

    //  fractalize sub-edges
    FractalizeEdge( hEdge, minLen, magnitude, uniformity );
    FractalizeEdge( reinterpret_cast<int>( pVert->m_pOutEdge ), minLen, magnitude, uniformity );
} // HalfEdgeMesh::FractalizeEdge

int HalfEdgeMesh::GetNEdgeOuts( int hEdge ) const
{
    HESEdge* pEdge = reinterpret_cast<HESEdge*>( hEdge );
    if (!pEdge) return 0;
    HESEdge* pCurEdge = pEdge->m_pOutEdge;
    int nOuts = 0;
    do
    {
        nOuts++;
        pCurEdge = pCurEdge->m_pOpposite->m_pOutEdge;
    }while (pCurEdge != pEdge->m_pOutEdge);
    return nOuts;
} // HalfEdgeMesh::GetNEdgeOuts

int HalfEdgeMesh::GetEdgeOutID( int hEdge, int outIdx ) const
{
    HESEdge* pEdge = reinterpret_cast<HESEdge*>( hEdge );
    if (!pEdge) return 0;
    HESEdge* pCurEdge = pEdge->m_pOutEdge;
    for (int i = 0; i < outIdx; i++)
    {
        pCurEdge = pCurEdge->m_pOpposite->m_pOutEdge;
    }
    return reinterpret_cast<int>( pCurEdge );
} // HalfEdgeMesh::GetEdgeOutID

int HalfEdgeMesh::ExtrudeEdge( int hEdge, float dist )
{
    HESEdge* pEdgeAB = reinterpret_cast<HESEdge*>( hEdge );
    if (!pEdgeAB) return 0;
    HESEdge* pEdgeBA = pEdgeAB->m_pOpposite;
    Vec2 a, b;
    pEdgeAB->GetPoints( a, b );
    Vec2 dir = b - a;
    dir.normalize();
    dir = dir.normal();
    HESEdge* pEdgeC = pEdgeBA->m_pOutEdge;
    HESEdge* pEdgeD = pEdgeBA->GetReferrer()->m_pOpposite;

    //  find distance projection on the edges
    Vec2 c = pEdgeC->m_pOpposite->m_pSourceVert->m_Pos;
    Vec2 d = pEdgeD->m_pOpposite->m_pSourceVert->m_Pos;
    Vec2 ac = c - a;
    Vec2 bd = d - b;
    float ct = ac.dot( dir );
    float dt = bd.dot( dir );
    if (_fabs( ct ) < c_FltEpsilon) return 0;
    if (_fabs( dt ) < c_FltEpsilon) return 0;
    ct = dist/ct;
    dt = dist/dt;
    if (ct >= 1.0f || ct <= 0.0f) return 0;
    if (dt >= 1.0f || dt <= 0.0f) return 0;

    //  perform extruding
    int vA = SplitEdge( reinterpret_cast<int>( pEdgeC ), ct );
    int vB = SplitEdge( reinterpret_cast<int>( pEdgeD ), dt );
    HESEdge* pNewEdge = reinterpret_cast<HESEdge*>( AddEdge( vA, vB ) );
    return reinterpret_cast<int>( pNewEdge->m_pFace );
} // HalfEdgeMesh::ExtrudeEdge

bool HalfEdgeMesh::SplitVertex( int edgeA, int edgeB, int& edgeNA, int& edgeNB, int& vertID, int& faceID )
{
    return false;
} // HalfEdgeMesh::SplitVertex


int HalfEdgeMesh::GetOuterFaceID() const
{
    return 0;
} // HalfEdgeMesh::GetOuterFaceID