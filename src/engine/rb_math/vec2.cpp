//****************************************************************************/
//  File:  Vec2.cpp
//  Desc:  3D vector
//****************************************************************************/
#include "stdafx.h"
#include "Frame.h"
#include "Vec2.h"

//****************************************************************************/
/*	Vec2 implementation
//****************************************************************************/
Vec2 Vec2::null = Vec2( 0.0f, 0.0f );
Vec2 Vec2::one  = Vec2( 1.0f, 1.0f );
Vec2 Vec2::oX   = Vec2( 1.0f, 0.0f );
Vec2 Vec2::oY   = Vec2( 0.0f, 1.0f );

void Vec2::remap( const Frame& from, const Frame& to )
{
    x = to.x + to.w*((x - from.x)/from.w);
    y = to.y + to.h*((y - from.y)/from.h);
} // Vec2::remap

void Vec2::rotate( float ang )
{
    float cs = cosf( ang );
    float ss = sinf( ang );
    
    float cx = x*cs - y*ss;
    float cy = x*ss + y*cs;
    
    x = cx;
    y = cy;
} // Vec2::rotate

