#include "stdafx.h"
#include "JCamera.h"

const float c_CamEps = 1e-4;
TEST( CameraProjection )
{
    JCamera* pCamera = new JCamera();

    float zn = 0.1;
    pCamera->SetOrtho( true );
    pCamera->SetZNear( zn );
    CHECK( equalf( zn, pCamera->GetZNear(), c_CamEps ) );

    float zf = 10.0;
    pCamera->SetZFar( zf );
    CHECK( equalf( zf, pCamera->GetZFar(), c_CamEps ) );
    CHECK( equalf( zn, pCamera->GetZNear(), c_CamEps ) );

    float aspect = 5.0f/2.0f;
    pCamera->SetAspect( aspect );
    CHECK( equalf( aspect, pCamera->GetAspect(), c_CamEps ) );
    CHECK( equalf( zf, pCamera->GetZFar(), c_CamEps ) );
    CHECK( equalf( zn, pCamera->GetZNear(), c_CamEps ) );
 
    float fov = 100.0f;
    pCamera->SetFOVx( fov );
    CHECK( equalf( aspect, pCamera->GetAspect(), c_CamEps ) );
    CHECK( equalf( zf, pCamera->GetZFar(), c_CamEps ) );
    CHECK( equalf( zn, pCamera->GetZNear(), c_CamEps ) );
    CHECK( equalf( fov, pCamera->GetFOVx(), c_CamEps ) );

    pCamera->SetOrtho( false );

    pCamera->SetZNear( zn );
    CHECK( equalf( zn, pCamera->GetZNear(), c_CamEps ) );

    pCamera->SetZFar( zf );
    CHECK( equalf( zf, pCamera->GetZFar(), c_CamEps ) );
    CHECK( equalf( zn, pCamera->GetZNear(), c_CamEps ) );

    pCamera->SetAspect( aspect );
    CHECK( equalf( aspect, pCamera->GetAspect(), c_CamEps ) );
    CHECK( equalf( zf, pCamera->GetZFar(), c_CamEps ) );
    CHECK( equalf( zn, pCamera->GetZNear(), c_CamEps ) );


    pCamera->Release();
}

TEST( CameraConversion )
{
    JCamera* pCamera = new JCamera();

    pCamera->SetOrtho( false );


    pCamera->Release();
}

TEST( CameraTransform )
{
    JCamera* pCamera = new JCamera();

    pCamera->SetOrtho ( false );
    pCamera->SetZNear ( 1.0f );
    pCamera->SetZFar  ( 1000.0f );
    pCamera->SetAspect( 1.0f );
    
    pCamera->SetPos( Vec3( 10.0f, 10.0f, 10.0f ) );
    pCamera->SetViewUp ( Vec3( 0.0f, 0.0f, 1.0f ) );
    pCamera->SetViewDir( Vec3( -1.0f, -1.0f, -1.0f ) );
    
    Mat4 tm;
    pCamera->GetTM( Space_World, Space_Projection, tm );

    Vec4 pt( 0.0f, 0.0f, 0.0f, 1.0f );

    tm.tm( pt );
    pt.normW();

    CHECK( IsZero( pt.x, 1e-06f ) );
    CHECK( IsZero( pt.y, 1e-06f ) );
    CHECK( pt.z > 0.0f && pt.z < 1.0f );

    pCamera->Release();
}
