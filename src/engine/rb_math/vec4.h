//****************************************************************************/
//  File:  Vec4.h
//  Desc:  3D vector
//****************************************************************************/
#ifndef __VEC4_H__
#define __VEC4_H__


//****************************************************************************/
//  Class:  Vec4
//  Desc:   4D vector
//****************************************************************************/
class Vec4
{
public:
    float       x, y, z, w;

    inline Vec4() {}
    inline Vec4( float vx, float vy, float vz, float vw ) : x(vx), y(vy), z(vz), w(vw) {}

    float* buf() { return &x; }
    const float* buf() const { return &x; }

    inline float normalize()
    {
        float nr = sqrtf( x*x + y*y + z*z + w*w );
        if (_fabs( nr ) < c_FltEpsilon) return 0.0f;
        float inr = 1.0f/nr;
        x *= inr; y *= inr; z *= inr; w *= inr;
        return nr;
    }

    inline float norm2() const
    {
        return x*x + y*y + z*z + w*w;
    }

    inline float norm() const
    {
        return sqrtf( x*x + y*y + z*z + w*w );
    }

    inline bool is_equal( const Vec4& v, float eps = c_FltEpsilon )
    {
        return (_fabs( x - v.x ) < eps &&
                _fabs( y - v.y ) < eps &&
                _fabs( z - v.z ) < eps &&
                _fabs( w - v.w ) < eps);
    }

    inline Vec4& operator *=( float v )
    {
        x *= v; y *= v; z *= v; w *= v;
        return *this;
    }

    inline Vec4& operator /=( float v )
    {
        x /= v; y /= v; z /= v; w /= v;
        return *this;
    }

    friend inline Vec4 operator -( const Vec4& a, const Vec4& b ) { return Vec4( a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w ); }
    friend inline Vec4 operator +( const Vec4& a, const Vec4& b ) { return Vec4( a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w ); }
    friend inline Vec4 operator *( const Vec4& a, float v ) { return Vec4( a.x*v, a.y*v, a.z*v, a.w*v ); }
    friend inline Vec4 operator *( float v, const Vec4& a ) { return Vec4( a.x*v, a.y*v, a.z*v, a.w*v ); }

    inline void normW()
    {
        x /= w; y /= w; z /= w;
        w = 1.0f;
    }

    static Vec4                     null;
    static Vec4                     one;

}; // class Vec4

#endif // __VEC4_H__

