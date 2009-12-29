/***********************************************************************************/
//  File:   JColorBox.h
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCOLORBOX_H__
#define __JCOLORBOX_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JColorBox
//  Desc:   
/***********************************************************************************/
class JColorBox : public JWidget
{
    DWORD               m_ColorVal;
    bool                m_bHasAlpha;
    JString             m_ColorTextVal;

    Frame               GetColorExt     () const;
    Frame               GetAlphaExt     () const;

public:
                        JColorBox       ();
    virtual void        Render          ();
    virtual void        OnMouse         ( JMouseEvent& e );
    const char*         GetColorTextVal () const { return m_ColorTextVal.c_str(); }
    void                SetColorTextVal ( const char* val );
    void                SetColorVal     ( DWORD val );
    DWORD               GetColorVal     () const { return m_ColorVal; }

    expose(JColorBox)
    {
        parent(JWidget);
        prop ( "ColorVal",     GetColorVal, SetColorVal );
        field( "HasAlpha",     m_bHasAlpha );
        prop ( "ColorTextVal", GetColorTextVal, SetColorTextVal );
    }
}; // class JColorBox

#endif //__JCOLORBOX_H__