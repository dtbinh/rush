/***********************************************************************************/
//  File:   JChart.cpp
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JChart.h"

/***********************************************************************************/
/*  JChart implementation
/***********************************************************************************/
decl_class( JChart );

JChart::JChart()
{
    m_MinVal            = 0;
    m_MaxVal            = 100;
    m_bAdaptive         = true;
    m_bDrawAxis         = true;
    m_AxisColor         = 0xFF333344;
    m_bDrawXLabels      = true;
    m_bDrawYLabels      = true;
    m_bLabelsColor      = 0xFF333344;
    m_LabelTextHeigh    = 10;
    m_bShowGrid         = true;
    m_GridColor         = 0x55333333;
    m_GridXStep         = 10;
    m_GridYStep         = 0.1f;
} // JChart::JChart

void JChart::Render()
{
    // TODO
} // JChart::Render