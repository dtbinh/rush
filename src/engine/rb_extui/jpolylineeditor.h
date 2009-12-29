/***********************************************************************************/
//  File:   JPolyLineEditor.h
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPOLYLINEEDITOR_H__
#define __JPOLYLINEEDITOR_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JPolyLineEditor
//  Desc:   
/***********************************************************************************/
class JPolyLineEditor : public JWidget
{
    PolyLine2           m_PolyLine;
    int                 m_SelPt;
    bool                m_bSmooth;
    bool                m_bClosed;

public:
                        JPolyLineEditor ();

    virtual void        Render          ();
    virtual void        OnKey           ( JKeyEvent& e );
    virtual void        OnDrag          ( JDragEvent& e );
    virtual void        OnMouse         ( JMouseEvent& e );

    expose(JPolyLineEditor)
    {
        parent(JWidget);
        field( "Value",     m_PolyLine );
        field( "Smooth",    m_bSmooth );
        field( "Closed",    m_bClosed );
    }
}; // class JPolyLineEditor

#endif //__JPOLYLINEEDITOR_H__