/***********************************************************************************/
//  File:   OdeUtil.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __ODEUTIL_H__
#define __ODEUTIL_H__
#include "ode/ode.h"

#include "PhysicsServer.h"
#include "Quaternion.h"
    
inline void Convert( dMatrix3& dm, const Mat4& m )
{
    dm[0] = m.e00; dm[4] = m.e01; dm[8]  = m.e02;
    dm[1] = m.e10; dm[5] = m.e11; dm[9]  = m.e12;
    dm[2] = m.e20; dm[6] = m.e21; dm[10] = m.e22;
    dm[3] = m.e30; dm[7] = m.e31; dm[11] = m.e32;
}

inline void Convert( dMatrix3& dm, const Mat3& m )
{
    dm[0] = m.e00; dm[4] = m.e01; dm[8]  = m.e02;
    dm[1] = m.e10; dm[5] = m.e11; dm[9]  = m.e12;
    dm[2] = m.e20; dm[6] = m.e21; dm[10] = m.e22;
    dm[3] = 0;	   dm[7] = 0;	  dm[11] = 0;
}

inline void Convert( dVector4& dv, const Vec4& v )
{
    dv[0] = v.x; dv[1] = v.y; dv[2] = v.z; dv[3] = v.w;
}

inline void Convert( dVector3& dv, const Vec3& v )
{
    dv[0] = v.x; dv[1] = v.y; dv[2] = v.z; 
}

inline void Convert( dQuaternion& dq, const Quaternion& q )
{
    dq[0] = q.s; 
    dq[1] = q.v.x;
    dq[2] = q.v.y;
    dq[3] = q.v.z;
}

//  from ODE
inline void Convert( Mat4& m, const dMatrix3& dm )
{
    m.e00 = dm[0]; m.e01 = dm[4]; m.e02 = dm[8] ; m.e03 = 0.0f;
    m.e10 = dm[1]; m.e11 = dm[5]; m.e12 = dm[9] ; m.e13 = 0.0f;
    m.e20 = dm[2]; m.e21 = dm[6]; m.e22 = dm[10]; m.e23 = 0.0f;
    m.e30 = dm[3]; m.e31 = dm[7]; m.e32 = dm[11]; m.e33 = 1.0f;
}

inline void Convert( Mat3& m, const dMatrix3& dm )
{
    m.e00 = dm[0]; m.e01 = dm[4]; m.e02 = dm[8] ;
    m.e10 = dm[1]; m.e11 = dm[5]; m.e12 = dm[9] ;
    m.e20 = dm[2]; m.e21 = dm[6]; m.e22 = dm[10];
}

inline void Convert( Vec4& v, const dVector4& dv )
{
    v.x = dv[0]; v.y = dv[1]; v.z = dv[2]; v.w = dv[3];
}

inline void Convert( Vec3& v, const dVector3& dv )
{
    v.x = dv[0]; v.y = dv[1]; v.z = dv[2]; 
}

inline void Convert( Quaternion& q, const dQuaternion& dq )
{
    q.s		= dq[0]; 
    q.v.x	= dq[1];
    q.v.y	= dq[2];
    q.v.z	= dq[3];
}


#endif //__ODEUTIL_H__