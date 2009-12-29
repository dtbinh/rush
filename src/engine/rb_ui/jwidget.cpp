//****************************************************************************/
//  File:   JWidget.cpp
//  Date:   18.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "IWindowServer.h"
#include "JWidget.h"

Frame ApplyAlignment( const Frame& rcAdjust, const Frame& rcParent, JXAlign xalign, JYAlign yalign )
{
    Frame res = rcAdjust;
    switch (xalign)
    {
    case XAlign_Left:        res.x = rcParent.x;                         break;
    case XAlign_Center:      res.x = rcParent.x + (rcParent.w - res.w)/2;break;
    case XAlign_Right:       res.x = rcParent.x + rcParent.w - res.w;    break;
    case XAlign_Parent:      res.x = rcParent.x; res.w = rcParent.w;     break;
    }

    switch (yalign)
    {
    case YAlign_Top:         res.y = rcParent.y;                         break;
    case YAlign_Center:      res.y = rcParent.y + (rcParent.h - res.h)/2;break;
    case YAlign_Bottom:      res.y = rcParent.y + rcParent.h - res.h;    break;
    case YAlign_Parent:      res.y = rcParent.y; res.h = rcParent.h;     break;
    }
    return res;
} // ApplyAlignment

//****************************************************************************/
/*	JWidget implementation
//****************************************************************************/
decl_class(JWidget);
JWidget::JWidget()
{
    m_Text              = "";       
    m_Font              = "";       
    m_ToolTip           = "";    
    m_FontHeight        = 12; 
    m_FontID            = -1;  
    m_bFilterFont       = true;
    m_bFocusable        = false;

    m_FgColor           = 0xFFFFFFFF;    
    m_HoverFgColor      = 0xFFFFFFFF;    
    m_DisableFgColor    = 0xFF777777;    

    m_BgColor           = 0xFF000000;
    m_HoverBgColor      = 0xFF000000;
    m_DisableBgColor    = 0xFF000000;

    m_LocalExt          = Frame::null;  
    m_LocalPivot        = Vec2::null;

    m_bHasFocus         = false;  
    m_bInited           = false;  
    m_XAlign            = XAlign_None;  
    m_YAlign            = YAlign_None;
    m_bHovered          = false;
    m_bDragged          = false;
    m_bClipToExtents    = false;
    m_bDraggable        = false;
    m_SkinPack          = "";
    m_SkinFrame         = -1;
    m_SkinPackID        = -1;
    m_bConsumeEvents    = false;
    m_bBlendAdd         = false;
    m_bAlwaysCacheSkin  = false;

} // JWidget::JWidget

float JWidget::GetAlpha() const
{
    ColorF clr( GetFgColor() );
    return clr.a;
}

void JWidget::SetAlpha( float alpha )
{
    ColorF bg( GetBgColor() );
    bg.a = alpha;
    SetBgColor( bg );
}

void JWidget::SetBgAlpha( float alpha )
{
    ColorF bg( GetBgColor() );
    bg.a = alpha;
    SetBgColor( bg );
}

float JWidget::GetBgAlpha() const
{
    ColorF clr( GetBgColor() );
    return clr.a;
}

void JWidget::ResInit()
{
    m_SkinPackID = g_pDrawServer->GetSpriteID( m_SkinPack.c_str() );
    m_FontID     = g_pDrawServer->GetSpriteID( m_Font.c_str() );
} // JWidget::ResInit

void JWidget::DrawBounds()
{
	Frame ext = GetExt();
	g_pDrawServer->DrawFrame( ext, 0xFFFFFF00 );
}; // JWidget::DrawBounds

int JWidget::GetFontID() const 
{ 
    //if (m_FontID == -1)
    {
        m_FontID = g_pDrawServer->GetSpriteID( m_Font.c_str() );
    }
    return m_FontID; 
} 

