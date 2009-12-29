//****************************************************************************/
//  File:  Vec3.h
//  Desc:  3D vector
//****************************************************************************/
#ifndef __VEC3_H__
#define __VEC3_H__
#pragma once

#include "Scalar.h"

//****************************************************************************/
//  Class:  Vec3
//  Desc:   3D vector
//****************************************************************************/
class Vec3
{
public:
    float       x, y, z;

    inline Vec3() {}
    inline Vec3( float vx, float vy, float vz ) : x(vx), y(vy), z(vz) {}

    float*          buf() { return &x; }
    const float*    buf() const { return &x; }

    inline float normalize()
    {
        float nr = sqrtf( x*x + y*y + z*z );
        if (_fabs( nr ) < c_FltEpsilon) return 0.0f;
        float inr = 1.0f/nr;
        x *= inr; y *= inr; z *= inr;;
        return nr;
    }

    bool ortonormalize( Vec3& a, Vec3& b );

    inline float dot( const Vec3& v ) const { return x*v.x + y*v.y + z*v.z; }

    inline float norm2() const
    {
        return x*x + y*y + z*z;
    }
    inline float norm() const { return sqrtf( norm2() ); }

    inline Vec3& operator *=( float v )
    {
        x *= v; y *= v; z *= v;
        return *this;
    }

    inline Vec3& operator *=( const Vec3& v )
    {
        x *= v.x; y *= v.y; z *= v.z;
        return *this;
    }

    inline Vec3& operator /=( float v )
    {
        float inv = 1.0f/v;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }

    inline Vec3& operator +=( const Vec3& v )
    {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    inline Vec3& operator -=( const Vec3& v )
    {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    inline void reverse()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    inline float dist( const Vec3& v ) const 
    { 
        Vec3 d = *this - v;
        return d.norm();
    } 

    friend inline Vec3 operator -( const Vec3& a, const Vec3& b ) { return Vec3( a.x - b.x, a.y - b.y, a.z - b.z ); }
    friend inline Vec3 operator +( const Vec3& a, const Vec3& b ) { return Vec3( a.x + b.x, a.y + b.y, a.z + b.z ); }
    friend inline Vec3 operator *( const Vec3& a, float v ) { return Vec3( a.x*v, a.y*v, a.z*v ); }
    friend inline Vec3 operator *( float v, const Vec3& a ) { return Vec3( a.x*v, a.y*v, a.z*v ); }
    friend inline Vec3 operator -( const Vec3& v ) { return Vec3( -v.x, -v.y, -v.z ); }


    inline void cross( const Vec3& a, const Vec3& b ) 
    {
        x = a.y * b.z - a.z * b.y; 
        y = a.z * b.x - a.x * b.z;
        z = a.x * b.y - a.y * b.x;
    }

    inline bool is_equal( const Vec3& v, float eps = c_FltEpsilon )
    {
        return (_fabs( x - v.x ) < eps &&
                _fabs( y - v.y ) < eps &&
                _fabs( z - v.z ) < eps);
    }

    inline void from_spherical( float rho, float theta, float phi )
    {
        float sinTheta = sinf( theta );
        x = rho * sinTheta * cos( phi );
        y = rho * sinTheta * sin( phi );
        z = rho * cos( theta );
    } // from_spherical

    void    random( float minX, float maxX, float minY, float maxY, float minZ, float maxZ );

    static Vec3                     null;
    static Vec3                     one;
    static Vec3                     oX;
    static Vec3                     oY;
    static Vec3                     oZ;

}; // class Vec3

#endif // __VEC3_H__

