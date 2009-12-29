/***********************************************************************************/
//  File:   JPivotEditor.h
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPIVOTEDITOR_H__
#define __JPIVOTEDITOR_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JPivotEditor
//  Desc:   
/***********************************************************************************/
class JPivotEditor : public JWidget
{
    Vec3                m_Pivot;

public:

                        JPivotEditor();
    virtual void        Render      ();
    virtual void        OnDrag      ( JDragEvent& e );

    expose(JPivotEditor)
    {
        parent( JWidget );
        field( "Value", m_Pivot );
    }
}; // class JPivotEditor

#endif //__JPIVOTEDITOR_H__