//****************************************************************************/
//  File:   JWidget.h
//  Date:   18.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JWIDGET_H__
#define __JWIDGET_H__

#include "jmouseevent.h"
#include "jkeyevent.h"
#include "jwheelevent.h"
#include "jdragevent.h"
#include "jmathtypecast.h"

//****************************************************************************/
//  Enum:  JXAlign
//  Desc:  Describes horizontal alignment type
//****************************************************************************/
enum JXAlign
{
    XAlign_None          = 0,
    XAlign_Left          = 1,
    XAlign_Center        = 2,
    XAlign_Right         = 3,
    XAlign_Parent        = 4,
    XAlign_Content       = 5,   //  alignment to the control content
}; // enum JXAlign

enum_beg(JXAlign)
    enum_val( "None",       XAlign_None   ),
    enum_val( "Left",       XAlign_Left   ),
    enum_val( "Center",     XAlign_Center ),
    enum_val( "Right",      XAlign_Right  ),
    enum_val( "Parent",     XAlign_Parent ),
    enum_val( "Content",    XAlign_Content )
enum_end(JXAlign)

//****************************************************************************/
//  Enum:  JYAlign
//  Desc:  Describes vertical alignment type
//****************************************************************************/
enum JYAlign
{
    YAlign_None           = 0,
    YAlign_Top            = 1,
    YAlign_Center         = 2,
    YAlign_Bottom         = 3,
    YAlign_Parent         = 4,
    YAlign_Content        = 5,
}; // enum JYAlign

enum_beg(JYAlign)
    enum_val( "None",       YAlign_None    ),
    enum_val( "Top",        YAlign_Top     ),
    enum_val( "Center",     YAlign_Center  ),
    enum_val( "Bottom",     YAlign_Bottom  ),
    enum_val( "Parent",     YAlign_Parent  ),
    enum_val( "Content",    YAlign_Content )
enum_end(JYAlign)

class JMouseEvent;
class JKeyEvent;
class JDragEvent;
class JWheelEvent;

//****************************************************************************/
//  Class:  JWidget
//  Desc:   Base class for all ui controls
//****************************************************************************/
class JWidget : public JObject
{
public:
                    JWidget         ();

    //  frame
    Frame           GetExt          () const;
    void            SetExt          ( Frame ext );

    Frame           GetParentExt    () const;

    Frame           GetLocalExt     () const { return m_LocalExt; }
    void            SetLocalExt     ( Frame ext );

    Vec2            GetPivot        () const;
    void            SetPivot        ( Vec2 pt );

    Vec2            GetLocalPivot   () const { return m_LocalPivot; }
    void            SetLocalPivot   ( Vec2 pt );

    void            Move            ( const Vec2& delta ) { m_LocalExt += delta; }
    virtual bool    PtIn            ( int px, int py ) const { return GetExt().PtIn( px, py ); }

    float           GetRight        () const { return GetExt().r(); }
    float           GetBottom       () const { return GetExt().b(); }
    float           GetTop          () const { return GetExt().y; }
    float           GetLeft         () const { return GetExt().x; }

    float           GetWidth        () const { return GetExt().w; }
    float           GetHeight       () const { return GetExt().h; }

    void            SetRight        ( float val );
    void            SetBottom       ( float val );
    void            SetLeft         ( float val );
    void            SetTop          ( float val );
    void            SetWidth        ( float val );
    void            SetHeight       ( float val );

    Vec2            GetLocalPos     () const;
    void            SetLocalPos     ( Vec2 pos );
    Vec2            GetPos          () const;
    void            SetPos          ( Vec2 pos );

    bool            IsClipToExt     () const { return m_bClipToExtents; }
    void            SetClipToExt    ( bool bClip = true ) { m_bClipToExtents = bClip; }

    //  tint
    uint32_t           GetFgColor      () const { return m_FgColor; }
    uint32_t           GetBgColor      () const { return m_BgColor; }

