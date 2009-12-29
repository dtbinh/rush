/***********************************************************************************/
//  File:   JHeartBit.cpp
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JHeartBit.h"

/***********************************************************************************/
/*  JHeartBit implementation
/***********************************************************************************/
decl_class( JHeartBit );
JHeartBit::JHeartBit()
{
    m_bAutoScale    = true;
    m_MinVal        = 0.0f;
    m_MaxVal        = 1.0f;
} // JHeartBit::JHeartBit

void JHeartBit::Render()
{
    Frame ext = GetExt();
    
    if ((GetBgColor()&0xFF000000) != 0)
    {
        g_pDrawServer->DrawRect( ext, GetBgColor() ); 
    }

    if (GetText()[0] != 0)
    {
        g_pDrawServer->DrawString( ext.x + 2, ext.y + 2, GetFontID(), GetText(), GetFgColor(), GetFontHeight() );
    }

    int nVal = m_Values.size();
    if (nVal == 0) 
    {
        return;
    }

    if (m_bAutoScale)
    {
        m_MaxVal = m_Values[0];
        m_MinVal = 0.0f;
        for (int i = 0; i < nVal; i++)
        {
            if (m_Values[i] > m_MaxVal)
            {
                m_MaxVal = m_Values[i];
            }
            if (m_Values[i] < m_MinVal)
            {
                m_MinVal = m_Values[i];
            }
        }
        if (m_MaxVal == m_MinVal)
        {
            m_MaxVal += 1.0f;
            m_MinVal -= 1.0f;
        }
    }

    Vec2 nPos, cPos;
    for (int i = 0; i < nVal; i++)
    {
        float val = m_Values[i];
        nPos.x = ext.x + float( i );
        nPos.y = ext.b() - ext.h*(val - m_MinVal)/(m_MaxVal - m_MinVal);
        if (i > 0)
        {
            g_pDrawServer->DrawLine( cPos.x, cPos.y, nPos.x, nPos.y, GetFgColor(), GetFgColor() );
        }
        cPos = nPos; 
    }
    g_pDrawServer->Flush();

} // JHeartBit::Render

float JHeartBit::GetLastValue() const
{
    if (m_Values.size() == 0) 
    {
        return 0.0f;
    }
    return m_Values.back();
} // JHeartBit::GetLastValue

int JHeartBit::GetMaxValuesNum() const
{
    Frame ext = GetExt();
    return (int)ext.w;
}

void JHeartBit::SetLastValue( float val )
{
    int nValMax = GetMaxValuesNum();
    if (m_Values.size() == nValMax)
    {
        m_Values.erase( m_Values.begin() );
    }
    m_Values.push_back( val );
} // JHeartBit::SetLastValue
