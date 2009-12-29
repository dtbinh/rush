//****************************************************************************/
//  File:  Mat4.h
//****************************************************************************/
#ifndef __MAT4_H__
#define __MAT4_H__


#include "vec3.h"
#include "vec4.h"
#include "mat3.h"

#include <string.h>

class Plane;

//****************************************************************************/
//  Class:  Mat4
//  Desc:   4x4 Matrix
//****************************************************************************/
class Mat4
{
public:
    float           e00, e01, e02, e03;
    float           e10, e11, e12, e13;
    float           e20, e21, e22, e23;
    float           e30, e31, e32, e33;


    inline Mat4(){}
    inline Mat4( const Quaternion& q ) { Mat3 m = q; *this = m; }
    inline Mat4( const Mat3& m )
    {
        e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = 0.0f;
        e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = 0.0f;
        e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = 0.0f;
        e30 = 0.0f;  e31 = 0.0f;  e32 = 0.0f;  e33 = 1.0f;
    }

    inline Mat4( const Mat3& m, const Vec3& pos )
    {
        e00 = m.e00; e01 = m.e01; e02 = m.e02; e03 = 0.0f;
        e10 = m.e10; e11 = m.e11; e12 = m.e12; e13 = 0.0f;
        e20 = m.e20; e21 = m.e21; e22 = m.e22; e23 = 0.0f;
        e30 = pos.x; e31 = pos.y; e32 = pos.z; e33 = 1.0f;
    }

    inline Mat4(    float v00, float v01, float v02, float v03,
                    float v10, float v11, float v12, float v13,
                    float v20, float v21, float v22, float v23,
                    float v30, float v31, float v32, float v33 )
    {
        e00 = v00; e01 = v01; e02 = v02; e03 = v03;
        e10 = v10; e11 = v11; e12 = v12; e13 = v13;
        e20 = v20; e21 = v21; e22 = v22; e23 = v23;
        e30 = v30; e31 = v31; e32 = v32; e33 = v33;
    }

    inline Mat4( const Vec3& s, const Vec3& t ) { st( s, t ); }
    inline Mat4( const Vec3& row0,
                 const Vec3& row1,
                 const Vec3& row2,
                 const Vec3& trans )
    {
        e00 = row0.x; e01 = row0.y; e02 = row0.z; e03 = 0.0f;
        e10 = row1.x; e11 = row1.y; e12 = row1.z; e13 = 0.0f;
        e20 = row2.x; e21 = row2.y; e22 = row2.z; e23 = 0.0f;
        e30 = trans.x; e31 = trans.y; e32 = trans.z; e33 = 1.0f;
    }
    inline Mat4( const Vec3& v ) { t( v ); }
    inline Vec4& v0() { return *((Vec4*)&e00); }
    inline Vec4& v1() { return *((Vec4*)&e10); }
    inline Vec4& v2() { return *((Vec4*)&e20); }
    inline Vec4& v3() { return *((Vec4*)&e30); }
    inline const Vec4& v0() const { return *((Vec4*)&e00); }
    inline const Vec4& v1() const { return *((Vec4*)&e10); }
    inline const Vec4& v2() const { return *((Vec4*)&e20); }
    inline const Vec4& v3() const { return *((Vec4*)&e30); }

    inline Vec3& translation() const { return *((Vec3*)&e30); }


    inline void st( const Vec3& scale, const Vec3& t )
    {
        e00 = scale.x;  e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
        e10 = 0.0f; e11 = scale.y;  e12 = 0.0f; e13 = 0.0f;
        e20 = 0.0f; e21 = 0.0f; e22 = scale.z;  e23 = 0.0f;
        e30 = t.x;  e31 = t.y;  e32 = t.z;  e33 = 1.0f;
    }

    inline void st( float scale, const Vec3& t )
    {
        e00 = scale;  e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
        e10 = 0.0f; e11 = scale;  e12 = 0.0f; e13 = 0.0f;
        e20 = 0.0f; e21 = 0.0f; e22 = scale;  e23 = 0.0f;
        e30 = t.x;  e31 = t.y;  e32 = t.z;  e33 = 1.0f;
    }

    void srt( float sc, const Vec3& axis, float angle, const Vec3& t );
    void srt( const Vec3& sc, const Mat3& rot, const Vec3& tr );
    void srt( const Vec3& sc, const Vec3& axis, float angle, const Vec3& tr );
    void rot( const Vec3& axis, float angle );
    void srt( float sc, const Mat3& rot, const Vec3& tr );

    inline void t( const Vec3& v )
    {
        e00 = 1.0f; e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
        e10 = 0.0f; e11 = 1.0f; e12 = 0.0f; e13 = 0.0f;
        e20 = 0.0f; e21 = 0.0f; e22 = 1.0f; e23 = 0.0f;
        e30 = v.x;  e31 = v.y;  e32 = v.z;  e33 = 1.0f;
    }

