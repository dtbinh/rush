//****************************************************************************/
//  File:  Mat3.cpp
//  Desc:  4x4 Matrix
//****************************************************************************/
#include "precompile.h"
#include "vec2.h"
#include "mat3.h"
#include "quaternion.h"
#include "mathconst.h"

//****************************************************************************/
/*  Mat3 implementation
//****************************************************************************/
Mat3 Mat3::identity = Mat3( 1.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f );
Mat3 Mat3::null = Mat3( 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f );

Mat3::Mat3( const Mat4& m )
{
    e00 = m.e00; e01 = m.e01; e02 = m.e02;
    e10 = m.e10; e11 = m.e11; e12 = m.e12;
    e20 = m.e20; e21 = m.e21; e22 = m.e22;
}

Mat3::Mat3( const Vec3& axis, float angle )
{
    Quaternion q( axis, angle );
    *this = q;
}

Mat3::Mat3( const Quaternion& q )
{
    float xx, yy, zz, xy, yz, xz, sx, sy, sz;
    xx = 2.0f * q.v.x * q.v.x;
    yy = 2.0f * q.v.y * q.v.y;
    zz = 2.0f * q.v.z * q.v.z;
    xy = 2.0f * q.v.x * q.v.y;
    yz = 2.0f * q.v.y * q.v.z;
    xz = 2.0f * q.v.x * q.v.z;
    sx = 2.0f * q.s   * q.v.x;
    sy = 2.0f * q.s	  * q.v.y;
    sz = 2.0f * q.s   * q.v.z;

    e00 = 1.0f - yy - zz;
    e01 = xy + sz;
    e02 = xz - sy;

    e10 = xy - sz;
    e11 = 1.0f - xx - zz;
    e12 = yz + sx;

    e20 = xz + sy;
    e21 = yz - sx;
    e22 = 1.0f - xx - yy;
}//  Mat3::Mat3

float Mat3::determinant() const
{
    return  e00*(e11*e22 - e21*e12) - e01*(e10*e22 - e20*e12) + e02*(e10*e21 - e20*e11);
} // Mat3::determinant

float Mat3::inverse( const Mat3& tm )
{
    const float* m   = tm.buf();
    float*       r   = buf();
    float det = tm.determinant();
	if (_fabs( det ) < c_FltEpsilon) return false;

	r[0] = (-m[5]*m[7] + m[4]*m[8])/det;
	r[1] = ( m[2]*m[7] - m[1]*m[8])/det;
	r[2] = (-m[2]*m[4] + m[1]*m[5])/det;

	r[3] = ( m[5]*m[6] - m[3]*m[8])/det;
	r[4] = (-m[2]*m[6] + m[0]*m[8])/det;
	r[5] = ( m[2]*m[3] - m[0]*m[5])/det;

	r[6] = (-m[4]*m[6] + m[3]*m[7])/det;
	r[7] = ( m[1]*m[6] - m[0]*m[7])/det;
	r[8] = (-m[1]*m[3] + m[0]*m[4])/det;

    return det;
} // Mat3::inverse

void Mat3::rot( const Vec3& axis, float angle )
{
    *this = Mat3( Quaternion( axis, angle ) );
} // Mat3::rot

void Mat3::from_euler( float phi, float theta, float psi )
{
    float sphi,cphi,stheta,ctheta,spsi,cpsi;
    sphi   = sinf( phi   );
    cphi   = cosf( phi   );
    stheta = sinf( theta );
    ctheta = cosf( theta );
    spsi   = sinf( psi   );
    cpsi   = cosf( psi   );

    e00 =    cpsi*ctheta;
    e01 =    spsi*ctheta;
    e02 =   -stheta;
    e10 =    cpsi*stheta*sphi - spsi*cphi;
    e11 =    spsi*stheta*sphi + cpsi*cphi;
    e12 =    ctheta*sphi;
    e20 =    cpsi*stheta*cphi + spsi*sphi;
    e21 =    spsi*stheta*cphi - cpsi*sphi;
    e22 =    ctheta*cphi;
} // Mat3::from_euler

Vec3 Mat3::to_euler() const
{
    Vec3 theta;

    float ce02 = clamp( e02, -1.0f, 1.0f );
    theta.y = asinf( ce02 );
    if (theta.y < c_HalfPI)
    {
        if (theta.y > -c_HalfPI)
        {
            theta.x = atan2f( -e12, e22 );
            theta.z = atan2f( -e01, e00 );
        }
        else
        {
            theta.x = -atan2f( e10, e11 );
            theta.z = 0.0f;
        }
    }
    else
    {
        theta.x = atan2f( e10, e11 );
        theta.z = 0.0f;
    }
    theta.reverse();
    return theta;
} // Mat3::to_euler

void Mat3::srt( float s, float angle, const Vec2& trans )
{
    float ss = sinf( angle );
    float cs = cosf( angle );
    e00 = s*cs;     e01 = s*ss;     e02 = 0.0f;
    e10 = -s*ss;    e11 = s*cs;     e12 = 0.0f;
    e20 = trans.x;  e21 = trans.y;  e22 = 1.0f;
} // Mat3::srt

void Mat3::srt( const Vec2& s, float angle, const Vec2& trans )
{
    float ss = sinf( angle );
    float cs = cosf( angle );
    e00 = s.x*cs;   e01 = s.x*ss;   e02 = 0.0f;
    e10 = -s.y*ss;  e11 = s.y*cs;   e12 = 0.0f;
    e20 = trans.x;  e21 = trans.y;  e22 = 1.0f;
} // Mat3::srt
