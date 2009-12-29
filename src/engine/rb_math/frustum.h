//****************************************************************************/
//  File:  Frustum.h
//  Desc:  
//****************************************************************************/
#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__
#pragma once

#include "Vec3.h"
#include "Plane.h"

class Plane;
class Mat4;
//****************************************************************************/
//  Class:  Frustum                                              
//  Desc:                          
//****************************************************************************/
class Frustum
{
public:
	Frustum() {}
    Frustum( const Mat4& cm ) { FromCameraMatrix( cm ); }

    const Vec3&     ltn() const { return vLTN; }
    const Vec3&     rtn() const { return vRTN; }
    const Vec3&     lbn() const { return vLBN; }
    const Vec3&     rbn() const { return vRBN; }

    const Vec3&     ltf() const { return vLTF; }
    const Vec3&     rtf() const { return vRTF; }
    const Vec3&     lbf() const { return vLBF; }
    const Vec3&     rbf() const { return vRBF; }

    void            FromCameraMatrix( const Mat4& cm ); 
    bool            PtIn            ( const Vec3& pt ) const;
	const Plane&    GetPlane        ( int idx ) const { return (&plNear)[idx]; }
    bool            GetTop          ( Vec3& top ) const;

public:
    Plane           plNear;
    Plane           plFar;
    Plane           plLeft;
    Plane           plRight;
    Plane           plTop;
    Plane           plBottom;

private:
    void        UpdateVertices();

    //  cached frustum vertices
    Vec3        vLTN, vRTN, vLBN, vRBN, vLTF, vRTF, vLBF, vRBF;

};  // class Frustum
#endif // __FRUSTUM_H__

