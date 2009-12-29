/***********************************************************************************/
//  File:   PolyLine2.cpp
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "PolyLine2.h"
#include "TriMesh2.h"
#include "windows.h"
#include "float.h"
#include <gl/glu.h>

#pragma comment(lib,"glu32.lib")

/***********************************************************************************/
/*  PolyLine2 implementation
/***********************************************************************************/
int PolyLine2::GetNSegments() const 
{ 
    if (m_Points.size() > 1) return m_Points.size();
    return 0;
} // PolyLine2::GetNSegments

bool PolyLine2::Intersects( const Seg2& seg, bool bClosed ) const
{
    int nSeg = GetNSegments();
    if (!bClosed) nSeg--;
    for (int i = 0; i < nSeg; i++)
    {
        if (seg.intersects( GetEdge( i ), true )) return true;
    }
    return false;
} // PolyLine2::Intersects

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
} // PolyLine2::GetCenter

void PolyLine2::DeletePoint( int idx )
{
    if (idx < 0 || idx >= m_Points.size()) return;
    m_Points.erase( m_Points.begin() + idx );
} // PolyLine2::DeletePoint 

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
} // PolyLine2::AddPoint

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
} // PolyLine2::PickPt

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
} // PolyLine2::GetNearestEdge

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
} // PolyLine2::PtIn

static GLenum s_TriType;
static int    s_cIdx = -1;

void __stdcall VertexCallBack( int idx, TriMesh2* pMesh )
{
    if (!pMesh) return;
    static Face f;
    if (s_TriType == GL_TRIANGLES) 
    {
        if (s_cIdx == -1) s_cIdx = 0;
        ((int*)&f)[s_cIdx] = idx;
        s_cIdx++;
        if (s_cIdx == 3)
        {
            pMesh->AddFace( f );
            s_cIdx = 0;
        }
    }
    else if (s_TriType == GL_TRIANGLE_STRIP)
    {
        if (s_cIdx == -1) s_cIdx = 1;
        if (s_cIdx < 3)
        {
            ((int*)&f)[s_cIdx] = idx;
            s_cIdx++;
        }
        else
        {
            f.va = f.vb;
            f.vb = f.vc;
            f.vc = idx;
            pMesh->AddFace( f );
        }
    }
    else if (s_TriType == GL_TRIANGLE_FAN)
    {
        if (s_cIdx == -1)
        {
            f.va = idx;
            s_cIdx = 2;
        }
        else if (s_cIdx < 3)
        {
            f.vb = idx;
            f.vc = idx;
            s_cIdx++;
        }
        else
        {
            f.vb = f.vc;
            f.vc = idx;
            pMesh->AddFace( f );
        }
    }
} // VertexCallBack

void __stdcall BeginCallBack( GLenum type, TriMesh2* pMesh )
{
    s_TriType = type;
    s_cIdx    = -1;
}

typedef void (__stdcall *GluTessCallbackType)();

bool PolyLine2::Tesselate( TriMesh2& mesh ) const
{
    int nP = GetNPoints();
    if (nP < 3) return false;
    if (nP == 3)
    {
        mesh.AddVert( m_Points[0] );
        mesh.AddVert( m_Points[1] );
        mesh.AddVert( m_Points[2] );
        mesh.AddFace( 0, 1, 2 );
        return true;
    }
    GLUtesselator* tess = gluNewTess();
    if (tess == NULL) return false;

    mesh.Clear();
    gluTessCallback( tess, GLU_TESS_VERTEX_DATA, 
        reinterpret_cast<GluTessCallbackType>( VertexCallBack ) );

    gluTessCallback( tess, GLU_TESS_BEGIN_DATA , 
        reinterpret_cast<GluTessCallbackType>( BeginCallBack ) );

    gluTessProperty( tess, GLU_TESS_BOUNDARY_ONLY, FALSE );
    gluTessProperty( tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD );  


    gluTessBeginPolygon( tess, reinterpret_cast<void*>( &mesh ) ); 
    gluTessBeginContour( tess );

    Vec2 p[4] = { Vec2( 0, 0 ), Vec2( 0, 1 ), Vec2( 1, 1 ), Vec2( 1, 0 ) };

    GLdouble v[3];
    for (int i = 0; i < nP; i++)
    {
        const Vec2& pt = GetPoint( i );
        v[0] = pt.x;
        v[1] = pt.y;
        v[2] = 0.0;
        gluTessVertex( tess, &v[0], reinterpret_cast<void*>( i ) ); 
        mesh.AddVert( pt );
    }   

    gluTessEndContour( tess );
    gluTessEndPolygon( tess ); 

    gluDeleteTess( tess );
    return true;
} // PolyLine2::Tesselate

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
} // PolyLine2::Distance

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
} // PolyLine2::Refine

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
} // PolyLine2::GetBound

void PolyLine2::InsertPoint( int pos, const Vec2& pt )
{
    if (pos < 0) pos = 0;
    if (pos > GetNPoints()) pos = GetNPoints();
    m_Points.insert( m_Points.begin() + pos, pt );
} // PolyLine2::InsertPoint

void PolyLine2::SmoothHermite( int numSubPoints )
{

} // PolyLine2::SmoothHermite

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
} // PolyLine2::GetLength

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
} // PolyLine2::FindPath

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
} // PolyLine2::GetParamPoint

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
} // PolyLine2::Move


