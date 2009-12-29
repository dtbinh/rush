#include "stdafx.h"
#include "Vec2.h"
#include "Mat3.h"
#include "Warping.h"

TEST( Warping )
{
    Vec2 lt( -0.5f,  1.0f );
    Vec2 rt(  0.5f,  1.0f );
    Vec2 rb(  1.0f, -1.0f );
    Vec2 lb( -1.0f, -1.0f );

    Mat3 tm = UnitSquareToQuadTM( lt, rt, rb, lb );
    tm.invert();
    
    Vec3 c( 0.0f, 0.0f, 1.0f );
    tm.tm( c );
    c /= c.z;

    CHECK( (c - Vec3( 0.5f, 2.0f/3.0f, 1.0f )).norm() < c_FltEpsilon );
}