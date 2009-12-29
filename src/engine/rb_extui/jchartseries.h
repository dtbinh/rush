/***********************************************************************************/
//  File:   JChartSeries.h
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCHARTSERIES_H__
#define __JCHARTSERIES_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JChartSeries
//  Desc:   Sigle series in y = f(x) graph 
/***********************************************************************************/
class JChartSeries : public JWidget
{
    DWORD               m_Color;
    float               m_StepWidth;
    float               m_LastValue;

    std::vector<float>  m_Values;

public:
                        JChartSeries();
    virtual void        Render();

    expose(JChartSeries)
    {
        parent(JWidget);
        field( "Color",     m_Color       );
        field( "StepWidth", m_StepWidth   );
        field( "Value",     m_LastValue   );
    }
}; // class JChartSeries

#endif //__JCHARTSERIES_H__