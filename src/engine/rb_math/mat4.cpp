//****************************************************************************/
//  File:  Mat4.cpp
//  Desc:  4x4 Matrix
//****************************************************************************/
#include "precompile.h"
#include "mat4.h"
#include "frame.h"
#include "mathconst.h"
#include "plane.h"

Mat4 FlattenMatrix( const Plane& p, const Vec4& dir )
{
    float d = dir.x*p.a + dir.y*p.b + dir.z*p.c + dir.w*p.d;
    Mat4 m;
    m.e00 = d - p.a*dir.x; m.e01 =   - p.a*dir.y;  m.e02 =   - p.a*dir.z;  m.e03 =   - p.a*dir.w;
    m.e10 =   - p.b*dir.x; m.e11 = d - p.b*dir.y;  m.e12 =   - p.b*dir.z;  m.e13 =   - p.b*dir.w;
    m.e20 =   - p.c*dir.x; m.e21 =   - p.c*dir.y;  m.e22 = d - p.c*dir.z;  m.e23 =   - p.c*dir.w;
    m.e30 =   - p.d*dir.x; m.e31 =   - p.d*dir.y;  m.e32 =   - p.d*dir.z;  m.e33 = d - p.d*dir.w;
    return m;
} // FlattenMatrix

bool OrthoProjTM( Mat4& m, float viewVolW, float zn, float zf, float aspect )
{
    float	  w = viewVolW;
    float	  h = viewVolW / aspect;
    float	  dz = zf - zn;
    if (_fabs( dz ) < c_FltEpsilon ||
        _fabs( w  ) < c_FltEpsilon ||
        _fabs( h  ) < c_FltEpsilon) return false;

    m.e00 = 2.0f/w; m.e01 = 0.0f;	m.e02 = 0.0f;			m.e03 = 0.0f;
    m.e10 = 0.0f;	m.e11 = 2.0f/h; m.e12 = 0.0f;			m.e13 = 0.0f;
    m.e20 = 0.0f;	m.e21 = 0.0f;	m.e22 = 1.0f/(zf - zn); m.e23 = 0.0f;
    m.e30 = 0.0f;	m.e31 = 0.0f;	m.e32 = -zn/(zf - zn);	m.e33 = 1.0f;
    return true;
} // OrthoProjTM

bool OrthoProjTM( Mat4& m, const Frame& vp, float zn, float zf )
{
    float	  dz = zf - zn;
    if (_fabs( dz    ) < c_FltEpsilon ||
        _fabs( vp.w  ) < c_FltEpsilon ||
        _fabs( vp.h  ) < c_FltEpsilon) return false;

    m.e00 = 2.0f/vp.w;  m.e01 = 0.0f;	    m.e02 = 0.0f;			m.e03 = 0.0f;
    m.e10 = 0.0f;	    m.e11 = 2.0f/vp.h;  m.e12 = 0.0f;			m.e13 = 0.0f;
    m.e20 = 0.0f;	    m.e21 = 0.0f;	    m.e22 = 1.0f/(zf - zn); m.e23 = 0.0f;
    m.e30 = -(vp.x + vp.r())/vp.w;	m.e31 = -(vp.y + vp.b())/vp.h;
    m.e32 = -zn/(zf - zn);	m.e33 = 1.0f;
    return true;
} // OrthoProjTM

bool PerspProjTMWH( Mat4& m, float w, float h, float zn, float zf )
{
    m.e00 = 2*zn/w;   m.e01 = 0.0f;      m.e02 = 0.0f;           m.e03 = 0.0f;
    m.e10 = 0.0f;     m.e11 = 2*zn/h;    m.e12 = 0.0f;           m.e13 = 0.0f;
    m.e20 = 0.0f;     m.e21 = 0.0f;      m.e22 = zf/(zf-zn);     m.e23 = 1.0f;
    m.e30 = 0.0f;     m.e31 = 0.0f;      m.e32 = zn*zf/(zn-zf);  m.e33 = 0.0f;
    return true;
} // PerspProjTM

