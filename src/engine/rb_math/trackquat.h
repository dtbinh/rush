/***********************************************************************************/
//  File:   TrackQuat.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TRACKQUAT_H__
#define __TRACKQUAT_H__

#include "track.h"
#include "quaternion.h"

/***********************************************************************************/
//  Class:  TrackQuatTraits
//  Desc:   Quaternion track traits
/***********************************************************************************/
struct TrackQuatTraits
{
    static inline Quaternion defaultVal() 
    {
        return Quaternion::null;
    }
    static inline float norm( const Quaternion& val ) 
    {
        return val.norm();
    }

    static inline Quaternion interpolate( const Quaternion& a, const Quaternion& b, float t )
    {
        Quaternion res;
        res.slerp( a, b, t );
        return res;
    }

}; // struct TrackQuatTraits


class TrackQuat : public Track<Quaternion, TrackQuatTraits> 
{
public:
    //  correct adjacent keys to go through shortest arc
    void FixupAdjacentKeys()
    {
	    int nKeys = GetNKeys();
	    for (int i = 1; i < nKeys; i++)
	    {
		    float cosTheta = m_KeyVal[i - 1].dot( m_KeyVal[i] );
		    if (cosTheta < 0.0f)
		    {
			    m_KeyVal[i].reverse();
		    }
        }
	}
}; //class TrackQuat

#endif //__TRACKQUAT_H__