    inline void scaling( const Vec3& v )
    {
        e00 = v.x; e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
        e10 = 0.0f; e11 = v.y; e12 = 0.0f; e13 = 0.0f;
        e20 = 0.0f; e21 = 0.0f; e22 = v.z; e23 = 0.0f;
        e30 = 0.0f;  e31 = 0.0f;  e32 = 0.0f;  e33 = 1.0f;
    }

    void shearXY( float a, float b );

    const float* buf() const { return &e00; }
    float* buf() { return &e00; }


    inline const Mat4& operator =( const Mat4& m )
    {
        memcpy( this, &m, sizeof( Mat4 ) );
        return *this;
    }

    inline Mat4& operator *=( const Mat4& r )
    {
        Mat4 l = *this;
        mul( l, r );
        return *this;
    }

    inline friend Mat4 operator *( const Mat4& l, const Mat4& r )
    {
        Mat4 res;
        res.mul( l, r );
        return res;
    }

    inline Mat4& operator *=( float w )
    {
        e00 *= w; e01 *= w; e02 *= w; e03 *= w;
        e10 *= w; e11 *= w; e12 *= w; e13 *= w;
        e20 *= w; e21 *= w; e22 *= w; e23 *= w;
        e30 *= w; e31 *= w; e32 *= w; e33 *= w;
        return *this;
    }

    inline Mat4& operator +=( const Mat4& r )
    {
        e00 += r.e00; e01 += r.e01; e02 += r.e02; e03 += r.e03;
        e10 += r.e10; e11 += r.e11; e12 += r.e12; e13 += r.e13;
        e20 += r.e20; e21 += r.e21; e22 += r.e22; e23 += r.e23;
        e30 += r.e30; e31 += r.e31; e32 += r.e32; e33 += r.e33;
        return *this;
    }

    inline Mat4& operator -=( const Mat4& r )
    {
        e00 -= r.e00; e01 -= r.e01; e02 -= r.e02; e03 -= r.e03;
        e10 -= r.e10; e11 -= r.e11; e12 -= r.e12; e13 -= r.e13;
        e20 -= r.e20; e21 -= r.e21; e22 -= r.e22; e23 -= r.e23;
        e30 -= r.e30; e31 -= r.e31; e32 -= r.e32; e33 -= r.e33;
        return *this;
    }

    inline void mul( const Mat4& l, const Mat4& r )
    {
        e00 = l.e00*r.e00 + l.e01*r.e10 + l.e02*r.e20 + l.e03*r.e30;
        e01 = l.e00*r.e01 + l.e01*r.e11 + l.e02*r.e21 + l.e03*r.e31;
        e02 = l.e00*r.e02 + l.e01*r.e12 + l.e02*r.e22 + l.e03*r.e32;
        e03 = l.e00*r.e03 + l.e01*r.e13 + l.e02*r.e23 + l.e03*r.e33;

        e10 = l.e10*r.e00 + l.e11*r.e10 + l.e12*r.e20 + l.e13*r.e30;
        e11 = l.e10*r.e01 + l.e11*r.e11 + l.e12*r.e21 + l.e13*r.e31;
        e12 = l.e10*r.e02 + l.e11*r.e12 + l.e12*r.e22 + l.e13*r.e32;
        e13 = l.e10*r.e03 + l.e11*r.e13 + l.e12*r.e23 + l.e13*r.e33;

        e20 = l.e20*r.e00 + l.e21*r.e10 + l.e22*r.e20 + l.e23*r.e30;
        e21 = l.e20*r.e01 + l.e21*r.e11 + l.e22*r.e21 + l.e23*r.e31;
        e22 = l.e20*r.e02 + l.e21*r.e12 + l.e22*r.e22 + l.e23*r.e32;
        e23 = l.e20*r.e03 + l.e21*r.e13 + l.e22*r.e23 + l.e23*r.e33;

        e30 = l.e30*r.e00 + l.e31*r.e10 + l.e32*r.e20 + l.e33*r.e30;
        e31 = l.e30*r.e01 + l.e31*r.e11 + l.e32*r.e21 + l.e33*r.e31;
        e32 = l.e30*r.e02 + l.e31*r.e12 + l.e32*r.e22 + l.e33*r.e32;
        e33 = l.e30*r.e03 + l.e31*r.e13 + l.e32*r.e23 + l.e33*r.e33;
    }

    inline void tmpt( Vec3& v ) const
    {
        float cx = v.x*e00 + v.y*e10 + v.z*e20 + e30;
        float cy = v.x*e01 + v.y*e11 + v.z*e21 + e31;
        float cz = v.x*e02 + v.y*e12 + v.z*e22 + e32;
        v.x = cx; v.y = cy; v.z = cz;
    }