    void            SetFgColor      ( uint32_t color ) { m_FgColor = color; }
    void            SetBgColor      ( uint32_t color ) { m_BgColor = color; }

    uint32_t           GetHoverFgColor () const { return m_HoverFgColor; }
    uint32_t           GetHoverBgColor () const { return m_HoverBgColor; }

    float           GetAlpha        () const;
    void            SetAlpha        ( float alpha );

    float           GetBgAlpha      () const;
    void            SetBgAlpha      ( float alpha );

    bool            IsBlendAdd      () const { return m_bBlendAdd; }
    void            SetBlendAdd     ( bool bClip = true ) { m_bBlendAdd = bClip; }

    //  text
    int             GetFontID       () const;
    const char*     GetFont         () const { return m_Font.c_str(); }
    void            SetFont         ( const char* name );

    const char*     GetText         () const { return m_Text.c_str(); }
    void            SetText         ( const char* text );

    const char*     GetCursor       () const { return m_Cursor.c_str(); }
    void            SetCursor       ( const char* cursor ) { m_Cursor = cursor; }

    int             GetFontHeight   () const;
    void            SetFontHeight   ( int h ) { m_FontHeight = h; }
    bool            IsFilterFont    () const { return m_bFilterFont; }

    //  focus
    bool            IsFocusable     () const { return m_bFocusable; }
    bool            HasFocus        () const { return m_bHasFocus; }
    void            SetFocus        ( bool bSet = true );
    void            SetFocusable    ( bool bVal ) { m_bFocusable = bVal; }
    void            CaptureMouse    ( bool bCapture = true );

    //  drag-n-drop
    bool            IsDragged       () const { return m_bDragged; }
    void            SetDragged      ( bool bDragged = true ) { m_bDragged = bDragged; }
    bool            IsDraggable     () const { return m_bDraggable; }
    void            SetDraggable    ( bool bSet = true ) { m_bDraggable = bSet; }
    void            SetHovered      ( bool bVal = true ) { m_bHovered = bVal; }
    bool            IsHovered       () const { return m_bHovered; }

    //  layout
    void            FitToChildren   ();
    JXAlign         GetXAlign       () const { return m_XAlign; }
    JYAlign         GetYAlign       () const { return m_YAlign; }
    void            SetXAlign       ( JXAlign align ) { m_XAlign = align; AdjustSize(); }
    void            SetYAlign       ( JYAlign align ) { m_YAlign = align; AdjustSize(); }

    //  skin
    int             GetSkinPackID   () const;
    int             GetSkinFrame    () const { return m_SkinFrame; }
    void            SetSkinFrame    ( int val ) { m_SkinFrame = val; }
    const char*     GetSkinPack     () const { return m_SkinPack.c_str(); }
    void            SetSkinPack     ( const char* name );

    //  messages
    bool            IsConsumeEvents () const { return m_bConsumeEvents; }
    void            SetConsumeEvents( bool bConsume = true ) { m_bConsumeEvents = bConsume; }

    void            SendMouseEvent  ( JMouseEvent& me, JDragEvent& de );
    void            SendKeyEvent    ( JKeyEvent& e, bool bSendToRoot = true );
    void            SendWheelEvent  ( JWheelEvent& e );

    //  resource
    virtual void    ResInit         ();

    //  bounds
    virtual void    DrawBounds      ();

protected:

    //  event dispatchers
    virtual void    OnMouse         ( JMouseEvent& e ){}
    virtual void    OnWheel         ( JWheelEvent& e ){}
    virtual void    OnDrag          ( JDragEvent&  e );
    virtual void    OnKey           ( JKeyEvent&   e ){}
    virtual void    OnFocus         ( bool bEnter ){}

    virtual void    OnSize          () {}
    virtual void    OnHover         () {}
    virtual void    OnStopHover     () {}
    void            OnLBDown        () {}
    void            OnLBUp          () {}
    void            OnLBClick       () {}
    void            OnLBDblClick    () {}

