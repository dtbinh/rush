/***********************************************************************************/
//  File:   JAreaControl.cpp
//  Date:   29.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JCharacter.h"
#include "JQuestGame.h"
#include "JCharControl.h"
#include "JAreaControl.h"

/***********************************************************************************/
/*  JAreaControl implementation
/***********************************************************************************/
decl_class(JAreaControl);
JAreaControl::JAreaControl()
{
    
} // JAreaControl::JAreaControl

void JAreaControl::Render()
{
    JCharControl::Render();

    if (IsHovered())
    {
        g_pQuestGame->SetCursor( "walk" );
    }
} // JAreaControl::Render

void JAreaControl::DrawBounds()
{
    static int spID = g_pDrawServer->GetSpriteID( "quest_base" );
    int nPt = m_Area.GetNPoints();
    for (int i = 0; i < nPt; i++)
    {
        const Vec2& a = m_Area.GetPoint( i );
        const Vec2& b = m_Area.GetPoint( i + 1 );
        g_pDrawServer->DrawSprite( a.x - 3, a.y - 3, spID, 1, 0xFF00EE00 );
        g_pDrawServer->DrawLine( a.x, a.y, b.x, b.y, 0xFF00EE00, 0xFF00EE00 );
    }
    JCharControl::DrawBounds();
} // JAreaControl::DrawBounds

bool JAreaControl::MoveTo( const Vec3& pos, bool bImmediately, float direction )
{
    if (!GetChar())
    {
        return false;
    }
    Vec3 spos = GetChar()->GetPos();
    Seg2 moveSeg( Vec2( spos.x, spos.y ), Vec2( pos.x, pos.y ) );
    static PolyLine2 path;
    if (!m_Area.FindPath( moveSeg, path )) return false;
    if (path.GetNPoints() == 0) return false;
    ClearPath();
    for (int i = 0; i < path.GetNPoints(); i++)
    {
        const Vec2& pt = path.GetPoint( i );
        JCharControl::MoveTo( Vec3( pt.x, pt.y, 0.0f ), false, direction );
    }
    if (m_MoveState == CharMove_Run || m_MoveState == CharMove_Walk)
    {
        MoveToPath();
    }
    return true;
} // JAreaControl::MoveTo

void JAreaControl::OnMouse( JMouseEvent& e )
{
    if (!GetChar()) return;
    if (e.MouseKey() == mkLeft && e.Action() == aKeyDown)
    {
        Vec3 dpos( e.MouseX(), e.MouseY(), 0.0f );
        MoveTo( dpos, true );
    }
} // JAreaControl::OnMouse
