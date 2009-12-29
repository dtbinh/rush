/***********************************************************************************/
//  File:   TrackVec3.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TRACKVEC3_H__
#define __TRACKVEC3_H__

#include "Track.h"
#include "Vec3.h"

/***********************************************************************************/
//  Class:  TrackVec3Traits
//  Desc:   Quaternion track traits
/***********************************************************************************/
struct TrackVec3Traits
{
    static inline Vec3 defaultVal() 
    {
        return Vec3::null;
    }

    static inline float norm( const Vec3& val ) 
    {
        return val.norm();
    }

    static inline Vec3 interpolate( const Vec3& a, const Vec3& b, float t )
    {
        return a + (b - a)*t;
    }
}; // struct TrackVec3Traits

typedef Track<Vec3, TrackVec3Traits> TrackVec3;

#endif //__TRACKVEC3_H__