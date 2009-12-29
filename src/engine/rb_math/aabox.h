//****************************************************************************/
//  File:  AABox.h
//****************************************************************************/
#ifndef __AABOX_H__
#define __AABOX_H__
#pragma once

#include "Vec3.h"
class Frame;
class Mat4;
//****************************************************************************/
//  Class:  AABox
//  Desc:  Axis-aligned bounding box
//****************************************************************************/
class AABox
{
public:
    Vec3    minv;
    Vec3    maxv;
    
    
            AABox(){}
            AABox( const Vec3& pmin, const Vec3& pmax ) : minv(pmin), maxv(pmax) {}
            AABox( const Vec3& c, float dx, float dy, float dz );
            AABox( const Vec3& c, float side );
            AABox( const Frame& base, float minz, float maxz );

    inline void AddPoint( const Vec3& pt )
    {
        if (minv.x > pt.x) minv.x = pt.x;
        if (minv.y > pt.y) minv.y = pt.y;
        if (minv.z > pt.z) minv.z = pt.z;
        if (maxv.x < pt.x) maxv.x = pt.x;
        if (maxv.y < pt.y) maxv.y = pt.y;
        if (maxv.z < pt.z) maxv.z = pt.z;
    }

    void Invalidate();
    void Transform( const Mat4& tm );

    Vec3 GetCenter() const { return 0.5f*(minv + maxv); }
    Vec3 GetExt() const { return maxv - minv; }

    Vec3 xyz() const { return minv; }
    Vec3 xyZ() const { return Vec3( minv.x, minv.y, maxv.z ); }
    Vec3 xYz() const { return Vec3( minv.x, maxv.y, minv.z ); }
    Vec3 xYZ() const { return Vec3( minv.x, maxv.y, maxv.z ); }

    Vec3 Xyz() const { return Vec3( maxv.x, minv.y, minv.z ); }
    Vec3 XyZ() const { return Vec3( maxv.x, minv.y, maxv.z ); }
    Vec3 XYz() const { return Vec3( maxv.x, maxv.y, minv.z ); }
    Vec3 XYZ() const { return maxv; }
            
}; // class AABox

#endif // __AABox_H__