    void            OnMBDown        () {}
    void            OnMBUp          () {}
    void            OnMBClick       () {}
    void            OnMBDblClick    () {}

    void            OnRBDown        () {}
    void            OnRBUp          () {}
    void            OnRBClick       () {}
    void            OnRBDblClick    () {}

    void            AdjustSize      ();

    void            SetTextNoNotify ( const char* text ) { m_Text = text; }

    expose( JWidget )
    {
        parent( JObject );
        prop ( "Text",              GetText, SetText            );
        prop ( "Extents",           GetLocalExt, SetLocalExt    );
        prop ( "Pivot",             GetLocalPivot, SetLocalPivot);
        prop ( "PivotAbs:d",        GetPivot, SetPivot          );
        prop ( "Position:d",        GetLocalPos, SetLocalPos    );
        prop ( "PositionAbs:d",     GetPos, SetPos              );

        prop ( "Font",              GetFont, SetFont            );
        field( "FontID:d",          m_FontID                    );
        field( "FontHeight",        m_FontHeight                );
        field( "FilterFont",        m_bFilterFont               );
        field( "FgColor",           m_FgColor                   );
        field( "BgColor",           m_BgColor                   );
        field( "HoverFgColor",      m_HoverFgColor              );
        field( "HoverBgColor",      m_HoverBgColor              );
        field( "DisableFgColor",    m_DisableFgColor            );
        field( "DisableBgColor",    m_DisableBgColor            );
        field( "BlendAdd",          m_bBlendAdd                 );
        field( "Cursor",            m_Cursor                    );

        prop( "Alpha:d",            GetAlpha, SetAlpha          );
        prop( "BgAlpha:d",          GetBgAlpha, SetBgAlpha      );


        field( "AlwaysCacheSkin",   m_bAlwaysCacheSkin          );

        //  hud widget
        field( "ConsumeEvents",     m_bConsumeEvents            );
        field( "ClipToExt",         m_bClipToExtents            );
        prop ( "HasFocus",          HasFocus, SetFocus          );
        field( "Draggable",         m_bDraggable                );
        prop ( "XAlign",            GetXAlign, SetXAlign        );
        prop ( "YAlign",            GetYAlign, SetYAlign        );
        field( "Focusable",         m_bFocusable                );

        prop ( "SkinPack",          GetSkinPack, SetSkinPack    );
        prop ( "SkinFrame",         GetSkinFrame, SetSkinFrame  );

        field( "Left:d",            m_LocalExt.x );
        field( "Top:d",             m_LocalExt.y );
        field( "Width:d",           m_LocalExt.w );
        field( "Height:d",          m_LocalExt.h );
        prop ( "ExtentsAbs:d",      GetExt, SetExt );

        //  events
        method( "OnHover",      OnHover      );
        method( "OnStopHover",  OnStopHover  );
        method( "OnLBDown",     OnLBDown     );
        method( "OnLBUp",       OnLBUp       );
        method( "OnLBClick",    OnLBClick    );
        method( "OnLBDblClick", OnLBDblClick );
        method( "OnMBDown",     OnMBDown     );
        method( "OnMBUp",       OnMBUp       );
        method( "OnMBClick",    OnMBClick    );
        method( "OnMBDblClick", OnMBDblClick );
        method( "OnRBDown",     OnRBDown     );
        method( "OnRBUp",       OnRBUp       );
        method( "OnRBClick",    OnRBClick    );
        method( "OnRBDblClick", OnRBDblClick );
    }

    friend class                JWindowServer;
private:
    JString             m_Text;             //  textual contents of widget (depends of context)
    JString             m_ToolTip;          //  when empty, no tooltip is displayed

    JString             m_SkinPack;         //  name of the sprite package containing skin elements
    JString             m_Font;             //  main widget text font

