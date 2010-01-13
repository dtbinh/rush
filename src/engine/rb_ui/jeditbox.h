//****************************************************************************/
//  File:   JEditBox.h
//  Date:   17.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JEDITBOX_H__
#define __JEDITBOX_H__

#include "JWidget.h"

//****************************************************************************/
//  Class:  JEditBox
//  Desc:   Edit box button control
//****************************************************************************/
class JEditBox : public JWidget
{
public:
                    JEditBox        ();
    
    virtual void    Render          ();
    virtual void    OnMouse         ( JMouseEvent& m );
    virtual void    OnKey           ( JKeyEvent&   e );
    void            ResetCaretBlink ();
    void            OnFocus         ( bool bEnter );
    virtual void    OnEnter         () {}
    void            Clear           ();

    expose( JEditBox )
    {
        parent( JWidget );
        field( "FocusedColor",      m_FocusedColor      );
        field( "SelectionColor",    m_SelectionColor    );
        field( "CaretColor",        m_CaretColor        );
        field( "FocusFrame",        m_FocusFrame        );
        method( "OnEnter",          OnEnter             );
        field( "Spacing",           m_Spacing           );
        field( "ShadowColor",       m_ShadowColor       );
        field( "ShadowShiftX",      m_ShadowShiftX      );
        field( "ShadowShiftY",      m_ShadowShiftY      );
    }

protected:
    uint32_t           m_FocusedColor;     //  color to draw widget when focused
    uint32_t           m_SelectionColor;   //  color of the text selection
    uint32_t           m_CaretColor;       

    uint32_t           m_ShadowColor;  //  color of the text's shadow, when 0 - no shadow is drawn
    int             m_ShadowShiftX;
    int             m_ShadowShiftY;

    float           m_Spacing;

    bool            m_bEditable;        //  whether user can edit contents right now
    int             m_CaretPos;         //  current position of caret, in symbols
    int             m_SelStart;         //  current selection start, in symbols
    int             m_SelEnd;           //  current selection end, in symbols
    int             m_FocusFrame;       

    uint32_t           m_CaretBlinkOn;     //  time (ms) of caret visible, when blinking
    uint32_t           m_CaretBlinkOff;    //  time (ms) of caret invisible, when blinking
    uint32_t           m_BlinkStart;       //  time when the caret started blinking

    int             m_TextLeftMargin;   
    int             m_TextShift;

    void            DrawCaret();
    void            DrawSelection();

}; // class JEditBox

#endif // __JEDITBOX_H__


