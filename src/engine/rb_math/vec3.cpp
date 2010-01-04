//****************************************************************************/
//  File:  Vec3.cpp
//  Desc:  3D vector
//****************************************************************************/
#include "precompile.h"
#include "vec3.h"
#include "rand.h"
#include "mathconst.h"

//****************************************************************************/
/*	Vec3 implementation
//****************************************************************************/
Vec3 Vec3::null = Vec3( 0.0f, 0.0f, 0.0f );
Vec3 Vec3::one  = Vec3( 1.0f, 1.0f, 1.0f );
Vec3 Vec3::oX   = Vec3( 1.0f, 0.0f, 0.0f );
Vec3 Vec3::oY   = Vec3( 0.0f, 1.0f, 0.0f );
Vec3 Vec3::oZ   = Vec3( 0.0f, 0.0f, 1.0f );

void Vec3::random( float minX, float maxX, float minY, float maxY, float minZ, float maxZ )
{
    x = frand( minX, maxX );
    y = frand( minY, maxY );
    z = frand( minZ, maxZ );
}

bool Vec3::ortonormalize( Vec3& a, Vec3& b )
 {
     normalize();
     a.normalize();
     b.cross( *this, a );
     b.normalize();
     a.cross( b, *this );

     return (_fabs( norm2() ) > c_FltEpsilon &&
        _fabs( a.norm2() ) > c_FltEpsilon &&
        _fabs( b.norm2() ) > c_FltEpsilon);
 } // Vec3::ortonormalize
