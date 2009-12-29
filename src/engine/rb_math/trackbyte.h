/***********************************************************************************/
//  File:   TrackByte.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TRACKBYTE_H__
#define __TRACKBYTE_H__

#include "Track.h"

/***********************************************************************************/
//  Class:  TrackByteTraits
//  Desc:   8-bit unsigned track traits
/***********************************************************************************/
struct TrackByteTraits
{
    static inline float defaultVal() 
    {
        return 0;
    }

    static inline float norm( const BYTE& val ) 
    {
        return (val == 0) ? 0.0f : 1.0f;
    }

    static inline BYTE interpolate( const BYTE& a, const BYTE& b, float t )
    {
        return BYTE( float( a ) + float( b - a )*t );
    }
}; // struct TrackByteTraits

typedef Track<BYTE, TrackByteTraits> TrackByte;

#endif // __TRACKBYTE_H__