int JWidget::GetFontHeight() const 
{
    if (m_FontHeight == -1)
    {
        return g_pDrawServer->GetFrameH( GetFontID(), 0 );
    }
    return m_FontHeight; 
} 

void JWidget::SetFont( const char* name )
{ 
    m_Font      = name; 
    m_FontID    = g_pDrawServer->GetSpriteID( m_Font.c_str() );
}

void JWidget::SetSkinPack( const char* name )
{
	if (m_SkinPackID != -1) 
    {
        m_SkinPackID = g_pDrawServer->GetSpriteID( name );
    }
	m_SkinPack = name;
} // JWidget::SetSkinPack

int JWidget::GetSkinPackID() const 
{ 
    if (m_SkinPackID < 0)
    {
        m_SkinPackID = g_pDrawServer->GetSpriteID( m_SkinPack.c_str(), NULL, m_bAlwaysCacheSkin );
    }
    return m_SkinPackID; 
}  

void JWidget::CaptureMouse( bool bCapture )
{
    if (bCapture) 
    {
       g_pWindowServer->SetMouseCapture( this ); 
    }
    else if (g_pWindowServer->GetMouseCapture() == this)
    {
        g_pWindowServer->SetMouseCapture( NULL );
    }
}  

void JWidget::SetFocus( bool bSet )
{
    JWidget* pParent = obj_cast<JWidget>( GetParent() );
    if (pParent)
    {
        pParent->SetFocus( bSet );
    }

    if (bSet && m_bFocusable) 
    {
        g_pWindowServer->SetFocus( this );
    }
    else
    {
        g_pWindowServer->SetFocus( NULL );    
    }
} // JWidget::SetFocus



void JWidget::AdjustSize()
{
    Frame parentExt = GetParentExt();
    m_LocalExt = ApplyAlignment( GetExt(), parentExt, m_XAlign, m_YAlign );

    m_LocalExt.x -= parentExt.x; 
    m_LocalExt.y -= parentExt.y;

    OnSize();
} // JWidget::AdjustSize

 void JWidget::FitToChildren()
{
    int nCh = GetNChildren();
    if (nCh == 0) return;
    JWidget* pChild = obj_cast<JWidget>( GetChild( 0 ) );
    Frame ext = pChild->GetLocalExt();
    float lx = ext.x;
    float ly = ext.y;
    for (int i = 1; i < nCh; i++)
    {
        pChild = obj_cast<JWidget>( GetChild( i ) );
        Frame cext = pChild->GetLocalExt();
        cext.x += lx;
        cext.y += ly;
        if (cext.b() > ext.b()) ext.h = cext.b() - ext.y;
        if (cext.r() > ext.r()) ext.w = cext.r() - ext.x;
    }
    
    m_LocalExt = ext;
} // JWidget::FitToChildren



void JWidget::SetText( const char* text ) 
{ 
    m_Text = text; 
} // JWidget::SetText

void JWidget::OnDrag( JDragEvent& e )
{
    if (e.Key() == mkMiddle)
    {
        if (e.GetType() == deDragStart && IsDraggable())
        {
            e.SetDragObject( this );
            m_bDragged = true;
        }
        if (e.GetType() == deDrag && e.GetDragObject() == this)
        {
            Frame ext = GetExt();
            ext += e.GetDelta();
            SetExt( ext );
        }
    }
} // JWidget::OnDrag

void PassMouseEvent( JObject* pObject, JMouseEvent& me, JDragEvent& de )
{
    if (!pObject->IsVisible() || !pObject->IsEnabled()) 
    {
        return; 
    }
    for (int i = pObject->GetNChildren() - 1; i >= 0; i--)
    {
        JWidget* pChildWidget = obj_cast<JWidget>( pObject->GetChild( i ) );
        if (!pChildWidget)
        {
            PassMouseEvent( pObject->GetChild( i ), me, de );
            if (me.Consumed()) return;
            continue;
        }
        pChildWidget->SendMouseEvent( me, de );
        if (me.Consumed() && (de.GetType() == deNone || de.Consumed())) return;
    }
} // PassMouseEvent

