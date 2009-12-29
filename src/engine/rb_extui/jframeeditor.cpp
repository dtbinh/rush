/***********************************************************************************/
//  File:   JFrameEditor.cpp
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JFrameEditor.h"

/***********************************************************************************/
/*  JFrameEditor implementation
/***********************************************************************************/
decl_class(JFrameEditor);
JFrameEditor::JFrameEditor()
{
    SetBgColor( 0xFFEEEE00 );
    SetFgColor( 0xFFEEEE00 );
}

void JFrameEditor::Render()
{
    static int spID = g_pDrawServer->GetSpriteID( "sys_gizmos" );
    g_pDrawServer->DrawFrame( m_Frame, GetBgColor() );
    Vec2 rb( m_Frame.r(), m_Frame.b() );
    g_pDrawServer->DrawSprite( rb.x - 3, rb.y - 3, spID, 1, GetFgColor() );
} // JFrameEditor::Render

static const float c_CornerPickDist = 8.0f;
static const float c_EdgeDragDist = 4.0f;
void JFrameEditor::OnDrag( JDragEvent& e )
{
    if (e.GetType() == deDragStart)
    {
        Vec2 rb( m_Frame.r(), m_Frame.b() );
        if (e.Key() == mkLeft)
        {
            int dragMode = -1;
            if (rb.dist( e.GetStartPos() ) < c_CornerPickDist)
            {
                dragMode = 1;
            }
            else 
            {
                //  check whether dragging by edge
                Seg2 edgeL( m_Frame.x,   m_Frame.y,   m_Frame.x,   m_Frame.b() );
                Seg2 edgeR( m_Frame.r(), m_Frame.y,   m_Frame.r(), m_Frame.b() );
                Seg2 edgeT( m_Frame.x,   m_Frame.y,   m_Frame.r(), m_Frame.y   );
                Seg2 edgeB( m_Frame.x,   m_Frame.b(), m_Frame.r(), m_Frame.b() );
                if (edgeL.dist2( e.GetStartPos() ) < c_EdgeDragDist*c_EdgeDragDist || 
                    edgeR.dist2( e.GetStartPos() ) < c_EdgeDragDist*c_EdgeDragDist || 
                    edgeT.dist2( e.GetStartPos() ) < c_EdgeDragDist*c_EdgeDragDist || 
                    edgeB.dist2( e.GetStartPos() ) < c_EdgeDragDist*c_EdgeDragDist)
                {
                    dragMode = 2;
                }
            }
            if (dragMode != -1)
            {
                e.SetSource( this );
                e.Consume();
                e.SetTag( dragMode );
            }
        }
    }
    
    //  resizing the frame
    if (e.GetType() == deDrag && e.GetTag() == 1 && e.GetSource() == this)
    {
        m_Frame.w += e.GetDelta().x;
        m_Frame.h += e.GetDelta().y;
        SendSignal( "value" );
    }
    
    //  moving the whole frame
    if (e.GetType() == deDrag && e.GetTag() == 2 && e.GetSource() == this)
    {
        m_Frame.x += e.GetDelta().x;
        m_Frame.y += e.GetDelta().y;
        SendSignal( "value" );
    }
} // JFrameEditor::OnDrag

