/***********************************************************************************/
//  File:   JPolyLineEditor.cpp
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JPolyLineEditor.h"
#include "windows.h"

/***********************************************************************************/
/*  JPolyLineEditor implementation
/***********************************************************************************/
decl_class(JPolyLineEditor);
JPolyLineEditor::JPolyLineEditor()
    : m_SelPt( -1 )
{
    m_bSmooth = false;
    m_bClosed = true;
    SetBgColor( 0xFFEEEE00 );
    SetFgColor( 0xFFEEEE00 );
}

void JPolyLineEditor::Render()
{
    int spID = g_pDrawServer->GetSpriteID( "sys_gizmos" );
    int nPt = m_PolyLine.GetNPoints();
    if (nPt == 0) return;

    for (int i = 0; i < nPt; i++)
    {
        const Vec2& a = m_PolyLine.GetPoint( i );
        const Vec2& b = m_PolyLine.GetPoint( i + 1 );
        g_pDrawServer->DrawSprite( a.x - 3, a.y - 3, spID, 1, GetFgColor() );
        if (m_bClosed || i < nPt - 1)
        {
            g_pDrawServer->DrawLine( a.x, a.y, b.x, b.y, GetBgColor(), GetBgColor() );
        }
    }
    if (m_SelPt >= 0)
    {
        const Vec2& a = m_PolyLine.GetPoint( m_SelPt );
        g_pDrawServer->DrawRect( Frame( a.x - 5, a.y - 5, 10, 10 ), 0xFFFF0000 );
    }
} // JPolyLineEditor::Render

const float c_PLPointSelArea = 8.0f;
const float c_EdgeDragDist   = 4.0f;
void JPolyLineEditor::OnDrag( JDragEvent& e )
{
    if (e.GetType() == deDragStart)
    {
        if (e.Key() == mkLeft)
        {
            int dragMode = -1;
            if (m_PolyLine.PickPt( e.GetStartPos(), c_PLPointSelArea ) != -1)
            {
                dragMode = 1;
            }
            else 
            {
                //  check whether dragging by edge
                int edgeIdx = m_PolyLine.GetNearestEdge( e.GetStartPos(), m_bClosed );
                if (edgeIdx >= 0)
                {
                    Seg2 edge = m_PolyLine.GetEdge( edgeIdx );
                    if (edge.dist2( e.GetStartPos() ) < c_EdgeDragDist*c_EdgeDragDist)
                    {
                        dragMode = 2;
                    }
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
    
    //  moving the selected point
    if (e.GetType() == deDrag && e.GetTag() == 1 && e.GetSource() == this)
    {
        if (m_SelPt < 0 || m_SelPt >= m_PolyLine.GetNPoints()) return;
        m_PolyLine[m_SelPt] += e.GetDelta();
        SendSignal( "value" );
    }
    
    //  moving the whole polyline
    if (e.GetType() == deDrag && e.GetTag() == 2 && e.GetSource() == this)
    {
        m_PolyLine.Move( e.GetDelta() );
        SendSignal( "value" );
    }
} // JPolyLineEditor::OnDrag

void JPolyLineEditor::OnMouse( JMouseEvent& e )
{
    if (e.MouseKey() == mkLeft && e.Action() == aKeyDown)
    {
        Vec2 mPos( e.MouseX(), e.MouseY() );
        if (GetKeyState( VK_CONTROL ) < 0)
        {
            //  adding new point
            int minEdge = m_PolyLine.GetNearestEdge( mPos, m_bClosed );
            if (minEdge >= 0 && 
                m_PolyLine.GetEdge( minEdge ).dist2( mPos ) < c_EdgeDragDist*c_EdgeDragDist)
            {
                m_PolyLine.InsertPoint( minEdge + 1, mPos );
                m_SelPt = minEdge + 1;
                SendSignal( "value" );
            }
        }
        else
        {
            //  selecting point
            int minPt = m_PolyLine.PickPt( mPos, c_PLPointSelArea );
            if (minPt >= 0) 
            {
                m_SelPt = minPt;
                e.Consume();
            }
        }
    }   
} // JPolyLineEditor::OnMouse

void JPolyLineEditor::OnKey( JKeyEvent& e )
{
    //  deleting the point
    if (e.Key() == VK_DELETE && e.Action() == aKeyDown)
    {
        m_PolyLine.DeletePoint( m_SelPt );
        if (m_SelPt >= m_PolyLine.GetNPoints()) 
        {
            m_SelPt = m_PolyLine.GetNPoints() - 1;
        }
        e.Consume();
        SendSignal( "value" );
    }
} // JPanel::OnKey