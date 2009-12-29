//****************************************************************************/
//  File:  MathUtil.h
//  Desc:  Common math constants
//****************************************************************************/
#ifndef __MATHUTIL_H__
#define __MATHUTIL_H__
#pragma once

inline float sgnf( float val )
{
    if (val > 0.0f) return 1.0f;
    if (val < 0.0f) return -1.0f;
    return 0.0f;
}

#endif // __MATHUTIL_H__

