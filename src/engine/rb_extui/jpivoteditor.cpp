/***********************************************************************************/
//  File:   JPivotEditor.cpp
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "IDrawServer.h"
#include "JPivotEditor.h"

/***********************************************************************************/
/*  JPivotEditor implementation
/***********************************************************************************/
decl_class(JPivotEditor);
JPivotEditor::JPivotEditor()
{
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
    SetFgColor( 0xFFEEEEFF );
}

void JPivotEditor::Render()
{
    int spID = g_pDrawServer->GetSpriteID( "sys_gizmos" );
    g_pDrawServer->DrawSprite( m_Pivot.x - 3, m_Pivot.y - 3, spID, 1, GetFgColor() );
} // JPivotEditor::Render

void JPivotEditor::OnDrag( JDragEvent& e )
{
    Vec3 mPos( e.GetStartPos().x, e.GetStartPos().y, 0.0f );
    if (e.GetType() == deDragStart)
    {
        if (e.Key() == mkLeft)
        {
            int dragMode = -1;
            const float c_PivotDragRadius = 5.0f;
            if (mPos.dist( m_Pivot ) < c_PivotDragRadius)
            {
                e.SetSource( this );
                e.Consume();
            }
        }
    }
    
    //  moving the pivot
    if (e.GetType() == deDrag && e.GetSource() == this)
    {
        m_Pivot.x += e.GetDelta().x;
        m_Pivot.y += e.GetDelta().y;
        SendSignal( "value" );
    }    
} // JPivotEditor::OnDrag