bool PerspProjTM( Mat4& m, float fovx, float zn, float zf, float aspect )
{
    float w = 2.0f * zn * tan( fovx * 0.5f );
    if (w <= 0.0f || aspect < 0.0f || zn <= 0.0f || zf <= zn ) return false;
    float h = w/aspect;
    return PerspProjTMWH( m, w, h, zn, zf );
} // PerspProjTM

bool FactorPerspProjTM( const Mat4& tm, float& fovx, float& zn, float& zf, float& aspect )
{
    if (_fabs( tm.e22 ) < c_Epsilon ||
        _fabs( tm.e32 ) < c_Epsilon ||
        _fabs( tm.e22 - 1.0f ) < c_Epsilon)
    {
        return false;
    }
    zn = -tm.e32/tm.e22;
    float w  = 2.0f*zn/tm.e00;
    float h  = 2.0f*zn/tm.e11;
    zf = tm.e22*zn/(tm.e22 - 1.0f);

    aspect = w/h;
    fovx = 2.0f*atanf( w/(2.0f*zn) );
    return true;
}

bool ScreenToProjTM( Mat4& m, const Frame& vp )
{
    if (_fabs( vp.w ) < c_FltEpsilon || _fabs( vp.h ) < c_FltEpsilon )
    {
        return false;
    }
    m.e00 = 2.0f/vp.w;
    m.e01 = 0.0f;
    m.e02 = 0.0f;
    m.e03 = 0.0f;

    m.e10 = 0.0f;
    m.e11 = -2.0f/vp.h;
    m.e12 = 0.0f;
    m.e13 = 0.0f;

    m.e20 = 0.0f;
    m.e21 = 0.0f;
    m.e22 = 1.0f;
    m.e23 = 0.0f;

    m.e30 = -2.0f*vp.x/vp.w - 1.0f;
    m.e31 =  2.0f*vp.y/vp.h + 1.0f;
    m.e32 = 0.0f;
    m.e33 = 1.0f;
    return true;
} // ScreenToProjTM

bool ProjToScreenTM( Mat4& m, const Frame& vp )
{
    m.e00 = vp.w*0.5f;
    m.e01 = 0.0f;
    m.e02 = 0.0f;
    m.e03 = 0.0f;

    m.e10 = 0.0f;
    m.e11 = -vp.h*0.5f;
    m.e12 = 0.0f;
    m.e13 = 0.0f;

    m.e20 = 0.0f;
    m.e21 = 0.0f;
    m.e22 = 1.0f;
    m.e23 = 0.0f;

    m.e30 = vp.w*0.5f + vp.x;
    m.e31 = vp.h*0.5f + vp.y;
    m.e32 = 0.0f;
    m.e33 = 1.0f;
    return true;
} // ProjToScreenTM

Mat4 IdentityMatrix()
{
    return Mat4( 1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f );
}

float GetProjectedOBBArea( const Mat4& obb, const Mat4& projTM, const Vec3& eyePos )
{
    return 0.0f;
}


//****************************************************************************/
/*  Mat4 implementation
//****************************************************************************/
Mat4 Mat4::identity = Mat4( 1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f );

Mat4 Mat4::null = Mat4( 0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f );

