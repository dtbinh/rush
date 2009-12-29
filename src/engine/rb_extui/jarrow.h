/***********************************************************************************/
//  File:   JArrow.h
//  Date:   21.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JARROW_H__
#define __JARROW_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JArrow
//  Desc:   Draws an fat wargame-like arrow in screen space
/***********************************************************************************/
class JArrow : public JWidget
{
    Vec2            m_Start;        //  arrow starting position
    float           m_StartWidth;    
    DWORD           m_StartColor;

    Vec2            m_End;          //  arrow end position
    float           m_EndWidth;    
    DWORD           m_EndColor;
    
    float           m_HeadWidth;    //  head width, part of end arrow width
    float           m_HeadLength;   //  head length, part of overall arrow length
    float           m_StartPitch;   //  starting pitch-from-direction-to-end angle
    float           m_EndPitch;     //  endinging pitch-from-direction-to-start angle
    int             m_NSteps;       //  number of arrow division steps

public:
                    JArrow();
    virtual void    Render();

    expose(JArrow)
    {
        parent(JWidget);
        field( "Start",         m_Start         );
        field( "StartWidth",    m_StartWidth    );
        field( "StartColor",    m_StartColor    );
        field( "End",           m_End           );
        field( "EndWidth",      m_EndWidth      );
        field( "EndColor",      m_EndColor      );
        field( "HeadWidth",     m_HeadWidth     );
        field( "HeadLength",    m_HeadLength    );
        field( "NumSteps",      m_NSteps        );
        field( "StartPitch",    m_StartPitch    );
        field( "EndPitch",      m_EndPitch      );
    }
}; // class JArrow

#endif //__JARROW_H__