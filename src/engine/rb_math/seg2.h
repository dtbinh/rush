/***********************************************************************************/
//  File:   Seg2.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __SEG2_H__
#define __SEG2_H__

#include "Vec2.h"
#include "MathConst.h"

/***********************************************************************************/
//  Class:  Seg2
//  Desc:   
/***********************************************************************************/
class Seg2
{
public:
    Vec2            a;
    Vec2            b;

                    Seg2        () {}
                    Seg2        ( const Vec2& va, const Vec2& vb ) : a( va ), b( vb ) {}
                    Seg2        ( float ax, float ay, float bx, float by ) : a( Vec2( ax, ay ) ), b( Vec2( bx, by ) ) {}

    bool            intersects  ( const Seg2& s, bool bInclusive = true, Vec2* pPt = NULL ) const;
    inline bool     is_vertical () const { return (_fabs( a.x - b.x ) < c_FltEpsilon); }
    inline float    length      () const { return sqrtf( (a.y - b.y)* (a.y - b.y) + (a.y - b.y)* (a.y - b.y) ); }
    bool            contains    ( const Vec2& p, float eps = c_FltEpsilon );
    float           dist2       ( const Seg2& s ) const;
    float           dist2       ( const Vec2& p ) const;

}; // class Seg2

#endif //__SEG2_H__