void JWidget::SendKeyEvent( JKeyEvent& e, bool bSendToRoot )
{
    if (!IsVisible() || !IsEnabled()) 
    {
        return; 
    }

    int nCh = GetNChildren();
    for (int i = nCh - 1; i >= 0; i--)
    {
        JWidget* pChildWidget = obj_cast<JWidget>( GetChild( i ) );
        if (!pChildWidget)
        {
            continue;
        }
        pChildWidget->SendKeyEvent( e, true );
        if (e.Consumed()) return;
    }
    if (bSendToRoot)
    {
        OnKey( e );
    }
} // JWidget:SendKeyEvent

void JWidget::SendWheelEvent( JWheelEvent& e )
{
    if (!IsVisible() || !IsEnabled()) 
    {
        return; 
    }

    int nCh = GetNChildren();
    for (int i = nCh - 1; i >= 0; i--)
    {
        JWidget* pChildWidget = obj_cast<JWidget>( GetChild( i ) );
        if (!pChildWidget)
        {
            continue;
        }
        pChildWidget->SendWheelEvent( e );
        if (e.Consumed()) return;
    }
    OnWheel( e );
} // JWidget:SendWheelEvent

void JWidget::SendMouseEvent( JMouseEvent& me, JDragEvent& de )
{
    if (!IsVisible() || !IsEnabled()) 
    {
        return; 
    }

    //  pass event to children widgets
    PassMouseEvent( this, me, de );

    if (PtIn( me.MouseX(), me.MouseY() )) 
    {
        if (!IsHovered())
        {
            SendSignal( "OnHover" );
            OnHover();
        }
        SetHovered( true );
        OnMouse( me );

        //  send signal
        if (me.Action() == aKeyDown)
        { 
            if (me.MouseKey() == mkLeft)     SendSignal( "onlbdown" );     
            if (me.MouseKey() == mkMiddle)   SendSignal( "onmbdown" );
            if (me.MouseKey() == mkRight)    SendSignal( "onrbdown" );
        }

        if (me.Action() == aKeyUp)
        {
            if (me.MouseKey() == mkLeft)     SendSignal( "onlbup" );
            if (me.MouseKey() == mkMiddle)   SendSignal( "onmbup" );
            if (me.MouseKey() == mkRight)    SendSignal( "onrbup" );
        }

        if (me.Action() == aClick)
        {
            if (me.MouseKey() == mkLeft)     SendSignal( "onlbclick" );
            if (me.MouseKey() == mkMiddle)   SendSignal( "onmbclick" );
            if (me.MouseKey() == mkRight)    SendSignal( "onrbclick" );
        }

        if (me.Action() == aDblClick)
        {
            if (me.MouseKey() == mkLeft)     SendSignal( "onlbdblclick" );
            if (me.MouseKey() == mkMiddle)   SendSignal( "onmbdblclick" );
            if (me.MouseKey() == mkRight)    SendSignal( "onrbdblclick" );
        }

        //  pass drag event, if appropriate 
        if (de.GetType() != deNone && !de.Consumed())
        {
            OnDrag( de );
            if (de.GetType() == deDragStart && 
                (this == de.GetSource() || this == de.GetDragObject()))
            {
                de.Consume();
                de.m_Type      = deDrag;
                de.m_HotSpot   = Vec2( me.MouseX(), me.MouseY() );
                Frame ext = GetExt();
                de.m_HotSpot.x   -= ext.x;
                de.m_HotSpot.y   -= ext.y;
                if (de.m_pObject) de.m_pObject->SetDragged();
            }
            if (de.Consumed()) 
            {
                me.Consume();
                return;
            }
        }
    }
    else
    {
        //  reset hovered flag
        if (IsHovered())
        {
            SendSignal( "OnStopHover" );
            SetHovered( false );            
        }
    }

}  





