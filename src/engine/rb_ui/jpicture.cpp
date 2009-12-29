//****************************************************************************/
//  File:   JPicture.cpp
//  Date:   28.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "JPicture.h"

//****************************************************************************/
/*  JPicture implementation 
//****************************************************************************/
decl_class(JPicture);
JPicture::JPicture()
{
    m_bTile         = false;
    m_bStretch      = false;
    m_bZEnable      = false;
    m_bMirror       = false;
    m_bAutoSize     = false;
    m_bSpritePivot  = false;
    m_bPixelPrecise = false;
    m_Rotation      = 0.0f;
} // JPicture::JPicture

void JPicture::Init()
{
    if (m_bSpritePivot)
    {
        SetLocalPivot(  g_pDrawServer->GetFramePivot( GetSkinPackID(), GetSkinFrame() ) );
    }
}   

bool JPicture::PtIn( int px, int py ) const 
{ 
    if (!m_bPixelPrecise)
    {
        return GetExt().PtIn( px, py ); 
    }
    else
    {
        Frame ext = GetExt();
        DWORD clr = g_pDrawServer->GetPixel( GetSkinPackID(), GetSkinFrame(), 
             Vec2( px - ext.x, py - ext.y ) );
        DWORD alpha = (clr&0xFF000000)>>24;
        return (alpha > 0x00);
    }
}

void JPicture::Render()
{    
    int w = g_pDrawServer->GetFrameW( GetSkinPackID(), GetSkinFrame() );
    int h = g_pDrawServer->GetFrameH( GetSkinPackID(), GetSkinFrame() );
    if (w == 0 || h == 0) return;
    
    if (m_bAutoSize)
    {
        const Frame lExt = GetLocalExt();
        if (lExt.w != w || lExt.h != h) SetLocalExt( Frame( lExt.x, lExt.y, w, h ) );
    }

    Frame rct = GetExt();

    g_pDrawServer->Flush();
    g_pDrawServer->SetLinFilter( IsFilterFont() );
    g_pDrawServer->SetZEnable( m_bZEnable );

    DWORD color = GetFgColor();

    if (IsBlendAdd())
    {
        g_pDrawServer->SetAdditive();
    }

    if (m_bTile)
    {
        g_pDrawServer->TileSprite( rct, GetSkinPackID(), GetSkinFrame(), color );
    }
    else if (m_bStretch)
    {
        Vec2 pivot = GetLocalPivot();
        rct.x += pivot.x;
        rct.y += pivot.y;

        g_pDrawServer->DrawSprite( rct, GetSkinPackID(), GetSkinFrame(), color, 0.0f, m_bMirror );
    }
    else
    {
        rct.w = w;
        rct.h = h;
        Vec2 pivot = GetLocalPivot();
        rct.x += pivot.x;
        rct.y += pivot.y;
        g_pDrawServer->DrawSprite( rct, GetSkinPackID(), GetSkinFrame(), color, m_Rotation, m_bMirror );
    }

    g_pDrawServer->Flush();   

    if (IsBlendAdd())
    {
        g_pDrawServer->SetAdditive( false );
    }
} // JPicture::Render

void JPicture::DrawBounds()
{
    g_pDrawServer->SetAdditive( true );
    Render();
    g_pDrawServer->SetAdditive( false );
}
