/***********************************************************************************/
//  File:   TrackByte.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TRACKBYTE_H__
#define __TRACKBYTE_H__

#include "track.h"

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

    static inline float norm( const uint8_t& val ) 
    {
        return (val == 0) ? 0.0f : 1.0f;
    }

    static inline uint8_t interpolate( const uint8_t& a, const uint8_t& b, float t )
    {
        return uint8_t( float( a ) + float( b - a )*t );
    }
}; // struct TrackByteTraits

typedef Track<uint8_t, TrackByteTraits> TrackByte;

#endif // __TRACKBYTE_H__