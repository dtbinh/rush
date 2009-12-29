/***********************************************************************************/
//  File:   JColorBox.cpp
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JColorBox.h"
#include "PickColorDialog.h"

/***********************************************************************************/
/*  JColorBox implementation
/***********************************************************************************/
decl_class(JColorBox);

JColorBox::JColorBox()
{
    SetColorVal( 0xFFFFFFFF );
    m_bHasAlpha = true;
} // JColorBox::JColorBox

void JColorBox::SetColorTextVal( const char* val )
{
    m_ColorTextVal = val;
    cast( m_ColorVal, m_ColorTextVal );
} // JColorBox::SetColorTextVal

void JColorBox::SetColorVal( DWORD val )
{
    m_ColorVal = val;
    char buf[16];
    sprintf( buf, "%X", val );
    m_ColorTextVal = buf;
} // JColorBox::SetColorVal

void JColorBox::OnMouse( JMouseEvent& e )
{
    if (e.Action() == aKeyDown && e.MouseKey() == mkLeft)
    {
        Frame cExt = GetColorExt();
        Frame aExt = GetAlphaExt();
        if (cExt.PtIn( e.MouseX(), e.MouseY() ))
        {
            PickColorDialog dlg( m_ColorVal );
            ::ShowCursor( TRUE );
            bool bPicked = dlg.Show();
            ::ShowCursor( FALSE );
            if (!bPicked) return;
            DWORD clr = 0x00FFFFFF & dlg.GetColor();
            m_ColorVal &= 0xFF000000;
            m_ColorVal |= clr;
            SetColorVal( m_ColorVal );
            SendSignal( "ColorVal" );
            e.Consume();
            SetFocus();
        }
    }
} // JColorBox::OnMouse

void JColorBox::Render()
{
    Frame ext = GetExt();
    ext.Inflate( -1 );
    Frame cExt = GetColorExt();
    Frame aExt = GetAlphaExt();
    
    DWORD clr = m_ColorVal|0xFF000000;
    DWORD a = (m_ColorVal&0xFF000000)>>24;
    DWORD aclr = 0xFF000000|(a<<16)|(a<<8)|a;

    g_pDrawServer->DrawRect( cExt, clr  );
    if (m_bHasAlpha) 
    {
        g_pDrawServer->DrawRect( aExt, aclr );
    }
    g_pDrawServer->DrawSprite( ext.x, ext.y, GetSkinPackID(), GetSkinFrame(), GetFgColor() );
} // JColorBox::Render

Frame JColorBox::GetColorExt() const
{
    Frame ext = GetExt();
    ext.Inflate( -1 );
    if (m_bHasAlpha) ext.w *= 0.75f;
    return ext;
} // JColorBox::GetColorExt

Frame JColorBox::GetAlphaExt() const
{
    Frame ext = GetExt();
    ext.Inflate( -1 );
    ext.x += ext.w*0.75f;
    ext.w *= 0.25f;
    if (!m_bHasAlpha)
    {
        ext.w = 0.0f;
        ext.x = ext.r();
    }
    return ext;
} // JColorBox::GetAlphaExt
