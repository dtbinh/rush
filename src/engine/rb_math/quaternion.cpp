//****************************************************************************/
//  File:  Quaternion.cpp
//  Desc:  Quaternion class
//****************************************************************************/
#include "precompile.h"
#include "quaternion.h"

//****************************************************************************/
/*  Quaternion implementation
//****************************************************************************/
Quaternion Quaternion::null( 0.0f, 0.0f, 0.0f, 0.0f );

Quaternion::Quaternion( const Vec3& axis, float angle )
{
    float h = angle*0.5f;
    s = cosf( h );
    v = axis;
    v.normalize();
    v *= sinf( h );
} // Quaternion::Quaternion

Quaternion::Quaternion( const Mat3& rotM )
{
    float trace = rotM.e00 + rotM.e11 + rotM.e22;
    float root;
    if (trace > 0.0f)
    {
        root = sqrtf( trace + 1.0f );
        s    = root*0.5f;

        root = 0.5f/root;
        v.x  = (rotM.e12 - rotM.e21)*root;
        v.y  = (rotM.e20 - rotM.e02)*root;
        v.z  = (rotM.e01 - rotM.e10)*root;
    }
    else
    {
        float maxC = tmax( rotM.e00, rotM.e11, rotM.e22 );
        if (maxC == rotM.e00)
        {
            root = sqrtf( 1.0f + rotM.e00 - rotM.e11 - rotM.e22 );
            v.x = 0.5f*root;
            if (root > c_FltEpsilon) root = 0.5f/root;

            v.y = (rotM.e01 + rotM.e10)*root;
            v.z = (rotM.e02 + rotM.e20)*root;
            s   = (rotM.e12 - rotM.e21)*root;
        }
        else if (maxC == rotM.e11)
        {
            root = sqrtf( 1.0f + rotM.e11 - rotM.e00 - rotM.e22 );
            v.y = 0.5f*root;
            if (root > c_FltEpsilon) root = 0.5f/root;

            v.x = (rotM.e01 + rotM.e10)*root;
            v.z = (rotM.e12 + rotM.e21)*root;
            s   = (rotM.e20 - rotM.e02)*root;
        }
        else if (maxC == rotM.e22)
        {
            root = sqrtf( 1.0f + rotM.e22 - rotM.e00 - rotM.e11 );
            v.z = 0.5f*root;
            if (root > c_FltEpsilon) root = 0.5f/root;

            v.x = (rotM.e02 + rotM.e20)*root;
            v.y = (rotM.e12 + rotM.e21)*root;
            s   = (rotM.e01 - rotM.e10)*root;
        }
        else assert( false );
    }
} // Quaternion::Quaternion

void Quaternion::normalize()
{
    float n = norm();
    if (_fabs( n ) < c_FltEpsilon) return;
    v /= n; s /= n;
} // Quaternion::normalize

void Quaternion::slerp( const Quaternion& q1, const Quaternion& q2, float t )
{
    Quaternion tmp( q2 );
    float cosTheta = q1.dot( q2 );

    //  choose the sign of q2, to choose shorter arc beetween q1 and q2
    if (cosTheta < 0.0f)
    {
        cosTheta = -cosTheta;
        tmp.reverse();
    }

    float t1, t2;
    const float c_QuatEpsilon = 0.001f;
    if (1.0f - cosTheta > c_QuatEpsilon)
    {
        float theta = acosf( cosTheta );
        float invSinTheta = 1.0f/sinf( theta );
        t1 = sinf( (1.0f - t)*theta);
        t2 = sinf( t * theta );

        v.x = t1*q1.v.x + t2*tmp.v.x;
        v.y = t1*q1.v.y + t2*tmp.v.y;
        v.z = t1*q1.v.z + t2*tmp.v.z;
        s   = t1*q1.s   + t2*tmp.s;

        v *= invSinTheta;
        s *= invSinTheta;
    }
    else
    {
        // do simple linear interpolation, because there is no reason for slerp
        t1  = 1.0f - t;
        t2  = t;
        v.x = t1*q1.v.x + t2*tmp.v.x;
        v.y = t1*q1.v.y + t2*tmp.v.y;
        v.z = t1*q1.v.z + t2*tmp.v.z;
        s   = t1*q1.s   + t2*tmp.s;
    }
} // slerp
