#include "stdafx.h"
#include "mat4.h"


const float c_CheckEps = 0.00001f;
TEST( TestProj )
{
    float fovx   = 0.23f;
    float zn     = 0.1f;
    float zf     = 10.0f;
    float aspect = 0.5f;

    Mat4 projTM;
    bool bRes = PerspProjTM( projTM, fovx, zn, zf, aspect );
    CHECK_EQUAL( bRes, true );

    float fovx1, aspect1, zn1, zf1;
    bRes = FactorPerspProjTM( projTM, fovx1, zn1, zf1, aspect1 );
    CHECK_EQUAL( bRes, true );

    CHECK( fabs( zn1 - zn ) < c_CheckEps );
    CHECK( fabs( zf1 - zf ) < 0.0001f );
    CHECK( fabs( aspect1 - aspect ) < c_CheckEps );
    CHECK( fabs( fovx1 - fovx ) < c_CheckEps );
}

