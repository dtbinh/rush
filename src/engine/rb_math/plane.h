//****************************************************************************/
//  File:  Plane.h
//  Desc:  3D Plane
//****************************************************************************/
#ifndef __PLANE_H__
#define __PLANE_H__

#include "vec3.h"
class Mat4;

//****************************************************************************/
//  Class:  Plane
//  Desc:   3D plane, described by ax+by+cz+d=0
//****************************************************************************/
class Plane
{
public:
    float           a;
    float           b;
    float           c;
    float           d;

                    Plane           (){}
                    Plane           ( float pa, float pb, float pc, float pd );
                    Plane           ( const Vec3& pt, const Vec3& norm );
                    Plane           ( const Vec3& v1, const Vec3& v2, const Vec3& v3 );
    bool            Intersect       ( const Plane& p1, const Plane& p2, Vec3& pt ) const;
    Vec3&           Normal          ();
    const Vec3&     Normal          () const;
    void            Normalize       ();
    bool            OnPositiveSide  ( const Vec3& v ) const;
    bool            ContainsPoint   ( const Vec3& v, float eps = c_Epsilon ) const;

    bool            IntersectRay    ( const Vec3& orig, const Vec3& dir, Vec3& pt ) const;
    void            Transform       ( const Mat4& tm );

    static Plane    xOy;
    static Plane    xOz;
    static Plane    yOz;

}; // class Plane

//****************************************************************************/
//  Plane inlines
//****************************************************************************/
inline Plane::Plane( float pa, float pb, float pc, float pd )
    :   a( pa ),
        b( pb ),
        c( pc ),
        d( pd )
{}

inline Plane::Plane( const Vec3& pt, const Vec3& norm )
{
    a = norm.x;
    b = norm.y;
    c = norm.z;
    d = -pt.dot( norm );
}

inline Plane::Plane( const Vec3& v1, const Vec3& v2, const Vec3& v3 )
{
    Vec3 av = v1 - v2;
    Vec3 bv = v3 - v2;
    Vec3& n = Normal();

    n.cross( av, bv );
    float area = n.normalize();
    d = -n.dot( v2 );
}

inline Vec3& Plane::Normal()
{
    return *((Vec3*)( &a ));
}

inline const Vec3& Plane::Normal() const
{
    return *((Vec3*)( &a ));
}

inline void Plane::Normalize()
{
   float n = Normal().norm();
   a /= n;
   b /= n;
   c /= n;
   d /= n;
}

inline bool Plane::OnPositiveSide( const Vec3& v ) const
{
    return a*v.x + b*v.y + c*v.z + d >= 0.0f;
}

inline bool Plane::ContainsPoint( const Vec3& v, float eps ) const
{
    return _fabs( a*v.x + b*v.y + c*v.z + d ) < eps;
}


#endif // __PLANE_H__

