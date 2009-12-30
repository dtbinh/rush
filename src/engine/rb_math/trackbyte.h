/***********************************************************************************/
//  File:   Trackuint8_t.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TRACKuint8_t_H__
#define __TRACKuint8_t_H__

#include "Track.h"

/***********************************************************************************/
//  Class:  Trackuint8_tTraits
//  Desc:   8-bit unsigned track traits
/***********************************************************************************/
struct Trackuint8_tTraits
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
}; // struct Trackuint8_tTraits

typedef Track<uint8_t, Trackuint8_tTraits> Trackuint8_t;

#endif // __TRACKuint8_t_H__