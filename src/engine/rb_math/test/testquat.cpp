#include "precompile.h"
#include "mat4.h"
#include "quaternion.h"


const float c_CheckEps = 0.00001f;
TEST( TestQuat )
{
    Vec3 pos  ( -4325.45f, 53.0f, 932.677f );
    Vec3 scale( 2.0f, 1.0f, 0.5f );
    Vec3 axis ( 0.1f, 0.5f, 0.2f );
    axis.normalize();
    Quaternion rot( axis, 0.62f );

    Mat4 tm;
    tm.srt( scale, rot, pos );

    Vec3 pos1, scale1;
    Quaternion rot1;

    tm.factorize( scale1, rot1, pos1 );
    
    CHECK( scale.is_equal( scale1, c_CheckEps ) );
    CHECK( pos.is_equal( pos1, c_CheckEps ) );
    CHECK( rot.is_equal( rot1, c_CheckEps ) );

    tm = Mat4( 1.0556698f, 0.0f, 0.0f, 0.0f,
               0.0f, -0.80266404f, -0.59643137f, 0.0f,
               0.0f, -0.59643137f, 0.80266404f, 0.0f,
               -7.4450092f, 34.438614f, 99.730240f, 1.0f );
    tm.factorize( scale, rot, pos );
    Mat4 checkTM;
    checkTM.srt( scale, rot, pos );
    CHECK( tm.is_equal( checkTM, c_CheckEps ) );

    Mat3 chInvTM( 0.54f, -10.45f, 7.0f, -30.f, 0.0f, -1.0f, 56.0f, 123.56f, -11.0f );
    Mat3 chInvTM1 = chInvTM;
    chInvTM1.invert();
    chInvTM1.invert();
    CHECK( chInvTM.is_equal( chInvTM1, 0.001f ) );
}

