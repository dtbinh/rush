/***********************************************************************************/
//  File:   JChart.h
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCHART_H__
#define __JCHART_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JChart
//  Desc:   Widget used to draw y = f(x) graphs
/***********************************************************************************/
class JChart : public JWidget
{
    float               m_MinVal;
    float               m_MaxVal;
    bool                m_bAdaptive;
    bool                m_bDrawAxis;
    DWORD               m_AxisColor;
    bool                m_bDrawXLabels;
    bool                m_bDrawYLabels;
    DWORD               m_bLabelsColor;
    int                 m_LabelTextHeigh;
    bool                m_bShowGrid;
    DWORD               m_GridColor;
    float               m_GridXStep;
    float               m_GridYStep;

public:
                        JChart();
    virtual void        Render();

    expose(JChart)
    {
        parent( JWidget );
        field( "MinVal",         m_MinVal           );
        field( "MaxVal",         m_MaxVal           );
        field( "Adaptive",       m_bAdaptive        );
        field( "DrawAxis",       m_bDrawAxis        );
        field( "AxisColor",      m_AxisColor        );
        field( "DrawXLabels",    m_bDrawXLabels     );
        field( "DrawYLabels",    m_bDrawYLabels     );
        field( "LabelsColor",    m_bLabelsColor     );
        field( "LabelTextHeigh", m_LabelTextHeigh   );
        field( "bShowGrid",      m_bShowGrid        );
        field( "GridColor",      m_GridColor        );
        field( "GridXStep",      m_GridXStep        );
        field( "GridYStep",      m_GridYStep        );
    }

}; // class JChart

#endif //__JCHART_H__