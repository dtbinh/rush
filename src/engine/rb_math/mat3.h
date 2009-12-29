//****************************************************************************/
//  File:  Mat3.h
//  Desc:  4x4 Matrix
//****************************************************************************/
#ifndef __MAT3_H__
#define __MAT3_H__


#include "vec3.h"

class Quaternion;
class Mat4;
class Vec2;
//****************************************************************************/
//  Class:  Mat3
//  Desc:   3x3 Matrix
//****************************************************************************/
class Mat3
{
public:
    float           e00, e01, e02;
    float           e10, e11, e12;
    float           e20, e21, e22;

    Mat3(){}
    Mat3( const Quaternion& q );
    Mat3( const Mat4& m );
    Mat3( const Vec3& axis, float angle );

    Mat3(   float v00, float v01, float v02,
            float v10, float v11, float v12,
            float v20, float v21, float v22 )
    {
        e00 = v00; e01 = v01; e02 = v02;
        e10 = v10; e11 = v11; e12 = v12;
        e20 = v20; e21 = v21; e22 = v22;
    }

    inline Vec3& v0() { return *((Vec3*)&e00); }
    inline Vec3& v1() { return *((Vec3*)&e10); }
    inline Vec3& v2() { return *((Vec3*)&e20); }

    void srt( float s, float angle, const Vec2& trans );
    void srt( const Vec2& s, float angle, const Vec2& trans );

    inline Mat3( const Vec3& row0, const Vec3& row1, const Vec3& row2 )
    {
        e00 = row0.x; e01 = row0.y; e02 = row0.z;
        e10 = row1.x; e11 = row1.y; e12 = row1.z;
        e20 = row2.x; e21 = row2.y; e22 = row2.z;
    }

    float inverse( const Mat3& tm );

    float invert()
    {
        Mat3 tmp( *this );
        return inverse( tmp );
    }

    float determinant() const;

    void transpose( const Mat3& m )
    {
        e00 = m.e00; e01 = m.e10; e02 = m.e20;
        e10 = m.e01; e11 = m.e11; e12 = m.e21;
        e20 = m.e02; e21 = m.e12; e22 = m.e22;
    }

    void transpose()
    {
        Mat3 tmp( *this );
        transpose( tmp );
    }


    const float* buf() const { return &e00; }
    float* buf() { return &e00; }

    inline const Mat3& operator =( const Mat3& matr )
    {
        e00 = matr.e00; e01 = matr.e01; e02 = matr.e02;
        e10 = matr.e10; e11 = matr.e11; e12 = matr.e12;
        e20 = matr.e20; e21 = matr.e21; e22 = matr.e22;
        return *this;
    }

    inline Mat3& operator *=( const Mat3& r )
    {
        Mat3 l = *this;
        mul( l, r );
        return *this;
    }

    inline Mat3& operator *=( float w )
    {
        e00 *= w; e01 *= w; e02 *= w;
        e10 *= w; e11 *= w; e12 *= w;
        e20 *= w; e21 *= w; e22 *= w;
        return *this;
    }

    inline Mat3& operator +=( const Mat3& r )
    {
        e00 += r.e00; e01 += r.e01; e02 += r.e02;;
        e10 += r.e10; e11 += r.e11; e12 += r.e12;;
        e20 += r.e20; e21 += r.e21; e22 += r.e22;;
        return *this;
    }

    inline Mat3& operator -=( const Mat3& r )
    {
        e00 -= r.e00; e01 -= r.e01; e02 -= r.e02;;
        e10 -= r.e10; e11 -= r.e11; e12 -= r.e12;;
        e20 -= r.e20; e21 -= r.e21; e22 -= r.e22;;
        return *this;
    }

    inline void mul( const Mat3& l, const Mat3& r )
    {
        e00 = l.e00*r.e00 + l.e01*r.e10 + l.e02*r.e20;
        e01 = l.e00*r.e01 + l.e01*r.e11 + l.e02*r.e21;
        e02 = l.e00*r.e02 + l.e01*r.e12 + l.e02*r.e22;

        e10 = l.e10*r.e00 + l.e11*r.e10 + l.e12*r.e20;
        e11 = l.e10*r.e01 + l.e11*r.e11 + l.e12*r.e21;
        e12 = l.e10*r.e02 + l.e11*r.e12 + l.e12*r.e22;

        e20 = l.e20*r.e00 + l.e21*r.e10 + l.e22*r.e20;
        e21 = l.e20*r.e01 + l.e21*r.e11 + l.e22*r.e21;
        e22 = l.e20*r.e02 + l.e21*r.e12 + l.e22*r.e22;
    }

    inline void tm( Vec3& v ) const
    {
        float cx = v.x*e00 + v.y*e10 + v.z*e20;
        float cy = v.x*e01 + v.y*e11 + v.z*e21;
        float cz = v.x*e02 + v.y*e12 + v.z*e22;
        v.x = cx; v.y = cy; v.z = cz;
    }

    inline bool is_equal( const Mat3& m, float eps = c_FltEpsilon )
    {
        return (_fabs( e00 - m.e00 ) < eps && _fabs( e01 - m.e01 ) < eps && _fabs( e02 - m.e02 ) < eps &&
                _fabs( e10 - m.e10 ) < eps && _fabs( e11 - m.e11 ) < eps && _fabs( e12 - m.e12 ) < eps &&
                _fabs( e20 - m.e20 ) < eps && _fabs( e21 - m.e21 ) < eps && _fabs( e22 - m.e22 ) < eps);
    }

    void from_euler( float phi, float theta, float psi );
    Vec3 to_euler() const;

    void rot( const Vec3& axis, float angle );

    inline void scaling( const Vec3& v )
    {
        e00 = v.x; e01 = 0.0f; e02 = 0.0f;
        e10 = 0.0f; e11 = v.y; e12 = 0.0f;
        e20 = 0.0f; e21 = 0.0f; e22 = v.z;
    }

    friend inline Mat3 operator *( const Mat3& a, const Mat3&b )
    {
        Mat3 res;
        res.mul( a, b );
        return res;
    }

    static Mat3         identity;
    static Mat3         null;

}; // class Mat3

#endif // __MAT3_H__

