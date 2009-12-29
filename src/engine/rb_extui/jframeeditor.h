/***********************************************************************************/
//  File:   JFrameEditor.h
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JFRAMEEDITOR_H__
#define __JFRAMEEDITOR_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JFrameEditor
//  Desc:   
/***********************************************************************************/
class JFrameEditor : public JWidget
{
    Frame               m_Frame;

public:

                        JFrameEditor();
    virtual void        Render      ();
    virtual void        OnDrag      ( JDragEvent& e );

    expose(JFrameEditor)
    {
        parent( JWidget );
        field( "Value", m_Frame );
    }
}; // class JFrameEditor

#endif //__JFRAMEEDITOR_H__