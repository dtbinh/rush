/***********************************************************************************/
//  File:   TrackPRS.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TRACKPRS_H__
#define __TRACKPRS_H__

#include "trackquat.h"
#include "trackvec3.h"
#include "trackbyte.h"

enum PRSFlags
{   
    PRSFlags_Constant = 0x01,    
}; // enum PRSFlags

/***********************************************************************************/
//  Class:  TrackPRS 
//  Desc:   Position-Rotation-Scale animation track
/*          Also includes visibility animation
/***********************************************************************************/
struct TrackPRS 
{
    std::string     m_Name;         //  track name        

    TrackQuat       m_Rot;          //  rotation track
    TrackVec3       m_Pos;          //  position track
    TrackVec3       m_Scale;        //  scale track
    TrackByte       m_Visibility;   //  visibility track
    uint16_t            m_Flags;        //  flag set
    Mat4            m_TM;           //  pose transform, if constant
    int             m_Tag;          //  some tag value, usually bone index

    TrackPRS() : m_Tag( -1 ), m_Flags( 0 ) {}
}; // struct TrackPRS 

#endif // __TRACKPRS_H__