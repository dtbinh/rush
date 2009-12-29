//****************************************************************************/
//  File:  Plane.cpp
//****************************************************************************/
#include "stdafx.h"
#include "MathConst.h"
#include "Plane.h"

//****************************************************************************/
/*  Plane implementation
//****************************************************************************/
Plane    Plane::xOy = Plane( 0.0f, 0.0f, 1.0f, 0.0f );
Plane    Plane::xOz = Plane( 0.0f, 1.0f, 0.0f, 0.0f );
Plane    Plane::yOz = Plane( 1.0f, 0.0f, 0.0f, 0.0f );

bool Plane::Intersect( const Plane& p1, const Plane& p2, Vec3& pt ) const
{
    Mat3 m;
    m.e00 = a;
    m.e01 = p1.a;
    m.e02 = p2.a;

    m.e10 = b;
    m.e11 = p1.b;
    m.e12 = p2.b;

    m.e20 = c;
    m.e21 = p1.c;
    m.e22 = p2.c;

    float det = m.invert();
    if (_fabs( det ) < c_FltEpsilon) return false;

    pt.x = -d*m.e00 - p1.d*m.e10 - p2.d*m.e20;
    pt.y = -d*m.e01 - p1.d*m.e11 - p2.d*m.e21;
    pt.z = -d*m.e02 - p1.d*m.e12 - p2.d*m.e22;
    return true;
} // Plane::Intersect

bool Plane::IntersectRay( const Vec3& orig, const Vec3& dir, Vec3& pt ) const
{
    float den = a*dir.x + b*dir.y + c*dir.z;
    if (_fabs( den ) < c_FltEpsilon)
    {
        return false;
    }
    float t = -(d + a*orig.x + b*orig.y + c*orig.z)/den;
    if (t < 0.0f)
    {
        return false;
    }
    pt = orig + t*dir;
    return true;
} // Plane::IntersectRay

void Plane::Transform( const Mat4& tm )
{
    Vec4 v( a, b, c, d );
    transpose( inverse( tm ) ).tm( v );
    
    a = v.x;
    b = v.y;
    c = v.z;
    d = v.w;
    Normalize();
}

