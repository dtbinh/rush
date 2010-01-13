//****************************************************************************/
//  File:   JButton.cpp
//  Date:   21.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "JButton.h"

//****************************************************************************/
/*  JButton implementation 
//****************************************************************************/
decl_class(JButton);
JButton::JButton()
{
    m_bPressed          = false;
    m_GlyphID           = -1;     
    m_GlyphFrame        = 0;

    m_ShadowShiftX      = 1;
    m_ShadowShiftY      = 1;
    m_ShadowColor       = 0x00000000;

    m_NormalFrame       = 0;
    m_PressedFrame      = -1;
    m_HoverFrame        = -1;
    m_DisabledFrame     = -1;

    m_PressDX           = 1; 
    m_PressDY           = 1; 
    m_bTwoState         = false;  
    m_bPixelPrecise     = false;
    SetBgColor( 0xFFFFFFFF );

} // JButton::JButton

bool JButton::PtIn( int px, int py ) const
{
    int spID = GetSkinPackID();
    bool bInRect = JWidget::PtIn( px, py );
    if (spID == -1 || m_NormalFrame == -1 || 
        !bInRect || !m_bPixelPrecise)
    {
        return bInRect;
    }
    Frame ext = GetExt();
    Vec2 pixPos( float( px ) - ext.x, float( py ) - ext.y );
    float frameW = g_pDrawServer->GetFrameW( spID, m_NormalFrame );
    float frameH = g_pDrawServer->GetFrameH( spID, m_NormalFrame );
    float scX = frameW/ext.w;
    float scY = frameH/ext.h;
    pixPos.x *= scX;
    pixPos.y *= scY;
    uint32_t clr = g_pDrawServer->GetPixel( spID, m_NormalFrame, pixPos );
    uint32_t alpha = (clr&0xFF000000)>>24;
    return (alpha > 0x50);
} // JPuzzlePiece::PtIn

void JButton::Render()
{    
    Frame ext = GetExt();
    uint32_t clr = GetFgColor();
    int skinFrame = m_NormalFrame;

    if (IsBlendAdd()) 
    {
        g_pDrawServer->SetAdditive( true );
    }
    
    if (m_bPressed && m_PressedFrame != -1)
    {
        skinFrame = m_PressedFrame;
    }

    if (!IsEnabled())
    {
        if (m_DisabledFrame != -1)
        {
            skinFrame = m_DisabledFrame;
        }
        //clr = m_DisableFgColor;
    }

    if (IsHovered()) 
    {
        clr = GetHoverFgColor();
        if (m_HoverFrame != -1) 
        {
            skinFrame = m_HoverFrame;
        }
    }

    g_pDrawServer->Flush();
    if (GetSkinPackID() >= 0)
    {
        g_pDrawServer->DrawSprite( ext, GetSkinPackID(), skinFrame, GetBgColor() );
    }
    g_pDrawServer->SetLinFilter( IsFilterFont() );
    g_pDrawServer->Flush();
    
    const char* text = GetText();
    if (text && text[0] != 0)
    {
        //  draw button text 
        ext.w = g_pDrawServer->GetTextWidth( GetFontID(), text, GetFontHeight() );
        ext.h = GetFontHeight();
        ext = ApplyAlignment( ext, GetExt(), XAlign_Center, YAlign_Center );
        if (m_ShadowColor != 0)
        {
            g_pDrawServer->DrawString( ext.x + m_ShadowShiftX, ext.y + m_ShadowShiftY, 
                GetFontID(), GetText(), m_ShadowColor, GetFontHeight() );
            g_pDrawServer->Flush();
        }
        g_pDrawServer->DrawString( ext.x, ext.y, GetFontID(), GetText(), clr, GetFontHeight() );
    }
    else if (m_GlyphID >= 0)
    {
        //  draw button glyph 
        ext.w = g_pDrawServer->GetFrameW( m_GlyphID, m_GlyphFrame );
        ext.h = g_pDrawServer->GetFrameH( m_GlyphID, m_GlyphFrame );
        ext = ApplyAlignment( ext, GetExt(), XAlign_Center, YAlign_Center );
        g_pDrawServer->DrawSprite( ext.x, ext.y, m_GlyphID, m_GlyphFrame, GetFgColor() );
    }
    g_pDrawServer->Flush();

    if (IsBlendAdd()) 
    {
        g_pDrawServer->SetAdditive( false );
    }
} // JButton::Render

void JButton::OnMouse( JMouseEvent& m )
{
    //  check if button being pressed
    if ((m.Action() == aKeyDown || m.Action() == aDblClick) && 
        m.MouseKey() == mkLeft) 
    { 
        if (m_bTwoState)
        {
            if (m_bPressed) 
            {
                Unpress();  
            }
            else 
            {
                Press();
            }
        } 
        else 
        {
            CaptureMouse( true );
            Press();
        }
        SetFocus();
        m.Consume();
    }

    //  check if being unpressed (for one-state buttons)
    if (m.Action() == aKeyUp && 
        m.MouseKey() == mkLeft && !m_bTwoState) 
    {
        CaptureMouse( false );
        Unpress();
        m.Consume();
    }
    
    //  automatically unpress when leaving button area 
    if (m.Action() == aMouseMove && 
        !GetExt().PtIn( m.MouseX(), m.MouseY() ) && 
        m_bPressed)
    {
        CaptureMouse( false );
        if (!m_bTwoState) 
        {
            Unpress();
        }
    }

    if (IsConsumeEvents()) 
    {
        m.Consume();
    }
} // JButton::OnMouse

void JButton::Press()
{
    if (!m_bPressed)
    {
        Frame ext = GetLocalExt();
        ext.x  += float( m_PressDX );
        ext.y  += float( m_PressDY );
        SetLocalExt( ext );
    }

    m_bPressed = true;
    SendSignal( "Press" );
    SendSignal( "Pressed" );
    OnPress();
} // JButton::Press

void JButton::Unpress()
{
    if (m_bPressed)
    {
        Frame ext = GetLocalExt();
        ext.x  -= float( m_PressDX );
        ext.y  -= float( m_PressDY );
        SetLocalExt( ext );
    }

    m_bPressed = false;
    SendSignal( "Unpress" );
    SendSignal( "Pressed" );
    OnUnpress();
} // JButton::Unpress

void JButton::SetGlyphPack( const char* name ) 
{ 
    m_GlyphPack = name; 
    m_GlyphID = g_pDrawServer->GetSpriteID( name );
} 



