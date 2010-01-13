//****************************************************************************/
//  File:	Warping.cpp
//  Desc:	 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	17-03-2005
//****************************************************************************/
#include "precompile.h"
#include "Mat3.h"
#include "Vec2.h"

Mat3 UnitSquareToQuadTM( const Vec2& lt, const Vec2& rt, const Vec2& rb, const Vec2& lb )
{
    float sx  = lt.x - rt.x + rb.x - lb.x;
    float sy  = lt.y - rt.y + rb.y - lb.y;
    
    Mat3 m;

    if (_fabs( sx ) < c_FltEpsilon && _fabs( sy ) < c_FltEpsilon)
    {
        //  quad is parallelogram
        m.e00 = rt.x - lt.x;
        m.e10 = rb.x - rt.x;
        m.e20 = lt.x;
        
        m.e01 = rt.y - lt.y;
        m.e11 = rb.y - rt.y;
        m.e21 = lt.y;    

        m.e02 = 0.0f;
        m.e12 = 0.0f;
        m.e22 = 1.0f;
        return m;
    }

    float dx1 = rt.x - rb.x;
    float dx2 = lb.x - rb.x;
    float dy1 = rt.y - rb.y;
    float dy2 = lb.y - rb.y;

    float denom = dx1*dy2 - dy1*dx2;
    float g = (sx*dy2 - sy*dx2)/denom;
    float h = (dx1*sy - dy1*sx)/denom;

    m.e00 = rt.x - lt.x + g*rt.x;
    m.e10 = lb.x - lt.x + h*lb.x;
    m.e20 = lt.x;

    m.e01 = rt.y - lt.y + g*rt.y;
    m.e11 = lb.y - lt.y + h*lb.y;
    m.e21 = lt.y;    

    m.e02 = g;
    m.e12 = h;
    m.e22 = 1.0f;

    return m;
} // UnitSquareToQuadTM