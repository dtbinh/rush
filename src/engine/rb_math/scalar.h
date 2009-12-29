//****************************************************************************/
//  File:  Scalar.h
//  Desc:  Common scalar operations
//****************************************************************************/
#ifndef __SCALAR_H__
#define __SCALAR_H__
#pragma once

#include "MathConst.h"

template <class T> T tmin( const T& a, const T& b ) { return (a < b) ? a : b; }
template <class T> T tmax( const T& a, const T& b ) { return (a > b) ? a : b; }
template <class T> T tmin( const T& a, const T& b, const T& c ) { return tmin( tmin( a, b ), c ); }
template <class T> T tmax( const T& a, const T& b, const T& c ) { return tmax( tmax( a, b ), c ); }
template <class T> T tmin( const T& a, const T& b, const T& c, const T& d ) { return tmin( tmin( a, b ), tmin( c, d ) ); }
template <class T> T tmax( const T& a, const T& b, const T& c, const T& d ) { return tmax( tmax( a, b ), tmax( c, d ) ); }
template <class T> T tmin( const T& a, const T& b, const T& c, const T& d, const T& e ) { return tmin( tmin( a, b ), tmin( c, d, e ) ); }
template <class T> T tmax( const T& a, const T& b, const T& c, const T& d, const T& e ) { return tmax( tmax( a, b ), tmax( c, d, e ) ); }
template <class T> T tmin( const T& a, const T& b, const T& c, 
                          const T& d, const T& e, const T& f ) { return tmin( tmin( a, b, c ), tmin( d, e, f ) ); }
template <class T> T tmax( const T& a, const T& b, const T& c, 
                          const T& d, const T& e, const T& f ) { return tmax( tmax( a, b, c ), tmax( d, e, f ) ); }

inline float sign( float val )
{
    if (val < 0.0f) return -1.0f;
    if (val > 0.0f) return 1.0f;
    return 0.0f;
}

template <typename T>
inline T clamp( const T& val, const T& minv, const T& maxv )
{
    if (val < minv) return minv;
    if (val > maxv) return maxv;
    return val;
}

template <typename T>
inline T wrap( const T& val, const T& minv, const T& maxv )
{
    T delta = maxv - minv;
    T cval = val;
    while (cval < minv) 
    {
        cval += delta;
    }
    while (cval > maxv) 
    {
        cval -= delta;
    }
    return cval;
}

inline float FastInvSqrt( float f )
{
    float h = f * 0.5f;
    unsigned int y = 0x5F3759DF - ((unsigned int&)( f ) >> 1);
    (float&)y  = (float&)y*(1.5f - (h*(float&)y*(float&)y));
    return (float&)y;
} // FastInvSqrt

inline float RadToDeg( float rad ) 
{ 
    return 180.0f * rad / c_PI; 
}

inline float DegToRad( float deg ) 
{ 
    return c_PI * deg / 180.0f; 
}

inline float Exposure( float v, float sharpness )
{
    return 1.0f - exp( sharpness*v );
} // Exposure

inline float _fabs( float val )
{
    return (val >= 0.0f) ? val : -val;
}

inline bool equalf( float a, float b, float epsilon = c_FltEpsilon )
{
    return (_fabs( a - b ) < epsilon);
}

inline bool IsZero( float a, float epsilon = c_FltEpsilon )
{
    return (_fabs( a ) < epsilon);
}

#endif // __SCALAR_H__