float Mat4::inverse( const Mat4& m )
{
    float t[12];
    float s[16];
    const float* mat = m.buf();
    float* r = buf();
    float det = 0.0f;
    for ( int i = 0; i < 4; i++)
    {
        s[i		] = mat[i*4    ];
        s[i + 4	] = mat[i*4 + 1];
        s[i + 8	] = mat[i*4 + 2];
        s[i + 12] = mat[i*4 + 3];
    }

    //  co-factors
    t[0] = s[10]*s[15]; t[1] = s[11]*s[14]; t[2] = s[9]*s[15]; t[3] = s[11]*s[13];
    t[4] = s[9] *s[14]; t[5] = s[10]*s[13]; t[6] = s[8]*s[15]; t[7] = s[11]*s[12];
    t[8] = s[8] *s[14]; t[9] = s[10]*s[12]; t[10]= s[8]*s[13];t[11] = s[9]*s[12];

    r[0] =	t[0]*s[5] + t[3]*s[6] + t[4] *s[7];
    r[0] -=	t[1]*s[5] + t[2]*s[6] + t[5] *s[7];
    r[1] =	t[1]*s[4] + t[6]*s[6] + t[9] *s[7];
    r[1] -=	t[0]*s[4] + t[7]*s[6] + t[8] *s[7];
    r[2] =	t[2]*s[4] + t[7]*s[5] + t[10]*s[7];
    r[2] -=	t[3]*s[4] + t[6]*s[5] + t[11]*s[7];
    r[3] =	t[5]*s[4] + t[8]*s[5] + t[11]*s[6];
    r[3] -=	t[4]*s[4] + t[9]*s[5] + t[10]*s[6];
    r[4] =	t[1]*s[1] + t[2]*s[2] + t[5] *s[3];
    r[4] -=	t[0]*s[1] + t[3]*s[2] + t[4] *s[3];
    r[5] =	t[0]*s[0] + t[7]*s[2] + t[8] *s[3];
    r[5] -=	t[1]*s[0] + t[6]*s[2] + t[9] *s[3];
    r[6] =	t[3]*s[0] + t[6]*s[1] + t[11]*s[3];
    r[6] -=	t[2]*s[0] + t[7]*s[1] + t[10]*s[3];
    r[7] =	t[4]*s[0] + t[9]*s[1] + t[10]*s[2];
    r[7] -=	t[5]*s[0] + t[8]*s[1] + t[11]*s[2];

    t[0]	= s[2]*s[7];
    t[1]	= s[3]*s[6];
    t[2]	= s[1]*s[7];
    t[3]	= s[3]*s[5];
    t[4]	= s[1]*s[6];
    t[5]	= s[2]*s[5];
    t[6]	= s[0]*s[7];
    t[7]	= s[3]*s[4];
    t[8]	= s[0]*s[6];
    t[9]	= s[2]*s[4];
    t[10] = s[0]*s[5];
    t[11] = s[1]*s[4];

    r[8] =	t[0] *s[13] + t[3]*s[14] + t[4]*s[15];
    r[8] -=	t[1] *s[13] + t[2]*s[14] + t[5]*s[15];
    r[9] =	t[1] *s[12] + t[6]*s[14] + t[9]*s[15];
    r[9] -=	t[0] *s[12] + t[7]*s[14] + t[8]*s[15];
    r[10] =	t[2] *s[12] + t[7]*s[13] + t[10]*s[15];
    r[10]-=	t[3] *s[12] + t[6]*s[13] + t[11]*s[15];
    r[11] =	t[5] *s[12] + t[8]*s[13] + t[11]*s[14];
    r[11]-=	t[4] *s[12] + t[9]*s[13] + t[10]*s[14];
    r[12] =	t[2] *s[10] + t[5]*s[11] + t[1]*s[9];
    r[12]-=	t[4] *s[11] + t[0]*s[9]  + t[3]*s[10];
    r[13] =	t[8] *s[11] + t[0]*s[8]  + t[7]*s[10];
    r[13]-=	t[6] *s[10] + t[9]*s[11] + t[1]*s[8];
    r[14] =	t[6] *s[9]  + t[11]*s[11]+ t[3]*s[8];
    r[14]-=	t[10]*s[11] + t[2] *s[8] + t[7]*s[9];
    r[15] =	t[10]*s[10] + t[4] *s[8] + t[9]*s[9];
    r[15]-=	t[8] *s[9]  + t[11]*s[10]+ t[5]*s[8];

    //  determinant
    det=s[0]*r[0] + s[1]*r[1] + s[2]*r[2] + s[3]*r[3];
    //  matrix inverse
    float idet = 1.0f;
    if (_fabs( det ) >= c_FltEpsilon) idet = 1.0f/det;
    for (int j = 0; j < 16; j++) r[j] *= idet;

    return det;
} // Mat4::inverse

