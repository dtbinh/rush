/***********************************************************************************/
/*  File:   JPaintArea.cpp
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JButton.h"
#include "JPaintArea.h"

/***********************************************************************************/
/*  JPaintArea implementation
/***********************************************************************************/
decl_class(JPaintArea);
JPaintArea::JPaintArea()
{
    m_PressDX       = 0;
    m_PressDY       = 0;
    m_bPixelPrecise = true;
    SetBgColor( 0 );
} // JPaintArea::JPaintArea

void JPaintArea::Init()
{
} // JPaintArea:Init

void JPaintArea::Render()
{
    JButton::Render();
} // JPaintArea::Render

bool JPaintArea::PtInArea( int px, int py ) const
{
    int spID = GetSkinPackID();
    if (spID == -1 || m_NormalFrame == -1 || !m_bPixelPrecise)
    {
        return JWidget::PtIn( px, py );
    }
    Frame ext = GetExt();
    Vec2 pixPos( float( px ) - ext.x, float( py ) - ext.y );
    DWORD clr = g_pDrawServer->GetPixel( spID, m_NormalFrame, pixPos );
    DWORD alpha = (clr&0xFF000000)>>24;
    return (alpha > 0x50);
} // JPaintArea::PtInArea

bool JPaintArea::PtIn( int px, int py ) const
{
    int c_SqSize = 1;
    int c_SqStep = 2;
    int nSamples = (2*c_SqSize + 1)*(2*c_SqSize + 1);
    int nIn = 0;
    int nTested = 0;
    for (int j = -c_SqSize; j <= c_SqSize; j++)
    {
        for (int i = -c_SqSize; i <= c_SqSize; i++)
        {
            if (PtInArea( px + j*c_SqStep, py + i*c_SqStep ))
            {
                nIn++;
            }
            nTested++;
            if (nTested > nSamples/2 && nIn == 0) return false;
            if (nIn > nSamples/2) return true;
        }
    }
    return (nIn > nSamples/2);
} // JPaintArea::PtIn