//****************************************************************************/
//  File:  Frustum.cpp
//  Desc:
//****************************************************************************/
#include "stdafx.h"
#include "mat4.h"
#include "mathconst.h"
#include "frustum.h"

//****************************************************************************/
/*  Frustum implementation
//****************************************************************************/
void Frustum::FromCameraMatrix( const Mat4& cm )
{
    plLeft.a = cm.e03 + cm.e00;
    plLeft.b = cm.e13 + cm.e10;
    plLeft.c = cm.e23 + cm.e20;
    plLeft.d = cm.e33 + cm.e30;

    plRight.a = cm.e03 - cm.e00;
    plRight.b = cm.e13 - cm.e10;
    plRight.c = cm.e23 - cm.e20;
    plRight.d = cm.e33 - cm.e30;

    plBottom.a = cm.e03 + cm.e01;
    plBottom.b = cm.e13 + cm.e11;
    plBottom.c = cm.e23 + cm.e21;
    plBottom.d = cm.e33 + cm.e31;

    plTop.a = cm.e03 - cm.e01;
    plTop.b = cm.e13 - cm.e11;
    plTop.c = cm.e23 - cm.e21;
    plTop.d = cm.e33 - cm.e31;

    plNear.a = cm.e02;
    plNear.b = cm.e12;
    plNear.c = cm.e22;
    plNear.d = cm.e32;

    plFar.a = cm.e03 - cm.e02;
    plFar.b = cm.e13 - cm.e12;
    plFar.c = cm.e23 - cm.e22;
    plFar.d = cm.e33 - cm.e32;

    plLeft.Normalize();
    plRight.Normalize();
    plBottom.Normalize();
    plTop.Normalize();
    plNear.Normalize();
    plFar.Normalize();

    UpdateVertices();
} // Frustum::FromCameraMatrix

bool Frustum::GetTop( Vec3& top ) const
{
    if (plRight.Intersect( plLeft, plTop, top )) return true;
    return false;
} // Frustum::GetTop

bool Frustum::PtIn( const Vec3& pt ) const
{
    if (!plRight    .OnPositiveSide( pt )) return false;
    if (!plLeft     .OnPositiveSide( pt )) return false;
    if (!plTop      .OnPositiveSide( pt )) return false;
    if (!plBottom   .OnPositiveSide( pt )) return false;
    if (!plNear     .OnPositiveSide( pt )) return false;
    if (!plFar      .OnPositiveSide( pt )) return false;
    return true;
} // Frustum::PtIn

void Frustum::UpdateVertices()
{
    plLeft. Intersect( plTop,    plNear,    vLTN );
    plRight.Intersect( plTop,    plNear,    vRTN );
    plLeft. Intersect( plBottom, plNear,    vLBN );
    plRight.Intersect( plBottom, plNear,    vRBN );
    plLeft. Intersect( plTop,    plFar,     vLTF );
    plRight.Intersect( plTop,    plFar,     vRTF );
    plLeft. Intersect( plBottom, plFar,     vLBF );
    plRight.Intersect( plBottom, plFar,     vRBF );
} // Frustum::UpdateVertices
