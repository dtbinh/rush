//****************************************************************************/
//  File:  Seg3.cpp
//  Desc:  3D segment
//****************************************************************************/
#include "stdafx.h"
#include "Vec3.h"
#include "Seg3.h"

//****************************************************************************/
/*	Seg3 implementation
//****************************************************************************/
float Seg3::dist( Vec3& v ) const
{
    Vec3 p = v - a;
    Vec3 d = b - a;
    float l = d.normalize();
    float k = p.dot( d );
    if (k < 0.0f) return v.dist( a );
    if (k > l)    return v.dist( b );
    k *= k;
    return sqrtf( p.norm2() - k );
} // Seg3::dist


