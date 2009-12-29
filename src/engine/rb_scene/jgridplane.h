/***********************************************************************************/
//  File:   JGridPlane.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JGridPlane_H__
#define __JGridPlane_H__

/***********************************************************************************/
//  Class:  JGridPlane
//  Desc:   
/***********************************************************************************/
class JGridPlane : public JWidget
{
    DWORD               m_LinesColor;
    DWORD               m_RulersColor;
    DWORD               m_XColor;
    DWORD               m_YColor;
    int                 m_AxisLineWidth;
    float               m_CellSide;
    int                 m_CellsW;
    int                 m_CellsH;
    Vec3                m_Center;
    Vec3                m_Normal;

public:
                        JGridPlane   ();
    virtual void        Render  ();

    expose(JGridPlane)
    {
        parent(JWidget);
        field( "LinesColor",      m_LinesColor      );
        field( "XColor",          m_XColor          );
        field( "YColor",          m_YColor          );
        field( "RulersColor",     m_RulersColor     );
        field( "AxisLineWidth",   m_AxisLineWidth   );
        field( "CellSide",        m_CellSide        );
        field( "CellsW",          m_CellsW          );
        field( "CellsH",          m_CellsH          );
        field( "Center",          m_Center          );
        field( "Normal",          m_Normal          );
    }
}; // class JGridPlane

#endif //__JGridPlane_H__