/***********************************************************************************/
//  File:   JLocator.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JLocator.h"
#include "IWindowServer.h"
#include "Mat4.h"

/***********************************************************************************/
/*  JLocator implementation
/***********************************************************************************/
decl_class(JLocator);
JLocator::JLocator()
{
    m_Pos           = Vec3::null;
    m_bScreenSpace  = true;
    m_Rotation      = 0.0f;
    m_Dir           = Vec3::oZ;

    //  FIXME: Locator should be derived from JObject, not JWidget
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
    SetDraggable( true );
} // JLocator::JLocator

void JLocator::DrawBounds()
{
    static int spID = g_pDrawServer->GetSpriteID( "quest_base" );
    const Vec2 c_Pivot( 4.0f, 4.0f );
    const int  c_GizmoFrame = 0;
    if (m_bScreenSpace)
    {
        g_pDrawServer->DrawSprite( m_Pos.x - c_Pivot.x, m_Pos.y - c_Pivot.y, spID, c_GizmoFrame );
    }
    else
    {
        const float c_GizmoWorldScale = 0.04f;
        Mat4 tm;
        tm.st( c_GizmoWorldScale, m_Pos );
        g_pDrawServer->DrawSprite( tm, spID, c_GizmoFrame, 0xFFFFFFFF );
    }
} // JLocator::DrawBounds

void JLocator::SetScreenSpace( bool bSet )
{
    if (bSet == m_bScreenSpace)
    {
        return;
    }
    m_bScreenSpace = bSet;
    Mat4 tm; 
    ProjToScreenTM( tm, g_pRenderServer->GetViewport() );
    if (g_pRenderServer)
    {
        tm = g_pRenderServer->GetViewTM()*g_pRenderServer->GetProjTM()*tm;
    }
    if (!m_bScreenSpace)
    {
        tm = inverse( tm );
    }
    tm.tmproj( m_Pos );
} // JLocator::SetScreenSpace

void JLocator::OnDrag( JDragEvent& e )
{
    if (e.GetType() == deDragStart)
    {
        e.SetDragObject( this );
        e.Consume();
    }
    else if (e.GetType() == deDrag && e.GetDragObject() == this)
    {
        m_Pos.x = e.GetCurPos().x;
        m_Pos.y = e.GetCurPos().y;
        e.Consume();
    }
} // JLocator::OnDrag

bool JLocator::PtIn( int px, int py ) const
{
    if (!IsDrawBounds())
    {
        return false;
    }
    int dx = px - (int)m_Pos.x;
    int dy = py - (int)m_Pos.y;
    static const float c_LocatorRadius = 8.0f;
    bool bIn = dx*dx + dy*dy < c_LocatorRadius*c_LocatorRadius;
    return bIn;
} // JLocator::PtIn