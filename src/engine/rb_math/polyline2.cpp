/***********************************************************************************/
//  File:   PolyLine2.cpp
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "polyline2.h"
#include "float.h"

/***********************************************************************************/
/*  PolyLine2 implementation
/***********************************************************************************/
int PolyLine2::GetNSegments() const
{
    if (m_Points.size() > 1) return m_Points.size();
    return 0;
}

bool PolyLine2::Intersects( const Seg2& seg, bool bClosed ) const
{
    int nSeg = GetNSegments();
    if (!bClosed) nSeg--;
    for (int i = 0; i < nSeg; i++)
    {
        if (seg.intersects( GetEdge( i ), true )) return true;
    }
    return false;
}

Vec2 PolyLine2::GetCenter() const
{
    int nP = GetNPoints();
    Vec2 res( 0.0f, 0.0f );
    for (int i = 0; i < nP; i++)
    {
        res += GetPoint( i );
    }
    res /= float( nP );
    return res;
}

void PolyLine2::DeletePoint( int idx )
{
    if (idx < 0 || idx >= m_Points.size()) return;
    m_Points.erase( m_Points.begin() + idx );
}

bool PolyLine2::AddPoint( const Vec2& pt, bool bNoSelfCross )
{
    int nP = GetNPoints();
    if (bNoSelfCross && nP > 0)
    {
        int nSeg = GetNSegments() - 2;
        Seg2 newSeg( pt, GetPoint( nP - 1 ) );
        for (int i = 0; i < nSeg; i++)
        {
            if (newSeg.intersects( GetEdge( i ) )) return false;
        }
    }
    m_Points.push_back( pt );
    return true;
}

int PolyLine2::PickPt( const Vec2& pt, float radius ) const
{
    int nPt = m_Points.size();
    float r2 = radius*radius;
    int minPt = -1;
    for (int i = 0; i < nPt; i++)
    {
        if (m_Points[i].dist2( pt ) <= r2)
        {
            minPt = i;
        }
    }
    return minPt;
}

int PolyLine2::GetNearestEdge( const Vec2& pt, bool bClosed ) const
{
    int minEdge = -1;
    float minDist = FLT_MAX;
    int nPt = m_Points.size();
    if (!bClosed)
    {
        nPt--;
    }
    for (int i = 0; i < nPt; i++)
    {
        float d = pt.dist_seg( GetPoint( i ), GetPoint( i + 1 ) );
        if (d < minDist)
        {
            minDist = d;
            minEdge = i;
        }
    }
    return minEdge;
}

bool PolyLine2::PtIn( const Vec2& pt ) const
{
    int nP = GetNPoints();
    if (nP < 3) return false;
    int nW = 0;

    //  loop through all edges of the polygon
    for (int i = 0; i < nP; i++)
    {
        const Vec2& a = m_Points[i];
        const Vec2& b = GetPoint( i + 1 );
        if (a.y <= pt.y)
        {
            if (b.y > pt.y)
            {
                if (pt.leftof( a, b ) > 0.0f)
                {
                    nW++;
                }
            }
        }
        else
        {
            if (b.y <= pt.y)
            {
                if (pt.leftof( a, b ) < 0.0f)
                {
                    nW--;
                }
            }
        }
    }
    return (nW != 0);
}

float PolyLine2::Distance( const PolyLine2& p ) const
{
    int nSA = GetNSegments();
    int nSB = p.GetNSegments();
    if (nSA == 0 || nSB == 0) return 0.0f;
    float res = GetEdge( 0 ).dist2( p.GetEdge( 0 ) );

    for (int i = 0; i < nSA; i++)
    {
        for (int j = 0; j < nSB; j++)
        {
            float cDist = GetEdge( i ).dist2( p.GetEdge( j ) );
            if (cDist < res) res = cDist;
        }
    }
    return sqrtf( res );
}

void PolyLine2::Refine()
{
    int cP = 0;
    while (cP < m_Points.size())
    {
        const Vec2& pc = GetPoint( cP );
        const Vec2& pn = GetPoint( cP + 1 );
        if (pc.equal( pn ))
        {
            DeletePoint( cP );
        }
        else
        {
            cP++;
        }
    }
}

Frame PolyLine2::GetBound() const
{
    int nP = GetNPoints();
    if (nP == 0) return Frame::null;
    const Vec2& p0 = GetPoint( 0 );
    Frame rect( p0.x, p0.y, p0.x, p0.y );
    for (int i = 1; i < nP; i++)
    {
        const Vec2& p = GetPoint( i );
        if (p.x < rect.x) rect.x = p.x;
        if (p.y < rect.y) rect.y = p.y;
        if (p.x > rect.w) rect.w = p.x;
        if (p.y > rect.h) rect.h = p.y;
    }
    rect.w -= rect.x;
    rect.h -= rect.y;
    return rect;
}

void PolyLine2::InsertPoint( int pos, const Vec2& pt )
{
    if (pos < 0) pos = 0;
    if (pos > GetNPoints()) pos = GetNPoints();
    m_Points.insert( m_Points.begin() + pos, pt );
}

