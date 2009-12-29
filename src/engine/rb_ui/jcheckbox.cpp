//****************************************************************************/
//  File:   JCheckBox.cpp
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk 
//****************************************************************************/
#include "stdafx.h"
#include "JButton.h"
#include "JCheckBox.h"

//****************************************************************************/
/*  JCheckBox implementation
//****************************************************************************/
decl_class(JCheckBox);
JCheckBox::JCheckBox()
{
    m_bTwoState = true;
    // default appearance
    SetSkinPack( "button" );
    m_NormalFrame 	= 15;
    m_PressedFrame	= 16;
} // JCheckBox::JCheckBox

void JCheckBox::Check( bool bCheck )
{
    if (IsPressed() != bCheck)
    {
        if (bCheck) 
        {
            Press();
        }
        else
        {
            Unpress();
        }
    }
} // JCheckBox::Check

void JCheckBox::Render()
{   
    Frame    ext = GetExt();
    DWORD   clr = GetFgColor();

    int skinFrame = m_NormalFrame;
    
    if (IsHovered()) 
    {
        clr = GetHoverFgColor();
        skinFrame = m_HoverFrame == -1 ? m_NormalFrame : m_HoverFrame;
    }

    if (m_bPressed)
    {
        skinFrame = m_PressedFrame == -1 ? m_NormalFrame : m_PressedFrame;
    }

    if (GetSkinPackID() >= 0)
    {
        g_pDrawServer->DrawSprite( ext.x, ext.y, GetSkinPackID(), skinFrame, GetBgColor() );
    }
    g_pDrawServer->SetLinFilter( IsFilterFont() );
    g_pDrawServer->Flush();
    
    int fontHeight = GetFontHeight();
    ext.y = ext.b() - fontHeight;
    ext.h = fontHeight; 
    ext = ApplyAlignment( ext, GetExt(), XAlign_Left, YAlign_Center );
    ext.x += g_pDrawServer->GetFrameW( GetSkinPackID(), skinFrame );
    ext.x += 4;

    g_pDrawServer->DrawString( ext.x, ext.y, GetFontID(), GetText(), clr, GetFontHeight() );
} // JCheckBox::Render