    int                 m_FontHeight;
    mutable int         m_FontID;
    bool                m_bFilterFont;
    bool                m_bFocusable;
    bool                m_bDraggable;

    bool                m_bAlwaysCacheSkin;

    uint32_t               m_FgColor;
    uint32_t               m_HoverFgColor;
    uint32_t               m_DisableFgColor;

    uint32_t               m_BgColor;
    uint32_t               m_HoverBgColor;
    uint32_t               m_DisableBgColor;

    bool                m_bHasFocus;
    bool                m_bHovered;
    bool                m_bDragged;
    bool                m_bClipToExtents;
    bool                m_bInited;
    bool                m_bBlendAdd;
    bool                m_bConsumeEvents;
    JXAlign             m_XAlign;
    JYAlign             m_YAlign;

    JString             m_Cursor;

    int                 m_SkinFrame;
    mutable int         m_SkinPackID;

    Frame               m_LocalExt;             //  widget extents, relative to parent
    Vec2                m_LocalPivot;           //  widget pivot point
}; // class JWidget

Frame ApplyAlignment( const Frame& rcAdjust, const Frame& rcParent, JXAlign xalign, JYAlign yalign );

//****************************************************************************/
/*  Inline Functions
//****************************************************************************/
inline Frame JWidget::GetExt() const
{
    Frame ext( m_LocalExt );
    JObject* pParent = GetParent();
    while (pParent)
    {
        JWidget* pParentWidget = obj_cast<JWidget>( pParent );
        if (pParentWidget)
        {
            ext.x += pParentWidget->m_LocalExt.x;
            ext.y += pParentWidget->m_LocalExt.y;
        }
        pParent = pParent->GetParent();
    }
    return ext;
} // JWidget::GetExt

inline void JWidget::SetExt( Frame ext )
{
    Frame parentExt = GetParentExt();
    ext.x -= parentExt.x;
    ext.y -= parentExt.y;
    SetLocalExt( ext );
} // JWidget::SetExt

inline void JWidget::SetLocalExt( Frame ext )
{
    m_LocalExt = ext;
} // JWidget::SetLocalExt

inline Vec2 JWidget::GetPivot() const
{
    Frame ext = GetExt();
    return Vec2( m_LocalPivot.x + ext.x, m_LocalPivot.y + ext.y );
}

inline void JWidget::SetPivot( Vec2 pt )
{
    Frame ext = GetExt();
    pt.x -= ext.x;
    pt.y -= ext.y;
    SetLocalPivot( pt );
}

inline void JWidget::SetLocalPivot( Vec2 pt )
{
    m_LocalPivot = pt;
}

inline Vec2 JWidget::GetLocalPos() const
{
    Vec2 pos;
    pos.x = m_LocalExt.x + m_LocalPivot.x;
    pos.y = m_LocalExt.y + m_LocalPivot.y;
    return pos;
}

inline void JWidget::SetLocalPos( Vec2 pos )
{
    m_LocalExt.x = pos.x - m_LocalPivot.x;
    m_LocalExt.y = pos.y - m_LocalPivot.y;
}

inline Vec2 JWidget::GetPos() const
{
    Vec2 pos;
    Frame ext = GetExt();
    pos.x = ext.x + m_LocalPivot.x;
    pos.y = ext.y + m_LocalPivot.y;
    return pos;
}

inline void JWidget::SetPos( Vec2 pos )
{
    Frame ext = GetExt();
    ext.x = pos.x - m_LocalPivot.x;
    ext.y = pos.y - m_LocalPivot.y;
    SetExt( ext );
}

inline Frame JWidget::GetParentExt() const
{
    JObject* pParent = GetParent();
    while (pParent)
    {
        JWidget* pParentWidget = obj_cast<JWidget>( pParent );
        if (pParentWidget)
        {
            return pParentWidget->GetExt();
        }
        pParent = pParent->GetParent();
    }
    return m_LocalExt;
} // JWidget::GetParentExt

#endif // __JWIDGET_H__


