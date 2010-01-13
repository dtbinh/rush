//****************************************************************************/
//  File:   JPolyButton.cpp
//  Date:   21.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "PolyLine2.h"
#include "JPolyButton.h"

//****************************************************************************/
/*  JPolyButton implementation 
//****************************************************************************/
decl_class(JPolyButton);
JPolyButton::JPolyButton()
{
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
}

void JPolyButton::DrawBounds()
{    
    Frame ext = GetParentExt();
    Vec2  base( ext.x, ext.y );
    static int spID = g_pDrawServer->GetSpriteID( "sys_gizmos" );
    int nPt = m_Contour.GetNPoints();
    for (int i = 0; i < nPt; i++)
    {
        Vec2 a = base + m_Contour.GetPoint( i );
        Vec2 b = base + m_Contour.GetPoint( i + 1 );
        g_pDrawServer->DrawSprite( a.x - 3, a.y - 3, spID, 1, 0xFFEEEE00 );
        g_pDrawServer->DrawLine( a.x, a.y, b.x, b.y, 0xFFEEEE00, 0xFFEEEE00 );
    }
} // JPolyButton::DrawBounds

PolyLine2 JPolyButton::GetContour() const
{
    return m_Contour;
} // JPolyButton::GetContour

void JPolyButton::SetContour( PolyLine2 pt )
{
    m_Contour = pt;
} // JPolyButton::SetPointList

bool JPolyButton::PtIn( int px, int py ) const 
{ 
    if (m_Contour.GetNPoints() == 0)
    {
        return JWidget::PtIn( px, py );
    }
    Vec2 pt( px, py );
    Frame pext = GetParentExt();
    pt.x -= pext.x;
    pt.y -= pext.y; 
    return m_Contour.PtIn( pt ); 
}





