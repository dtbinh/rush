//****************************************************************************/
//  File:  Tri2.h
//  Desc:  2D triangle
//****************************************************************************/
#ifndef __TRI2_H__
#define __TRI2_H__

#include "vec2.h"

//****************************************************************************/
//  Class:  Tri2
//  Desc:   2D triangle
//****************************************************************************/
class Tri2
{
public:
    Vec2    a, b, c;

    Tri2() {}
    Tri2( const Vec2& pa, const Vec2& pb, const Vec2& pc ) : a(pa), b(pb), c(pc){}

    Vec2            bary_coords( const Vec2& p ) const;
    inline Vec2     from_bary_coords( const Vec2& p ) const
    {
        return Vec2( (a.x - c.x)*p.x + (b.x - c.x)*p.x + c.x,
                     (a.y - c.y)*p.y + (b.y - c.y)*p.y + c.y);
    }
    bool            contains( const Vec2& p ) const;
    inline float    signed_area() const { return 0.5f*(-b.x*a.y + c.x*a.y + a.x*b.y - c.x*b.y - a.x*c.y + b.x*c.y); }

}; // class Tri2

#endif // __TRI2_H__

