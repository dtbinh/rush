//****************************************************************************/
//  File:  Vec2.h
//  Desc:  2D vector
//****************************************************************************/
#ifndef __VEC2_H__
#define __VEC2_H__
#pragma once

#include "MathConst.h"
#include "Scalar.h"

class Frame;
//****************************************************************************/
//  Class:  Vec2
//  Desc:   3D vector
//****************************************************************************/
class Vec2
{
public:
    float       x, y;

    inline Vec2() {}
    inline Vec2( float vx, float vy ) : x(vx), y(vy) {}

    float*          buf() { return &x; }
    const float*    buf() const { return &x; }

    inline float normalize()
    {
        float nr = sqrtf( x*x + y*y );
        if (_fabs( nr ) < c_FltEpsilon) return 0.0f;
        float inr = 1.0f/nr;
        x *= inr; y *= inr;
        return nr;
    }

    inline float dot( const Vec2& v ) const { return x*v.x + y*v.y; }
    inline float perp( const Vec2& v ) const { return x*v.y - y*v.x; }
    inline bool  equal( const Vec2& v ) const 
    { 
        return _fabs( x - v.x ) < c_FltEpsilon && 
            _fabs( y - v.y ) < c_FltEpsilon; 
    }

    inline float norm2() const
    {
        return x*x + y*y;
    }
    inline float norm() const { return sqrtf( norm2() ); }

    inline Vec2& operator *=( float v )
    {
        x *= v; y *= v;;
        return *this;
    }

    inline Vec2& operator /=( float v )
    {
        float inv = 1.0f/v;
        x *= inv; y *= inv;
        return *this;
    }

    inline Vec2& operator +=( const Vec2& v )
    {
        x += v.x; y += v.y;
        return *this;
    }

    inline Vec2& operator -=( const Vec2& v )
    {
        x -= v.x; y -= v.y;
        return *this;
    }

    friend inline Vec2 operator -( const Vec2& v )
    {
        return Vec2( -v.x, -v.y );
    }

    inline float dist( const Vec2& v ) const 
    { 
        Vec2 d = *this - v;
        return d.norm();
    } 

    inline float dist2( const Vec2& v ) const 
    { 
        Vec2 d = *this - v;
        return d.norm2();
    } 

    inline float dist_line( const Vec2& a, const Vec2& b ) const
    {
        float d = a.dist2( b );
        if (d < c_FltEpsilon) return dist( a );
        return _fabs( leftof( a, b )/sqrtf( d ) );
    }

    inline float dist_seg( const Vec2& a, const Vec2& b ) const
    {
        float d = a.dist2( b );
        if (d < c_FltEpsilon) return dist( a );
        float ratio = ((x - a.x)*(b.x - a.x) + (y - a.y)*(b.y - a.y))/d;
        if (ratio <= 0.0f) return dist( a );
        if (ratio >= 1.0f) return dist( b );
        return _fabs( leftof( a, b )/sqrtf( d ) );
    }

    inline float angle( const Vec2& v ) const
    {
        float ang = atan2f( v.x, v.y ) - atan2f( x, y );
        if (ang < 0.0f) ang += c_DoublePI;
        return ang;
    }

    void rotate( float ang );
    
    Vec2 normal() const
    {
        return Vec2( y, -x );    
    }

    void remap( const Frame& from, const Frame& to );

    inline float leftof( const Vec2& a, const Vec2& b ) const
    {
        return (b.x - a.x)*(y - a.y) - (x - a.x)*(b.y - a.y);
    }

    inline bool is_equal( const Vec2& v, float eps = c_FltEpsilon )
    {
        return (_fabs( x - v.x ) < eps &&
                _fabs( y - v.y ) < eps);
    }

    friend inline Vec2 operator -( const Vec2& a, const Vec2& b ) { return Vec2( a.x - b.x, a.y - b.y ); }
    friend inline Vec2 operator +( const Vec2& a, const Vec2& b ) { return Vec2( a.x + b.x, a.y + b.y ); }
    friend inline Vec2 operator /( const Vec2& a, float f ) { return Vec2( a.x/f, a.y/f ); }
    friend inline Vec2 operator *( const Vec2& a, float f ) { return Vec2( a.x*f, a.y*f ); }
    friend inline Vec2 operator *( float f, const Vec2& a ) { return Vec2( a.x*f, a.y*f ); }

    static Vec2                     null;
    static Vec2                     one;
    static Vec2                     oX, oY;
}; // class Vec2

#endif // __VEC2_H__