void Mat4::srt( float scale, const Vec3& axis, float angle, const Vec3& t )
{
    Mat3 scTM;
    scTM.scaling( Vec3( scale, scale, scale ) );
    Mat3 rotTM;
    rotTM.rot( axis, angle );
    Mat3 tm;
    tm.mul( scTM, rotTM );
    *this = Mat4( tm, t );
} // Mat4::srt

void Mat4::srt( const Vec3& sc, const Vec3& axis, float angle, const Vec3& tr )
{
    Mat3 scTM;
    scTM.scaling( sc );
    Mat3 rotTM;
    rotTM.rot( axis, angle );
    Mat3 tm;
    tm.mul( scTM, rotTM );
    *this = Mat4( tm, tr );
} // Mat4::srt

void Mat4::rot( const Vec3& axis, float angle )
{
    Mat3 rotTM;
    rotTM.rot( axis, angle );
    *this = rotTM;
} // Mat4::rot

void Mat4::factorize( Vec3& scale, Quaternion& rot, Vec3& trans ) const
{
    Mat3 rotTM;
    factorize( scale, rotTM, trans );
    rot = rotTM;
} // Mat4::factorize

void Mat4::factorize( Vec3& scale, Mat3& rot, Vec3& trans ) const
{
    rot = *this;
    scale.x = rot.v0().normalize();
    scale.y = rot.v1().normalize();
    scale.z = rot.v2().normalize();

    trans.x = e30;
    trans.y = e31;
    trans.z = e32;

    //  check for negative scale
    float d = rot.determinant();
    if (d < 0.0f)
    {
        scale.x = -scale.x;
        rot.v0().reverse();
    }
} // Mat4::factorize

void Mat4::srt( float sc, const Mat3& rot, const Vec3& tr )
{
    e00 = rot.e00 * sc;	e01 = rot.e01 * sc;	e02 = rot.e02 * sc;	e03 = 0.0f;
	e10 = rot.e10 * sc;	e11 = rot.e11 * sc;	e12 = rot.e12 * sc;	e13 = 0.0f;
	e20 = rot.e20 * sc;	e21 = rot.e21 * sc;	e22 = rot.e22 * sc;	e23 = 0.0f;
    e30 = tr.x;			e31 = tr.y;		    e32 = tr.z;			e33 = 1.0f;
} // Mat4::srt

void Mat4::srt( const Vec3& sc, const Mat3& rot, const Vec3& tr )
{
    e00 = rot.e00 * sc.x;	e01 = rot.e01 * sc.x;	e02 = rot.e02 * sc.x;	e03 = 0.0f;
    e10 = rot.e10 * sc.y;	e11 = rot.e11 * sc.y;	e12 = rot.e12 * sc.y;	e13 = 0.0f;
    e20 = rot.e20 * sc.z;	e21 = rot.e21 * sc.z;	e22 = rot.e22 * sc.z;	e23 = 0.0f;
    e30 = tr.x;			    e31 = tr.y;		        e32 = tr.z;			    e33 = 1.0f;
} // Mat4::srt

void Mat4::shearXY( float a, float b )
{
    e00 = 1.0f; e01 = 0.0f; e02 = 0.0f; e03 = 0.0f;
    e10 = 0.0f; e11 = 1.0f; e12 = 0.0f; e13 = 0.0f;
    e20 = a;    e21 = b;    e22 = 1.0f; e23 = 0.0f;
    e30 = 0.0f; e31 = 0.0f; e32 = 0.0f; e33 = 1.0f;
} // Mat4::shearXY



