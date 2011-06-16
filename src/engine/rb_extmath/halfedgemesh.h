//****************************************************************************/
//  File:   HalfEdgeMesh.h
//  Desc:
/*  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __HALFEDGEMESH_H__
#define __HALFEDGEMESH_H__

#include "vec2.h"
#include "frame.h"
#include "polyline2.h"
#include "uniformpool.h"

struct HESEdge;
struct HESFace;

//****************************************************************************/
//  Class:  HESVert
//  Desc:
//****************************************************************************/
struct HESVert
{
    Vec2        m_Pos;
    HESEdge*    m_pOutEdge;

    HESVert*    m_pNextSibling;
    HESVert*    m_pPrevSibling;

    HESVert() {}

    static bool CompareByX( const HESVert* pA, const HESVert* pB)
    {
        return (pA->m_Pos.x < pB->m_Pos.x);
    }

    static bool CompareByY( const HESVert* pA, const HESVert* pB)
    {
        return (pA->m_Pos.y < pB->m_Pos.y);
    }
}; // struct HESVert

//****************************************************************************/
//  Class:  HESEdge
//  Desc:
//****************************************************************************/
struct HESEdge
{
    HESVert*    m_pSourceVert;
    HESEdge*    m_pOpposite;
    HESEdge*    m_pOutEdge;
    HESFace*    m_pFace;

    HESEdge*    m_pNextSibling;
    HESEdge*    m_pPrevSibling;

    HESEdge() {}

    inline HESVert* GetEndVert() const { return m_pOpposite->m_pSourceVert; }
    inline HESEdge* GetNext1RingEdge() const { return m_pOpposite->m_pOutEdge; }
    HESEdge*        GetReferrer() const
    {
        const HESEdge* pEdge = this;
        do
        {
            if (pEdge->m_pOpposite->m_pOutEdge == this)
            {
                return pEdge->m_pOpposite;
            }
            pEdge = pEdge->m_pOpposite->m_pOutEdge;
        }while (pEdge != this);
        return NULL;
    }

    float GetLength() const
    {
        return m_pSourceVert->m_Pos.dist( m_pOpposite->m_pSourceVert->m_Pos );
    }

    Vec2 GetDir() const
    {
        return m_pOpposite->m_pSourceVert->m_Pos - m_pSourceVert->m_Pos;
    }

    void GetPoints( Vec2& a, Vec2& b )
    {
        a = m_pSourceVert->m_Pos;
        b = m_pOpposite->m_pSourceVert->m_Pos;
    }
}; // struct HESEdge

//****************************************************************************/
//  Class:  HESFace
//  Desc:
//****************************************************************************/
struct HESFace
{
    HESEdge*    m_pFirstEdge;

    HESFace*    m_pNextSibling;
    HESFace*    m_pPrevSibling;

    HESFace() {}
}; // struct HESFace

//****************************************************************************/
//  Class:  HalfEdgeMesh
//  Desc:   Describes system of simple polygons, which share vertices and edges.
/*          Internal implementation uses halfedge data structure
//****************************************************************************/
class HalfEdgeMesh
{
    UniformPool<HESVert>        m_VertPool;
    UniformPool<HESEdge>        m_EdgePool;
    UniformPool<HESFace>        m_FacePool;

    HESVert*                    m_pVerts;
    HESEdge*                    m_pEdges;
    HESFace*                    m_pFaces;

public:

    HalfEdgeMesh() : m_pVerts( NULL ), m_pEdges( NULL ), m_pFaces( NULL ) {}

    bool        CreateVoronoi   ( const Vec2* sites, int nSites, const Frame& bounds );
    int         GetNFaces       () const { return m_FacePool.GetNElem(); }
    int         GetNEdges       () const { return m_EdgePool.GetNElem()/2; }
    int         GetNHalfEdges   () const { return m_EdgePool.GetNElem(); }
    int         GetNVerts       () const { return m_VertPool.GetNElem(); }

    int         PickFace        ( const Vec2& pt ) const;
    bool        MergeFaces      ( int fa, int fb );
    int         GetFaceNVert    ( int FaceID ) const;
    const Vec2& GetFaceVert     ( int FaceID, int vertID );
    void        Clear           ();
    bool        PointInFace     ( int faceID, const Vec2& pt ) const;
    int         GetVertID       ( const Vec2& v, float bias = c_FltEpsilon ) const;
    int         AddVert         ( const Vec2& v );
    int         AddEdge         ( int va, int vb );
    int         AddEdge         ();
    int         AddFace         ();

    int         GetFirstEdgeID  () const { return reinterpret_cast<int>( m_pEdges ); }
    int         GetNextEdge     ( int hEdge ) const;

    int         GetFirstFaceID  () const { return reinterpret_cast<int>( m_pFaces ); }
    int         GetOuterFaceID  () const;
    bool        GetFace         ( int hFace, PolyLine2& poly ) const;
    int         GetNextFace     ( int hFace ) const;
    Vec2        GetVert         ( int hVert ) const;
    int         GetFirstVertID  () const { return reinterpret_cast<int>( m_pVerts ); }
    int         GetNextVert     ( int hVert ) const;

    bool        GetEdge         ( int hEdge, Vec2& a, Vec2& b ) const;
    int         SplitEdge       ( int hEdge, float t );
    int         ExtrudeEdge     ( int hEdge, float dist );
    int         GetNEdgeOuts    ( int hEdge ) const;
    int         GetEdgeOutID    ( int hEdge, int outIdx ) const;

    void        FindFaces       ();

    bool        IsConsistent    () const;
    void        FractalizeEdge  ( int hEdge, float minLen, float magnitude = 0.2f, float uniformity = 1.0f );
    bool        SplitVertex     ( int edgeA, int edgeB, int& edgeNA, int& edgeNB, int& vertID, int& faceID );


    class FaceCirculator
    {
        HESEdge*    m_pCurEdge;
        HESFace*    m_pHostFace;
        int         m_Iteration;
    public:
        FaceCirculator() : m_pHostFace(NULL), m_pCurEdge(NULL), m_Iteration(0) {}
        FaceCirculator( int faceID )
        {
            m_pHostFace = reinterpret_cast<HESFace*>( faceID );
            m_pCurEdge  = m_pHostFace->m_pFirstEdge;
            m_Iteration = 0;
        }
        void operator ++()
        {
            m_pCurEdge = m_pCurEdge->m_pOutEdge;
            if (m_pCurEdge == m_pHostFace->m_pFirstEdge) m_Iteration++;
        }
        operator bool() const
        {
            return (m_Iteration == 0)&&m_pCurEdge;
        }
        void operator +=( int nSteps )
        {
            for (int i = 0; i < nSteps; i++) operator++();
        }
        int GetNeighbor() const { return reinterpret_cast<int>( m_pCurEdge->m_pOpposite->m_pFace ); }
        void Reset()
        {
            m_pCurEdge  = m_pHostFace->m_pFirstEdge;
            m_Iteration = 0;
        }
    protected:
        HESEdge* GetEdge() const { return m_pCurEdge; }

        friend class HalfEdgeMesh;
    }; // class FaceCirculator

    FaceCirculator  GetFaceCirculator( int faceID ) const { return FaceCirculator( faceID ); }
    int             GetNFaceEdges    ( int faceID ) const;

protected:
    void            DestroyFace      ( HESFace* pFace );
    void            DestroyEdge      ( HESEdge* pEdge );
    void            DestroyVert      ( HESVert* pVert );

}; // class HalfEdgeMesh

#endif // __HALFEDGEMESH_H__