void PolyLine2::SmoothHermite( int numSubPoints )
{

}

bool PolyLine2::GetSubPoly( int ptBeg, int ptEnd, PolyLine2& poly, bool bCW ) const
{
    poly.Clear();
    if (ptBeg < 0 || ptEnd < 0) return false;
    if (bCW)
    {
        if (ptEnd < ptBeg) ptEnd += m_Points.size();
        for (int i = ptBeg; i <= ptEnd; i++)
        {
            poly.AddPoint( GetPoint( i ) );
        }
    }
    else
    {
        if (ptBeg < ptEnd) ptBeg += m_Points.size();
        for (int i = ptBeg; i >= ptEnd; i--)
        {
            poly.AddPoint( GetPoint( i ) );
        }
    }
    return true;
} // PolyLine2::GetSubPoly

float PolyLine2::GetLength( bool bClosed ) const
{
    if (m_Points.size() == 0) return 0.0f;
    float res = 0.0f;
    for (int i = 1; i < m_Points.size(); i++)
    {
        res += m_Points[i - 1].dist( m_Points[i] );
    }
    if (bClosed)
    {
        res += m_Points[0].dist( m_Points.back() );
    }
    return res;
}

bool PolyLine2::FindPath( const Seg2& moveSeg, PolyLine2& path ) const
{
    path.Clear();
    const Vec2& a = moveSeg.a;
    const Vec2& b = moveSeg.b;
    if (!PtIn( a ) || !PtIn( b )) return false;
    int nPt = m_Points.size();
    Vec2 pt;
    float minDist = FLT_MAX;
    float maxDist = 0.0f;
    int   nBeg1   = -1;
    int   nBeg2   = -1;
    int   nEnd1   = -1;
    int   nEnd2   = -1;

    for (int i = 0; i < nPt; i++)
    {
        Seg2 seg = GetEdge( i );
        if (!seg.intersects( moveSeg, false, &pt )) continue;
        float dist = pt.dist2( a );
        if (dist < minDist)
        {
            minDist = dist;
            nBeg1   = i;
            nBeg2   = i + 1;
        }
        if (dist > maxDist)
        {
            maxDist = dist;
            nEnd1   = i + 1;
            nEnd2   = i;
        }
    }

    GetSubPoly( nBeg1, nEnd1, path, false );
    path.InsertPoint( 0, a );
    path.AddPoint( b );
    float len = path.GetLength( false );

    GetSubPoly( nBeg2, nEnd2, path, true );
    path.InsertPoint( 0, a );
    path.AddPoint( b );

    if (path.GetLength( false ) > len)
    {
        GetSubPoly( nBeg1, nEnd1, path, false );
        path.InsertPoint( 0, a );
        path.AddPoint( b );
    }

    return true;
}

Vec2 PolyLine2::GetParamPoint( float t, bool bClosed, Vec2* pTangent ) const
{
    float tlen = t*GetLength( bClosed );
    if (m_Points.size() == 0)
    {
        if (!pTangent) *pTangent = Vec2::null;
        return Vec2::null;
    }
    if (m_Points.size() == 1)
    {
        if (!pTangent) *pTangent = Vec2::null;
        return m_Points[0];
    }

    float cDist = 0.0f;
    int lastPt = m_Points.size() - 1;
    if (bClosed) lastPt++;
    for (int i = 1; i <= lastPt; i++)
    {
        const Vec2& a = GetPoint( i - 1 );
        const Vec2& b = GetPoint( i     );

        float cLen = m_Points[i - 1].dist( m_Points[i] );
        if (cDist + cLen >= tlen)
        {
            Vec2 ab = b - a;
            ab.normalize();
            if (pTangent) *pTangent = ab;
            return a + (tlen - cDist)*ab;
        }
        cDist += cLen;
    }

    //  for t > 1
    if (pTangent)
    {
        *pTangent = GetPoint( lastPt ) - GetPoint( lastPt - 1 );
        pTangent->normalize();
    }
    return GetPoint( lastPt );
}

float PolyLine2::GetPointParam( const Vec2& pt, bool bClosed ) const
{
    int minEdge = -1;
    float minDist = FLT_MAX;
    float minLen  = 0.0f;
    int nPt = m_Points.size();
    if (!bClosed)
    {
        nPt--;
    }
    float len = 0.0f;
    for (int i = 0; i < nPt; i++)
    {
        const Vec2& a = GetPoint( i );
        const Vec2& b = GetPoint( i + 1 );
        float d = pt.dist_seg( a, b );
        if (d < minDist)
        {
            minDist = d;
            minEdge = i;
            minLen  = len;
        }
        len += (a - b).norm();
    }

    const Vec2& a = GetPoint( minEdge );
    const Vec2& b = GetPoint( minEdge + 1 );
    Vec2 dir = b - a;
    dir.normalize();
    float dlen = minLen + dir.dot( pt - a );
    return dlen/GetLength( bClosed );
}

void PolyLine2::Move( const Vec2&  delta )
{
    int nPt = m_Points.size();
    for (int i = 0; i < nPt; i++)
    {
        m_Points[i] += delta;
    }
}


