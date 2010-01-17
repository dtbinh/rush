//****************************************************************************/
//  File:   JButton.h
//  Date:   21.09.2004
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JBUTTON_H__
#define __JBUTTON_H__

#include "jwidget.h"

//****************************************************************************/
//  Class:  JButton
//  Desc:   Push button control
//****************************************************************************/
class JButton : public JWidget
{
public:
                    JButton         ();

    virtual void    Render          ();
    virtual void    OnMouse         ( JMouseEvent& m );
    void            Press           ();
    void            Unpress         ();
    virtual void    OnUnpress       () {}
    virtual void    OnPress         () {}

    int             GetNormalFrame  () const { return m_NormalFrame; }
    bool            IsPressed       () const { return m_bPressed; }

    const char*     GetGlyphPack    () const { return m_GlyphPack.c_str(); }
    void            SetGlyphPack    ( const char* name );

    virtual bool    PtIn            ( int px, int py ) const;

public:

    expose( JButton )
    {
        parent( JWidget );
        field ( "Pressed",          m_bPressed                 );
        field ( "TwoState",         m_bTwoState                );
        field ( "PressDX",          m_PressDX                  );
        field ( "PressDY",          m_PressDY                  );
        prop  ( "GlyphPack",        GetGlyphPack, SetGlyphPack );
        field ( "GlyphFrame",       m_GlyphFrame               );

        field ( "NormalFrame",      m_NormalFrame              );
        field ( "PressedFrame",     m_PressedFrame             );
        field ( "HoverFrame",       m_HoverFrame               );
        field ( "DisabledFrame",    m_DisabledFrame            );
        method( "Press",            Press                      );
        method( "Unpress",          Unpress                    );
        field ( "ShadowColor",      m_ShadowColor              );
        field ( "ShadowShiftX",     m_ShadowShiftX             );
        field ( "ShadowShiftY",     m_ShadowShiftY             );
        field ( "PixelPrecise",     m_bPixelPrecise            );
    }
protected:
    bool           m_bPressed;      //  whether button is in pressed state
    bool           m_bTwoState;     //  whether button stays in pressed state when mouse released
    bool           m_bPixelPrecise; //  point-in testing happens on per-pixel basis

    uint32_t          m_ShadowColor;   //  button text shadow color
    int            m_ShadowShiftX;  //  button text shadow x shift
    int            m_ShadowShiftY;  //  button text shadow y shift

    //  button appearance
    JString        m_GlyphPack;     //  button glyph sprite package, if empty - no glyph present
    int            m_GlyphFrame;    //  button glyph frame in the sprite package

    int            m_NormalFrame;   //  sprite frame for button normal state
    int            m_PressedFrame;  //  sprite frame for button pressed state
    int            m_HoverFrame;    //  sprite frame for button being mouse hovered state
    int            m_DisabledFrame; //  sprite frame for button disabled state

    int            m_PressDX;       //  on press button shift x amount
    int            m_PressDY;       //  on press button shift y amount

private:
    int            m_GlyphID;       //  cached glyph sprite package handle

}; // class JButton


#endif // __JBUTTON_H__


