/***********************************************************************************/
//  File:   JAngleEditor.cpp
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JAngleEditor.h"

/***********************************************************************************/
/*  JAngleEditor implementation
/***********************************************************************************/
decl_class(JAngleEditor);
JAngleEditor::JAngleEditor()
{
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );

    SetBgColor( 0xFFEEEE00 );
    SetFgColor( 0xFFEEEE00 );

    m_Angle = 0.0f;
    m_Pivot = Vec2::null;
    m_OuterRadius = 30;
    m_InnerRadius = 5;
    m_DragSector  = 0.5f;
}

void JAngleEditor::Render()
{
    const int c_ArrowSpriteID = 3;
    static int spID = g_pDrawServer->GetSpriteID( "sys_gizmos" );
    g_pDrawServer->DrawSprite( m_Pivot.x, m_Pivot.y, spID, c_ArrowSpriteID, GetFgColor(), DegToRad( m_Angle ) + c_PI*0.5f );
} // JAngleEditor::Render

void JAngleEditor::OnDrag( JDragEvent& e )
{
    if (e.GetType() == deDragStart && e.Key() == mkLeft)
    {
        Vec2 dir = e.GetStartPos() - m_Pivot;
        float len = dir.norm();
        float ang = wrap( atan2f( dir.y, dir.x ), c_Epsilon, c_PI*2.0f );
        if (len >= m_InnerRadius && len <= m_OuterRadius && 
            fabs( ang - DegToRad( m_Angle ) ) < m_DragSector)
        {
            e.SetSource( this );
            e.Consume();
        }
    }
    
    //  perform rotation 
    if (e.GetType() == deDrag && e.GetSource() == this)
    {
        Vec2 dir = e.GetCurPos() - m_Pivot;
        dir.normalize();
        m_Angle = wrap( RadToDeg( atan2f( dir.y, dir.x ) ), 0.0f, 360.0f );
        SendSignal( "angle" );
    }
    
} // JAngleEditor::OnDrag

