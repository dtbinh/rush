//****************************************************************************/
//  File:  Frame.h
//  Desc:  2D rectangle
//****************************************************************************/
#ifndef __FRAME_H__
#define __FRAME_H__

#include "scalar.h"
#include "vec2.h"
#include "rand.h"

//****************************************************************************/
//  Class:  Frame
//  Desc:   2D rectangle
//****************************************************************************/
class Frame
{
public:
    float       x, y, w, h;

    inline          Frame( float nx, float ny, float nw, float nh ) : x(nx), y(ny), w(nw), h(nh) {}
    inline          Frame() {}
    inline          Frame( const Vec2& pt ) : x( pt.x ), y( pt.y ), w( 0.0f ), h( 0.0f ) {}
    void            zero() { x = y = w = h = 0; }
    float           r   () const { return x + w; }
    float           b   () const { return y + h; }
    float           cx  () const { return x + w*0.5f; }
    float           cy  () const { return y + h*0.5f; }
    const Vec2&     lt  () const { return *((Vec2*)&x); }

    Vec2    rand_pt() const { return Vec2( frand( x, r() ), frand( y, b() ) ); }
    Vec2    center() const { return Vec2( x + w*0.5f, y + h*0.5f ); }

    bool    PtIn( float px, float py ) const
    {
        return (px >= x)&&(py >= y)&&(px <= x + w)&&(py <= y + h);
    }

    void    Clip( const Frame& rct );

    inline bool IsOutside( const Frame& rct ) const
    {
        return	(r() < rct.x) || (x > rct.r()) ||
                (b() < rct.y) || (y > rct.b());
    }

    void AddPoint( const Vec2& pt )
    {
        if (pt.x < x) x = pt.x;
        if (pt.y < y) y = pt.y;
        if (pt.x > x + w) w = pt.x - x;
        if (pt.y > y + h) h = pt.y - y;
    }

    inline bool Intersects( const Frame& rct ) const
    {
        return !IsOutside( rct ) && !rct.IsOutside( *this );
    }

    inline void Unify()
    {
        if (w < 0) { x += w; w = -w; }
        if (h < 0) { y += h; h = -h; }
    }

    inline void Union( const Frame& rct )
    {
        x = tmin( x, rct.x );
        y = tmin( y, rct.y );
        w = tmax( w, rct.w, rct.r() - x, r() - rct.x );
        h = tmax( h, rct.h, rct.b() - y, b() - rct.y );
    }

    inline void Inflate( float v )
    {
        x -= v;
        y -= v;
        w += v*2.0f;
        h += v*2.0f;
    }

    inline void Inflate( float left, float top, float right, float bottom )
    {
        x -= left;
        y -= top;
        w += left + right;
        h += top + bottom;
    }

    inline bool equal( const Frame& r ) const
    {
        return (_fabs( x - r.x ) < c_FltEpsilon &&
                _fabs( y - r.y ) < c_FltEpsilon &&
                _fabs( w - r.w ) < c_FltEpsilon &&
                _fabs( h - r.h ) < c_FltEpsilon);
    }

    float dist( const Frame& rc ) const;

    inline Frame& operator +=( const Vec2& v )
    {
        x += v.x; y += v.y;
        return *this;
    }

    inline Frame& operator -=( const Vec2& v )
    {
        x -= v.x; y -= v.y;
        return *this;
    }

    static Frame null;
}; // class Frame

#endif // __FRAME_H__

