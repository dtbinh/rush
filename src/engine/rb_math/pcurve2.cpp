/***********************************************************************************/
//  File:   PCurve2.cpp
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "PCurve2.h"

/***********************************************************************************/
/*  PCurve2 implementation
/***********************************************************************************/
void PCurve2::AddPoint( float t, const Vec2& pt )
{
    m_Times.push_back( t );
    m_Points.push_back( pt );
    assert( m_Times.size() == m_Points.size() );
} // PCurve2::AddPoint

void PCurve2::DeletePoint( int idx )
{
    m_Times.erase( m_Times.begin() + idx );
    m_Points.erase( m_Points.begin() + idx );
    assert( m_Times.size() == m_Points.size() );
} // PCurve2::DeletePoint

void PCurve2::InsertPoint( int idx, float t, const Vec2& pt )
{
    idx = clamp( idx, 0, static_cast<int>( m_Points.size() ) );
    m_Times.insert( m_Times.begin() + idx, t );
    m_Points.insert( m_Points.begin() + idx, pt );
    assert( m_Times.size() == m_Points.size() );
} // PCurve2::DeletePoint



