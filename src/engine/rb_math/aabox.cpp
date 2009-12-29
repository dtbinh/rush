//****************************************************************************/
//  File:  AABox.cpp
//****************************************************************************/
#include "stdafx.h"
#include "AABox.h"
#include "Frame.h"

//****************************************************************************/
/*  AABox implementation  
//****************************************************************************/
AABox::AABox( const Vec3& c, float dx, float dy, float dz )
{
    minv = c;
    maxv = c;

    minv.x -= dx*0.5f; 
    minv.y -= dy*0.5f; 
    minv.z -= dz*0.5f;

    maxv.x += dx*0.5f; 
    maxv.y += dy*0.5f; 
    maxv.z += dz*0.5f;
} // AABox::AABox

AABox::AABox( const Vec3& c, float side )
{
    minv = c;
    maxv = c;
    float hs = side*0.5f;

    minv.x -= hs; 
    minv.y -= hs; 
    minv.z -= hs;

    maxv.x += hs; 
    maxv.y += hs; 
    maxv.z += hs;
} // AABox::AABox

AABox::AABox( const Frame& base, float minz, float maxz )
{
    minv.x = base.x; 
    minv.y = base.y; 
    minv.z = minz;

    maxv.x = base.r();
    maxv.y = base.b();
    maxv.z = maxz;
} // AABox::AABox

void AABox::Invalidate()
{
    minv = Vec3( FLT_MAX, FLT_MAX, FLT_MAX );
    maxv = -minv;
} // AABox::Invalidate

void AABox::Transform( const Mat4& tm )
{
    AABox res;
    res.Invalidate();

    Vec3 pt;
    pt = xyz();
    tm.tmpt( pt );
    res.AddPoint( pt );

    pt = xyZ();
    tm.tmpt( pt );
    res.AddPoint( pt );

    pt = xYz();
    tm.tmpt( pt );
    res.AddPoint( pt );

    pt = xYZ();
    tm.tmpt( pt );
    res.AddPoint( pt );

    pt = Xyz();
    tm.tmpt( pt );
    res.AddPoint( pt );

    pt = XyZ();
    tm.tmpt( pt );
    res.AddPoint( pt );

    pt = XYz();
    tm.tmpt( pt );
    res.AddPoint( pt );

    pt = XYZ();
    tm.tmpt( pt );
    res.AddPoint( pt );
    
    *this = res;
} // AABox::Transform
