/***********************************************************************************/
//  File:   Spline2.h
//  Date:   21.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __SPLINE2_H__
#define __SPLINE2_H__

#include "vec2.h"
Vec2 Hermite( const Vec2& p0, const Vec2& t0, const Vec2& p1, const Vec2& t1, float t );

#endif //__SPLINE2_H__
