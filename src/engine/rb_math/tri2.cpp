//****************************************************************************/
//  File:  Tri2.cpp
//****************************************************************************/
#include "stdafx.h"
#include "Tri2.h"
#include "MathConst.h"

//****************************************************************************/
/*	Tri2 implementation
//****************************************************************************/
Vec2 Tri2::bary_coords( const Vec2& p ) const
{
    Vec2 res;
    float acx	= a.x - c.x;
    float acy	= a.y - c.y;
    float bcx	= b.x - c.x;
    float bcy	= b.y - c.y;
    float pcx	= p.x - c.x;
    float pcy	= p.y - c.y;
    float m00 	= acx*acx + acy*acy;
    float m01 	= acx*bcx + acy*bcy;
    float m11 	= bcx*bcx + bcy*bcy;
    float r0  	= acx*pcx + acy*pcy;
    float r1  	= bcx*pcx + bcy*pcy;
    float det 	= m00 * m11 - m01 * m01;
    if (_fabs( det ) < c_FltEpsilon) return Vec2::null;
    float invDet = 1.0f/det;

    res.x = (m11 * r0 - m01 * r1) * invDet;
    res.y = (m00 * r1 - m01 * r0) * invDet;
    return res;
} // Tri2::bary_coords

bool Tri2::contains( const Vec2& p ) const
{
    Vec2 bc = bary_coords( p );
    return (bc.x >= 0.0f && bc.y >= 0.0f && bc.x + bc.y <= 1.0f);
} // Tri2::contains
