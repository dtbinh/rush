/***********************************************************************************/
//  File:   PolyLine2.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __POLYLINE2_H__
#define __POLYLINE2_H__

#include "Vec2.h"
#include "Seg2.h"
#include "Frame.h"
#include <vector>

class TriMesh2;

/***********************************************************************************/
//  Class:  PolyLine2
//  Desc:   
/***********************************************************************************/
class PolyLine2
{
    std::vector<Vec2>           m_Points;

public:
    void        Clear           () { m_Points.clear(); }
    bool        AddPoint        ( const Vec2& pt, bool bNoSelfCross = true );
    void        InsertPoint     ( int pos, const Vec2& pt );
    int         GetNPoints      () const { return static_cast<int>( m_Points.size() ); }
    const Vec2& GetPoint        ( int idx ) const { return m_Points[idx % m_Points.size()]; }
    Vec2        GetParamPoint   ( float t, bool bClosed = true, Vec2* pTangent = NULL ) const;
    float       GetPointParam   ( const Vec2& pt, bool bClosed = true ) const;

    int         GetNSegments    () const;
    void        DeletePoint     ( int idx );
    Seg2        GetEdge         ( int idx ) const;
    Vec2        GetCenter       () const;
    Frame       GetBound        () const;
    void        Move            ( const Vec2&  delta );
    int         GetNearestEdge  ( const Vec2& pt, bool bClosed = true ) const;

    bool        PtIn            ( const Vec2& pt ) const;
    int         PickPt          ( const Vec2& pt, float radius ) const;
    bool        Tesselate       ( TriMesh2& mesh ) const;
    float       Distance        ( const PolyLine2& p ) const;
    void        SmoothHermite   ( int numSubPoints );
    bool        Intersects      ( const Seg2& seg, bool bClosed = true ) const;
    bool        FindPath        ( const Seg2& moveSeg, PolyLine2& path ) const;
    bool        GetSubPoly      ( int ptBeg, int ptEnd, PolyLine2& poly, bool bCW = true ) const;
    float       GetLength       ( bool bClosed ) const;
    void        Refine          ();

    Vec2&       operator       []( int idx ) { return m_Points[idx % m_Points.size()]; }

}; // class PolyLine2

/***********************************************************************************/
//  PolyLine2 inlines 
/***********************************************************************************/
inline Seg2 PolyLine2::GetEdge( int idx ) const 
{ 
    return Seg2( m_Points[idx], m_Points[(idx + 1)%m_Points.size()] ); 
}

#endif //__POLYLINE2_H__