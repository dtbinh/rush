#include "stdafx.h"
#include "Mat4.h"
#include "Quaternion.h"

Mat4 Convert( const D3DXMATRIX& tm )
{
    Mat4 m;
    m.e00 = tm._11; m.e01 = tm._12; m.e02 = tm._13; m.e03 = tm._14;
    m.e10 = tm._21; m.e11 = tm._22; m.e12 = tm._23; m.e13 = tm._24;
    m.e20 = tm._31; m.e21 = tm._32; m.e22 = tm._33; m.e23 = tm._34;
    m.e30 = tm._41; m.e31 = tm._42; m.e32 = tm._43; m.e33 = tm._44;
    return m;
}

D3DXMATRIX Convert( const Mat4& tm )
{
    D3DXMATRIX m;
    m._11 = tm.e00; m._12 = tm.e01; m._13 = tm.e02; m._14 = tm.e03;
    m._21 = tm.e10; m._22 = tm.e11; m._23 = tm.e12; m._24 = tm.e13;
    m._31 = tm.e20; m._32 = tm.e21; m._33 = tm.e22; m._34 = tm.e23;
    m._41 = tm.e30; m._42 = tm.e31; m._43 = tm.e32; m._44 = tm.e33;
    return m;
}

Quaternion Convert( const D3DXQUATERNION& quat )
{
    return Quaternion( quat.x, quat.y, quat.z, quat.w );
}

D3DXQUATERNION Convert( const Quaternion& quat )
{
    D3DXQUATERNION q;
    q.x = quat.v.x;
    q.y = quat.v.y;
    q.z = quat.v.z;
    q.w = quat.s;
    return q;
}

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

    D3DXQUATERNION dxq;
    D3DXVECTOR3 dxScale, dxPos;
    D3DXMATRIX dxTM = Convert( tm );
    D3DXMatrixDecompose( &dxScale, &dxq, &dxPos, &dxTM );
    CHECK( rot.is_equal( Convert( dxq ), c_CheckEps ) );


    Mat3 rotTM( tm );
    rotTM.v0().normalize();
    rotTM.v1().normalize();
    rotTM.v2().normalize();
    
    D3DXMATRIX dxRotTM = Convert( Mat4( rotTM ) );
    D3DXQuaternionRotationMatrix( &dxq, &dxRotTM );
    Quaternion checkQ = Convert( dxq );
    CHECK( checkQ.is_equal( Quaternion( rotTM ), c_CheckEps ) );

    D3DXMatrixRotationQuaternion( &dxRotTM, &dxq );
    checkTM = checkQ;
    CHECK( checkTM.is_equal( Convert( dxRotTM ), c_CheckEps ) );

    Mat3 chInvTM( 0.54f, -10.45f, 7.0f, -30.f, 0.0f, -1.0f, 56.0f, 123.56f, -11.0f );
    Mat3 chInvTM1 = chInvTM;
    chInvTM1.invert();
    chInvTM1.invert();
    CHECK( chInvTM.is_equal( chInvTM1, 0.001f ) );
}

