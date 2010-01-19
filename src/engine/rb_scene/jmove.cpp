//****************************************************************************/
//  File:  JMove.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "timer.h"
#include "janimation.h"
#include "iwindowserver.h"
#include "jmove.h"

//****************************************************************************/
/*  JMove implementation
//****************************************************************************/
decl_class(JMove);
JMove::JMove()
{
    m_Speed     = 0.0f;
    m_bClosed   = false;
    m_bSmooth   = false;
} // JMove::JMove

void JMove::Render()
{
    JAnimation::Render();
    JAnmContext& ctx = PushContext();

    if (!IsPlaying()) return;

    double t = 0.0f;
    double len = m_Path.GetLength( m_bClosed );
    if (GetDuration() != 0.0f)
    {
        t = double( GetContext().m_Time )/double( GetDuration() );
    }
    else
    {
        t = double( GetContext().m_Time*m_Speed )/double( len );
    }
    if (t > 1.0)
    {
        Stop();
        return;
    }
    Vec2 dir;
    Vec2 pt = m_Path.GetParamPoint( float( t ), m_bClosed, &dir );

    ctx.m_Pos.x += pt.x;
    ctx.m_Pos.y += pt.y;
    ctx.m_State = "move";
    ctx.m_Angle = RadToDeg( atan2f( dir.y, dir.x ) );
    if (ctx.m_Angle < 0.0f) ctx.m_Angle += 360.0f;
} // JMove::Render

void JMove::PostRender()
{
    JAnimation::PostRender();
    PopContext();
} // JMove::PostRender

void JMove::DrawBounds()
{
    //  draw knots
    int nPt = m_Path.GetNPoints();
    static int spID = g_pDrawServer->GetSpriteID( "quest_base" );
    for (int i = 0; i < nPt - 1; i++)
    {
        const Vec2& a = m_Path.GetPoint( i );
        g_pDrawServer->DrawSprite( a.x - 3, a.y - 3, spID, 0, 0xFF000044 );
    }

    if (!m_bSmooth)
    {
        for (int i = 1; i < nPt; i++)
        {
            const Vec2& a = m_Path.GetPoint( i - 1 );
            const Vec2& b = m_Path.GetPoint( i );
            g_pDrawServer->DrawLine( a.x, a.y, b.x, b.y, 0xFF0000FF, 0xFF0000FF );
        }
        if (m_bClosed)
        {
            const Vec2& a = m_Path.GetPoint( 0 );
            const Vec2& b = m_Path.GetPoint( m_Path.GetNPoints() - 1 );
            g_pDrawServer->DrawLine( a.x, a.y, b.x, b.y, 0xFF0000FF, 0xFF0000FF );
        }
    }
    else
    {

    }
    g_pDrawServer->Flush();
} // JMove::DrawBounds




