//****************************************************************************/
//  File:   JBevel.h
//  Date:   24.10.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JBEVEL_H__
#define __JBEVEL_H__

#include "JWidget.h"

//****************************************************************************/
//  Class:  JBevel
//  Desc:   Gizmo used for manual value tweaking by dragging mouse
//****************************************************************************/
class JBevel : public JWidget
{
public:
                    JBevel            ();
    virtual void    Render            ();
    virtual void    Init              ();

    expose( JBevel )
    {
        parent( JWidget );
        field( "LeftTopFrame",     m_LeftTop      );
        field( "RightTopFrame",    m_RightTop     );
        field( "LeftBottomFrame",  m_LeftBottom   );
        field( "RightBottomFrame", m_RightBottom  );
        field( "LeftFrame",        m_Left         );
        field( "TopFrame",         m_Top          );
        field( "RightFrame",       m_Right        );
        field( "BottomFrame",      m_Bottom       );
        field( "CenterFrame",      m_Center       );
    }

private:
    int             m_LeftTop;
    int             m_RightTop;
    int             m_LeftBottom;
    int             m_RightBottom;

    int             m_Left;
    int             m_Top;
    int             m_Right;
    int             m_Bottom;

    int             m_Center;

}; // class JBevel

#endif // __JBEVEL_H__


