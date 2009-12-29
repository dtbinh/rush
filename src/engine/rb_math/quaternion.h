//****************************************************************************/
//  File:  Quaternion.h
//  Desc:  Quaternion class
//****************************************************************************/
#ifndef __QUATERNION_H__
#define __QUATERNION_H__


#include "vec3.h"

class Mat3;
//****************************************************************************/
//  Class:  Quaternion
//  Desc:   Quaternion class
//****************************************************************************/
class Quaternion
{
public:
    Vec3        v;
    float       s;

    Quaternion(){}
    Quaternion( const Mat3& rot );
    Quaternion( const Vec3& axis, float angle );
    Quaternion( float vx, float vy, float vz, float _s ) : v( Vec3( vx, vy, vz ) ), s(_s) {}

    float* buf() { return &v.x; }
    const float* buf() const { return &v.x; }

    inline float norm() const { return sqrtf( norm2() ); }
    inline float norm2() const { return s * s + v.norm2(); }

    void slerp( const Quaternion& q1, const Quaternion& q2, float t );

    inline void reverse()
    {
	    s = -s;
	    v.reverse();
    }

    float dot( const Quaternion& q ) const
    {
	    return s*q.s + v.x*q.v.x + v.y*q.v.y + v.z*q.v.z;
    }

    inline bool is_equal( const Quaternion& q, float eps = c_FltEpsilon )
    {
        return (v.is_equal( q.v, eps ) &&
                _fabs( s - q.s ) < eps);
    }

    void normalize();

    static Quaternion       null;
}; // class Quaternion

inline Quaternion operator -( const Quaternion& a, const Quaternion& b )
{
    return Quaternion( a.v.x - b.v.x, a.v.y - b.v.y, a.v.z - b.v.z, a.s - b.s );
}

inline Quaternion operator +( const Quaternion& a, const Quaternion& b )
{
    return Quaternion( a.v.x + b.v.x, a.v.y + b.v.y, a.v.z + b.v.z, a.s + b.s );
}

#endif // __QUATERNION_H__

