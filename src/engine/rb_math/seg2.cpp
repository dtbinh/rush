/***********************************************************************************/
//  File:   Seg2.cpp
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "Seg2.h"
#include "Scalar.h"

/***********************************************************************************/
/*  Seg2 implementation
/***********************************************************************************/
bool Seg2::contains( const Vec2& p, float eps )
{
    if (!is_vertical())
    {
        float t = (p.x - a.x)/(b.x - a.x);
        return ((t >= 0) && (t <= 1.0f) && _fabs( a.y + t*(b.y - a.y) - p.y ) < eps);
    }
    else
    {
        float t = (p.y - a.y)/(b.y - a.y);
        return ((t >= 0) && (t <= 1.0f) && _fabs( a.x + t*(b.x - a.x) - p.x ) < eps);
    }
} // Seg2::contains

bool Seg2::intersects( const Seg2& s, bool bInclusive, Vec2* pPt ) const
{
    Vec2 d1 = b - a;
    Vec2 d2 = s.b - s.a;
    float det = d2.perp( d1 );
    if (_fabs( det ) < c_FltEpsilon) return false;
    float invdet = 1.0f / det;
    float p = (-a.y*d2.x + a.x*d2.y - d2.y*s.a.x + d2.x*s.a.y)*invdet;
    float q = (-a.y*d1.x + a.x*d1.y - d1.y*s.a.x + d1.x*s.a.y)*invdet;
    if (bInclusive)
    {
        if (p < 0.0f || p > 1.0f) return false;
        if (q < 0.0f || q > 1.0f) return false;
    }
    else
    {
        if (p <= 0.0f || p >= 1.0f) return false;
        if (q <= 0.0f || q >= 1.0f) return false;
    }
    if (pPt)
    {
        *pPt = a + q*d1;
    }
    return true;
} // Seg2::intersects

float Seg2::dist2( const Seg2& s ) const
{
    return tmin( dist2( s.a ), dist2( s.b ), s.dist2( a ), s.dist2( b ) );
} // Seg2::dist2

float Seg2::dist2( const Vec2& p ) const
{
    Vec2 ba = b - a;
    Vec2 pa = p - a;

    float det = ba.norm2();
    if (_fabs( det ) < c_FltEpsilon) 
    {
        return b.dist2( p );
    }
    float t = pa.dot( ba )/det;
    t = clamp( t, 0.0f, 1.0f );
    float res = p.dist2( a + ba*t );
    return res;
} // Seg2::dist2
