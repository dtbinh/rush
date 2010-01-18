//****************************************************************************/
//  File:  JCursor.cpp
//  Desc:  
//****************************************************************************/
#include "precompile.h"
#include "JAnimation.h"
#include "JSwitch.h"
#include "JCursor.h"
#include "IWindowServer.h"
#include "windows.h"

//****************************************************************************/
/*  JCursor implementation
//****************************************************************************/
static bool m_bSysCursorOnly = false;
decl_class(JCursor);
JCursor::JCursor()
{
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
    m_bHideSysCursor = false;
    SetFocusable( false );
    m_Position = Vec2::null;
}

JCursor::~JCursor()
{
    if (m_bHideSysCursor) 
    {
        ::ShowCursor( TRUE );
    }
}

 void JCursor::DrawBounds()
 {
    Frame ext = GetExt();
    int step = 3;
    g_pDrawServer->DrawLine( ext.x - step, ext.y, ext.x + step, ext.y, 0xFF0000FF, 0xFF0000FF );
    g_pDrawServer->DrawLine( ext.x, ext.y - step, ext.x, ext.y + step, 0xFF0000FF, 0xFF0000FF );
    g_pDrawServer->Flush();
 } // JCursor::DrawBounds

void JCursor::Render()
{
    if (m_bHideSysCursor)
    {
        while (::ShowCursor( FALSE ) >= 0);
    }
    else
    {
        while (::ShowCursor( TRUE ) < 0);    
    }

    JObject* pActive = GetActiveChild();
    if (!pActive) return;
    Frame ext = GetExt();
    //  change our extents for children to render properly, at mouse position
    if (pActive) 
    {
        SetXAlign( XAlign_None );
        SetYAlign( YAlign_None );
        g_pWindowServer->GetMousePos( m_Position );
        const int c_DummyW = 10;
        SetExt( Frame( m_Position.x, m_Position.y, c_DummyW, c_DummyW ) );
    }
    m_CurExt = ext;

    JAnmContext& ctx = JAnimation::GetContext();
    ctx.m_Pos.x += m_Position.x;
    ctx.m_Pos.y += m_Position.y;
} // JCursor::Render

void JCursor::PostRender()
{
    // restore extents to recieve mouse messages
    SetExt( m_CurExt );
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );

    JAnmContext& ctx = JAnimation::GetContext();
    ctx.m_Pos.x -= m_Position.x;
    ctx.m_Pos.y -= m_Position.y;
} // JCursor::PostRender

void JCursor::HideSysCursor( bool bHide )
{
    m_bHideSysCursor = bHide;
}

void JCursor::SetPosition( Vec2 pos )
{
    m_Position = pos; 
    ::SetCursorPos( pos.x, pos.y );
} // JCursor::SetPosition
    



