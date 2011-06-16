//****************************************************************************/
//  File:   Warping.h
//  Desc:    
/*  Author: Ruslan Shestopalyuk
/*  Date:   17-03-2005
//****************************************************************************/
#ifndef __WARPING_H__
#define __WARPING_H__

class Mat3;
class Vec2;

Mat3 UnitSquareToQuadTM( const Vec2& lt, const Vec2& rt, const Vec2& rb, const Vec2& lb );

#endif // __WARPING_H__
