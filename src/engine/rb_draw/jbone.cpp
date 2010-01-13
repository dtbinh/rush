/***********************************************************************************/
//  File:   JBone.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JBone.h"

/***********************************************************************************/
/*  JBone implementation
/***********************************************************************************/
decl_class(JBone);

Vec3 JBone::GetPosition() const
{ 
    Vec3 s, t;
    Mat3 r;
    m_TM.factorize( s, r, t );
    return t;
} // JBone::GetPosition

void JBone::SetPosition( Vec3 pos )
{ 
    Vec3 s, t;
    Mat3 r;
    m_TM.factorize( s, r, t );
    m_TM.srt( s, r, pos );
} // JBone::SetPosition

Mat3 JBone::GetRotation() const
{ 
    Vec3 s, t;
    Mat3 r;
    m_TM.factorize( s, r, t );
    return r;
} // JBone::GetRotation

void JBone::SetRotation( Mat3 tm )
{ 
    Vec3 s, t;
    Mat3 r;
    m_TM.factorize( s, r, t );
    m_TM.srt( s, tm, t );
} // JBone::SetRotation

Vec3 JBone::GetScale() const
{ 
    Vec3 s, t;
    Mat3 r;
    m_TM.factorize( s, r, t );
    return s; 
} // JBone::GetScale

void JBone::SetScale( Vec3 scale )
{  
    Vec3 s, t;
    Mat3 r;
    m_TM.factorize( s, r, t );
    m_TM.srt( scale, r, t );
} // JBone::SetScale