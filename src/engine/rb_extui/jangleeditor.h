/***********************************************************************************/
//  File:   JAngleEditor.h
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANGLEEDITOR_H__
#define __JANGLEEDITOR_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JAngleEditor
//  Desc:   
/***********************************************************************************/
class JAngleEditor : public JWidget
{
    float               m_Angle;
    Vec2                m_Pivot;

    float               m_InnerRadius;
    float               m_OuterRadius;
    float               m_DragSector;

public:

                        JAngleEditor();
    virtual void        Render      ();
    virtual void        OnDrag      ( JDragEvent& e );

    expose(JAngleEditor)
    {
        parent( JWidget );
        field( "Angle", m_Angle );
        field( "Pivot", m_Pivot );
        field( "InnerRadius", m_InnerRadius );
        field( "OuterRadius", m_OuterRadius );
        field( "DragSector", m_DragSector );
    }
}; // class JAngleEditor

#endif //__JANGLEEDITOR_H__