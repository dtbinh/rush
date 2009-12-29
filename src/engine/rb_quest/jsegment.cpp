/***********************************************************************************/
//  File:   JSegment.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JSegment.h"
#include "JAnimation.h"
#include "JQuestGame.h"
#include "Mat3.h"
#include "Vec2.h"

/***********************************************************************************/
/*  JSegment implementation
/***********************************************************************************/
decl_class(JSegment);

JSegment::JSegment()
{
    m_Beg               = Vec3::null;
    m_End               = Vec3::null;
    m_Width             = 1.0f;
    m_ClickWidth        = 2.0f;
    m_Color             = 0xFFFFFFFF;
    const float c_BigNum = 10000.0f;
    SetLocalExt( Frame( -c_BigNum, -c_BigNum, c_BigNum*2.0f, c_BigNum*2.0f ) );
} // JSegment::JSegment

void JSegment::Render()
{
    Vec2 beg( m_Beg.x, m_Beg.y );
    Vec2 end( m_End.x, m_End.y );

    Vec2 dir = end - beg;
    dir.normalize();

    Vec2 norm( -dir.y, dir.x );

    float hw = m_Width*0.5f;
    Vec2 ba = beg + norm*hw;
    Vec2 bb = beg - norm*hw;
    Vec2 ea = end + norm*hw;
    Vec2 eb = end - norm*hw;

    g_pDrawServer->DrawPoly( ba, ea, bb, m_Color, m_Color, m_Color );
    g_pDrawServer->DrawPoly( bb, ea, eb, m_Color, m_Color, m_Color );
} // JSegment::Render

bool JSegment::PtIn( int px, int py ) const
{
    Vec2 mpt;
    mpt.x = float( px );
    mpt.y = float( py );
    Vec2 beg( m_Beg.x, m_Beg.y );
    Vec2 end( m_End.x, m_End.y );
    float d = mpt.dist_seg( beg, end );
    return (d*2.0f < m_ClickWidth);
} // JSegment::PtIn
