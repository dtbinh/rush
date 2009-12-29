/***********************************************************************************/
//  File:   JArrow.cpp
//  Date:   21.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JArrow.h"
#include "Spline2.h"
#include "Color.h"

/***********************************************************************************/
/*  JArrow implementation
/***********************************************************************************/
decl_class(JArrow);
JArrow::JArrow()
{
    m_Start         = Vec2( 100, 100 );
    m_StartWidth    = 100.0f;
    m_StartColor    = 0xFF0000FF;
    m_StartPitch    = 0.0f;

    m_End           = Vec2( 400, 400 );
    m_EndWidth      = 30.0f;  
    m_EndColor      = 0xFFFF0000;
    m_EndPitch      = 0.0f;

    m_HeadWidth     = 1.1f;
    m_HeadLength    = 0.1f;
    m_NSteps        = 64;
} // JArrow::JArrow

void JArrow::Render()
{
    Vec2  cpos = m_Start;
    DWORD ccol = m_StartColor;
    float dt = (1.0f - m_HeadLength)/float( m_NSteps );
    float s = dt;

    ColorF clrBeg( m_StartColor );
    ColorF clrEnd( m_EndColor );

    Vec2 a, b, c, d, r;

    Vec2 t0 = m_End - m_Start;
    t0.rotate( m_StartPitch );
    Vec2 t1 = m_Start - m_End;
    t1.rotate( m_EndPitch );

    DWORD clr = 0;
    for (int i = 1; i < m_NSteps; i++)
    {
        //  find next position on the arrow
        Vec2 pos = Hermite( m_Start, t0, m_End, t1, s );
        clr = lerp( clrBeg, clrEnd, s );
        r = pos - cpos;
        r = r.normal(); 
        r.normalize();

        float w = m_StartWidth + (m_EndWidth - m_StartWidth)*s;
        w *= 0.5f;
        
        if (i == 1)
        {
            a = cpos + r*w;
            b = cpos - r*w;
        }

        c = pos + r*w;
        d = pos - r*w;
        
        //  draw arrow section
        g_pDrawServer->DrawPoly( a, b, c, ccol, ccol, clr );
        g_pDrawServer->DrawPoly( b, c, d, ccol, clr, clr );
        
        a = c;
        b = d;

        cpos = pos;
        ccol = clr;
        s += dt;
    }

    //  draw arrow head
    r = m_End - cpos;
    r = r.normal(); 
    r.normalize();
    float hw = m_EndWidth*m_HeadWidth;
    c = cpos + r*hw;
    d = cpos - r*hw;
    
    g_pDrawServer->DrawPoly( a, c, m_End, ccol, ccol, m_EndColor );
    g_pDrawServer->DrawPoly( b, a, m_End, ccol, ccol, m_EndColor );
    g_pDrawServer->DrawPoly( d, b, m_End, ccol, ccol, m_EndColor );
    
    g_pDrawServer->Flush();
} // JArrow::Render
