/***********************************************************************************/
//  File:   PCurve2.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __PCURVE2_H__
#define __PCURVE2_H__

#include "vec2.h"
#include <vector>

/***********************************************************************************/
//  Class:  PCurve2
//  Desc:
/***********************************************************************************/
class PCurve2
{
    std::vector<float>      m_Times;
    std::vector<Vec2>       m_Points;

public:
    void        Clear       () { m_Points.clear(); }
    int         GetNPoints  () const { return static_cast<int>( m_Points.size() ); }
    const Vec2& GetPoint    ( int idx ) const { return m_Points[idx%m_Points.size()]; }
    float       GetTime     ( int idx ) const { return m_Times[idx%m_Points.size()]; }

    void        AddPoint    ( float t, const Vec2& pt );
    void        InsertPoint ( int idx, float t, const Vec2& pt );
    void        DeletePoint ( int idx );

}; // class PCurve2

#endif //__PCURVE2_H__