    inline void tm( Vec4& v ) const
    {
        float cx = v.x*e00 + v.y*e10 + v.z*e20 + v.w*e30;
        float cy = v.x*e01 + v.y*e11 + v.z*e21 + v.w*e31;
        float cz = v.x*e02 + v.y*e12 + v.z*e22 + v.w*e32;
        float cw = v.x*e03 + v.y*e13 + v.z*e23 + v.w*e33;
        v.x = cx; v.y = cy; v.z = cz; v.w = cw;
    }

    inline void tmproj( Vec3& v ) const
    {
        float cx = v.x*e00 + v.y*e10 + v.z*e20 + e30;
        float cy = v.x*e01 + v.y*e11 + v.z*e21 + e31;
        float cz = v.x*e02 + v.y*e12 + v.z*e22 + e32;
        float cw = v.x*e03 + v.y*e13 + v.z*e23 + e33;
        if (_fabs( cw ) > c_FltEpsilon)
        {
            v.x = cx/cw; v.y = cy/cw; v.z = cz/cw;
        }
        else
        {
            v.x = cx; v.y = cy; v.z = cz;
        }
    }

    inline void tmvec( Vec3& v ) const
    {
        float cx = v.x*e00 + v.y*e10 + v.z*e20;
        float cy = v.x*e01 + v.y*e11 + v.z*e21;
        float cz = v.x*e02 + v.y*e12 + v.z*e22;
        v.x = cx; v.y = cy; v.z = cz;
    }

    float inverse( const Mat4& m );

    void transpose( const Mat4& m )
    {
        e00 = m.e00; e01 = m.e10; e02 = m.e20; e03 = m.e30;
        e10 = m.e01; e11 = m.e11; e12 = m.e21; e13 = m.e31;
        e20 = m.e02; e21 = m.e12; e22 = m.e22; e23 = m.e32;
        e30 = m.e03; e31 = m.e13; e32 = m.e23; e33 = m.e33;
    }

    inline bool is_equal( const Mat4& m, float eps = c_FltEpsilon )
    {
        return (_fabs( e00 - m.e00 ) < eps && _fabs( e01 - m.e01 ) < eps &&
                _fabs( e02 - m.e02 ) < eps && _fabs( e03 - m.e03 ) < eps &&
                _fabs( e10 - m.e10 ) < eps && _fabs( e11 - m.e11 ) < eps &&
                _fabs( e12 - m.e12 ) < eps && _fabs( e13 - m.e13 ) < eps &&
                _fabs( e20 - m.e20 ) < eps && _fabs( e21 - m.e21 ) < eps &&
                _fabs( e22 - m.e22 ) < eps && _fabs( e23 - m.e23 ) < eps &&
                _fabs( e30 - m.e30 ) < eps && _fabs( e31 - m.e31 ) < eps &&
                _fabs( e32 - m.e32 ) < eps && _fabs( e33 - m.e33 ) < eps);
    }

    void transpose()
    {
        Mat4 tmp( *this );
        transpose( tmp );
    }

    Mat3    getRotationTM() const { return Mat3( e00, e01, e02, e10, e11, e12, e20, e21, e22 ); }
    void    setRotationTM( const Mat3& m )
    {
        e00 = m.e00; e01 = m.e01; e02 = m.e02;
        e10 = m.e10; e11 = m.e11; e12 = m.e12;
        e20 = m.e20; e21 = m.e21; e22 = m.e22;
    }

    void    factorize( Vec3& scale, Quaternion& rot, Vec3& trans ) const;
    void    factorize( Vec3& scale, Mat3& rot, Vec3& trans ) const;

    static Mat4         identity;
    static Mat4         null;
}; // class Mat4

inline Mat4 inverse( const Mat4& tm )
{
    Mat4 invTM;
    invTM.inverse( tm );
    return invTM;
}

inline Mat4 transpose( const Mat4& tm )
{
    Mat4 trTM;
    trTM.transpose( tm );
    return trTM;
}

class Frame;

bool OrthoProjTM( Mat4& m, float viewVolW, float zn, float zf, float aspect );
bool OrthoProjTM( Mat4& m, const Frame& vp, float zn, float zf );
bool PerspProjTM( Mat4& m, float fovx, float zn, float zf, float aspect );
bool FactorPerspProjTM( const Mat4& tm, float& fovx, float& zn, float& zf, float& aspect );

bool ScreenToProjTM( Mat4& m, const Frame& vp );
bool ProjToScreenTM( Mat4& m, const Frame& vp );

float GetProjectedOBBArea( const Mat4& obb, const Mat4& projTM );

Mat4 FlattenMatrix( const Plane& p, const Vec4& dir );
Mat4 IdentityMatrix();

#endif // __MAT4_H__

