#include "stdafx.h"
#include "MathConst.h"
#include "Mat4.h"
#include "Plane.h"

TEST( PlaneTransform )
{
    Vec3 pos( 0.1f, 0.2f, 0.2f );
    Vec3 norm( 0.2f, 0.3f, -0.1f );
    norm.normalize();
    Plane plane( pos, norm );
    
    float res = pos.x*plane.a + pos.y*plane.b + pos.z*plane.c + plane.d;
    CHECK( fabs( res ) < c_Epsilon );

    CHECK( plane.ContainsPoint( pos, c_Epsilon ) );
    CHECK( !plane.ContainsPoint( Vec3( 40.0f, -10.0f, 10.0f ), c_Epsilon ) );

    Mat4 tm;
    tm.srt( Vec3( 1.0f, 2.0f, 2.0f ), Vec3::oX, 30.0f, Vec3( 0.3f, 0.1f, -1.0f ) );
    Mat4 ptm;
    PerspProjTM( ptm, 0.3f, 1.0f, 1000.0f, 1.3f );
    tm *= ptm;

    Plane tplane = plane;
    tplane.Transform( tm ); 
    
    Vec3 tpos( pos );
    tm.tmproj( tpos );
    CHECK( tplane.ContainsPoint( tpos, c_Epsilon ) );
    CHECK( !tplane.ContainsPoint( pos, c_Epsilon ) );

    Vec3 planePt( 7.0f, -10.0f, 0.0f );
    planePt.z = -(plane.a*planePt.x + plane.b*planePt.y + plane.d)/plane.c;
    CHECK( plane.ContainsPoint( planePt, c_Epsilon ) );
    
    tm.tmproj( planePt );
    CHECK( tplane.ContainsPoint( planePt, c_Epsilon ) );
    CHECK( !plane.ContainsPoint( planePt, c_Epsilon ) );
    
    tplane.Transform( inverse( tm ) );
    CHECK( equalf( plane.a, tplane.a, c_Epsilon ) );
    CHECK( equalf( plane.b, tplane.b, c_Epsilon ) );
    CHECK( equalf( plane.c, tplane.c, c_Epsilon ) );
    CHECK( equalf( plane.d, tplane.d, c_Epsilon ) );
}
