//****************************************************************************/
//  File:   JEditBox.cpp
//  Date:   17.10.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include <windows.h>

#include "JEditBox.h"

//****************************************************************************/
/*  JEditBox implementation
//****************************************************************************/
decl_class(JEditBox);
JEditBox::JEditBox()
{
    m_FocusedColor      = 0xAAFFFFFF;
    m_CaretColor        = 0xFFFFFFFF;
    m_SelectionColor    = 0xAA3333FF;
    m_CaretBlinkOn      = 600;
    m_CaretBlinkOff     = 400;
    m_TextLeftMargin    = 2;

    m_TextShift         = 0;
    m_CaretPos          = 0;
    m_SelStart          = 0;
    m_SelEnd            = 0;
    m_bEditable         = true;

    m_FocusFrame        = -1;

    m_ShadowShiftX      = 1;
    m_ShadowShiftY      = 1;
    m_ShadowColor       = 0x00000000;

    m_Spacing           = 0;

    SetFocusable( true );

    SetClipToExt();
} // JEditBox::JEditBox

void JEditBox::Render()
{    
    Frame ext = GetExt();
    Frame vp = g_pRenderServer->GetViewport();
    if (IsClipToExt()) 
    {
        g_pDrawServer->Flush();
        Frame cext( ext );
        cext.Clip( vp );
        g_pRenderServer->SetViewport( cext );
    }

    if (m_FocusFrame >= 0 && HasFocus())
    {
        g_pDrawServer->DrawSprite( ext, GetSkinPackID(), m_FocusFrame, m_FocusedColor );
        g_pDrawServer->Flush();
    }
    else if (GetBgColor() != 0)
    {
        g_pDrawServer->DrawRect( ext, GetBgColor() );
        g_pDrawServer->Flush();
    }

    int     cx  = ext.x + m_TextLeftMargin + m_TextShift;
    int     cy  = ext.y;
    DWORD   clr = GetFgColor();
    if (HasFocus()) 
    {
        DrawSelection();
    }
    g_pDrawServer->SetLinFilter( IsFilterFont() );
    
    if (m_ShadowColor != 0)
    {
        g_pDrawServer->DrawString( cx + m_ShadowShiftX, cy + m_ShadowShiftY, GetFontID(), GetText(), m_ShadowColor, GetFontHeight(), -1, m_Spacing );
        g_pDrawServer->Flush();
    }
    g_pDrawServer->DrawString( cx, cy, GetFontID(), GetText(), clr, GetFontHeight(), -1, m_Spacing ); 

    g_pDrawServer->Flush();
    if (HasFocus()) DrawCaret();
    g_pDrawServer->Flush();

    if (IsClipToExt()) g_pRenderServer->SetViewport( vp );
} // JEditBox::Render

void JEditBox::OnFocus( bool bEnter )
{
    if (!bEnter) SendSignal( "Text" );
}

void JEditBox::Clear()
{
    SetText( "" );
    m_TextShift         = 0;
    m_CaretPos          = 0;
    m_SelStart          = 0;
    m_SelEnd            = 0;
} // JEditBox::Clear

void JEditBox::OnMouse( JMouseEvent& m )
{
    Frame ext = GetExt();

    if (m.Action() == aDblClick && m.MouseKey() == mkLeft)
    {
        const char* pText = GetText();
        int nCh = strlen( pText );

        m_CaretPos = nCh;
        m_SelStart = 0;
        m_SelEnd   = nCh; 

        ResetCaretBlink();
        m.Consume();
    }

    if (m.Action() == aKeyDown && m.MouseKey() == mkLeft)
    {
        SetFocus();
        const char* pText = GetText();
        int nCh = strlen( pText );
        int cX = ext.x + m_TextLeftMargin + m_TextShift;
        int cPos = 0;
        int mX = m.MouseX();
        for (cPos = 0; cPos < nCh; cPos++)
        {
            int chW = g_pDrawServer->GetTextWidth( GetFontID(), &pText[cPos], GetFontHeight(), 1 );
            if (mX <= cX + chW/2) 
            {
                ResetCaretBlink();
                break;
            }
            cX += chW;
        }
        m_CaretPos = cPos;
        m_SelStart = m_CaretPos;
        m_SelEnd   = m_CaretPos; 

        CaptureMouse();
        ResetCaretBlink();
        if (ext.PtIn( m.MouseX(), m.MouseY() ))m.Consume();
    }

    if (m.Action() == aKeyUp && m.MouseKey() == mkLeft)
    {
        CaptureMouse( false );
        m.Consume();
    }
    
    if (m.Action() == aMouseMove && m.MouseKey() == mkLeft)
    {
       SetFocus();
       const char* pText = GetText();
       int nCh = strlen( pText );
       int cX = ext.x + m_TextLeftMargin + m_TextShift;
       int mX = m.MouseX();
       int cPos = 0;
       for (cPos = 0; cPos < nCh; cPos++)
       {
           if (mX <= cX) break;
           cX += g_pDrawServer->GetTextWidth( GetFontID(), &pText[cPos], GetFontHeight(), 1 );
       }
       m_SelEnd   = cPos; 
       m_CaretPos = cPos;
       m.Consume();
    }
} // JEditBox::OnMouse  

void JEditBox::OnKey( JKeyEvent& e )
{
    if (!HasFocus()) return;
    
    //  process control keys down events
    if (e.Action() != aKeyDown) return;
    DWORD keyCode = e.Key();
    if (keyCode == VK_DELETE)
    {
        JString text = GetText(); 
        if (m_SelStart != m_SelEnd)
        {
            if (m_SelStart > m_SelEnd) std::swap( m_SelEnd, m_SelStart );
            text.erase( m_SelStart, m_SelEnd - m_SelStart );
            m_CaretPos  = m_SelStart;
            m_SelEnd    = m_SelStart;
        }
        else
        {
            text.erase( m_CaretPos, 1 );
        }
        SetTextNoNotify( text.c_str() );
        ResetCaretBlink();
        e.Consume();
        return;
    } 
    if (keyCode == VK_BACK && m_CaretPos > 0)
    {
        JString text = GetText();
        if (m_SelStart != m_SelEnd)
        {
            if (m_SelStart > m_SelEnd) std::swap( m_SelEnd, m_SelStart );
            text.erase( m_SelStart, m_SelEnd - m_SelStart );
            m_CaretPos  = m_SelStart;
            m_SelEnd    = m_SelStart;
        }
        else
        {
            text.erase( m_CaretPos - 1, 1 );
            m_CaretPos--;
            if (m_CaretPos < 0) m_CaretPos = 0;
        }
        SetTextNoNotify( text.c_str() );
        ResetCaretBlink();
        e.Consume();
        return;
    }
    if (keyCode == VK_LEFT)
    {
        m_CaretPos -= 1;
        if (m_CaretPos < 0) m_CaretPos = 0;
        ResetCaretBlink();
        if (GetKeyState( VK_SHIFT ) < 0) m_SelEnd--;
        e.Consume();
        return;
    }
    if (keyCode == VK_RIGHT)
    {
        m_CaretPos += 1;
        int nCh = strlen( GetText() );
        if (m_CaretPos > nCh) m_CaretPos = nCh;
        ResetCaretBlink();
        if (GetKeyState( VK_SHIFT ) < 0) m_SelEnd++;
        e.Consume();
        return;
    }

    if (keyCode == VK_RETURN)
    {
        SetFocus( false );
        e.Consume();
        SendSignal( "OnEnter" );
        SendSignal( "Text" );
        CaptureMouse( false );
        return;
    }

    DWORD charCode = e.GetChar();
    if (charCode >= 32)
    {
        JString text = GetText(); 
        if (m_SelStart != m_SelEnd)
        {
            if (m_SelStart > m_SelEnd) std::swap( m_SelEnd, m_SelStart );
            text.erase( m_SelStart, m_SelEnd - m_SelStart );
            m_CaretPos  = m_SelStart;
            m_SelEnd    = m_SelStart;
        }

        char str[2];
        str[0] = charCode; str[1] = 0;
        if (m_CaretPos <= text.size()) text.insert( m_CaretPos, str );
        m_CaretPos++;
        SetTextNoNotify( text.c_str() );
        ResetCaretBlink();
        e.Consume();
        return;
    }

} // JEditBox::OnKey

void JEditBox::DrawCaret()
{
    DWORD tc = GetTickCount() - m_BlinkStart; // FIXME
    Frame ext = GetExt();
    tc %= m_CaretBlinkOn + m_CaretBlinkOff;
    if (tc < m_CaretBlinkOn)
    {
        int textW = g_pDrawServer->GetTextWidth( GetFontID(), GetText(), GetFontHeight(), m_CaretPos );
        int cx = ext.x + textW + m_TextLeftMargin - 1 + m_TextShift;
        
        if (cx > ext.r() - 2)
        {
            m_TextShift += ext.r() - cx;
            cx = ext.x + textW + m_TextLeftMargin - 3 + m_TextShift;
        }

        if (cx < ext.x + 2)
        {
            m_TextShift += ext.x - cx;
            cx = ext.x + textW + m_TextLeftMargin - 1 + m_TextShift;
        }

        ext.y += 2;
        ext.h -= 5;
        g_pDrawServer->DrawLine( cx, ext.y, cx, ext.b(), m_CaretColor, m_CaretColor );
    }
} // JEditBox::DrawCaret

void JEditBox::DrawSelection()
{
    const char* pText = GetText();
    if (m_SelStart == m_SelEnd || !pText || pText[0] == 0) return;
    int nCh = strlen( pText );
    m_SelStart = clamp( m_SelStart, 0, nCh );
    m_SelEnd = clamp( m_SelEnd, 0, nCh );
    int chStart = m_SelStart;
    int chEnd = m_SelEnd;
    if (m_SelStart > m_SelEnd)
    {
        chStart = m_SelEnd;
        chEnd = m_SelStart;
    }

    Frame ext = GetExt();
    int h = GetFontHeight() + 1;
    float selBeg = g_pDrawServer->GetTextWidth( GetFontID(), &pText[0],  h, chStart );
    float selEnd = g_pDrawServer->GetTextWidth( GetFontID(), &pText[0],  h, chEnd );
    Frame rc( ext.x + selBeg + m_TextShift, ext.y + 1, selEnd - selBeg, h );
    g_pDrawServer->DrawRect( rc, m_SelectionColor );
    g_pDrawServer->Flush();
} // JEditBox::DrawSelection

void JEditBox::ResetCaretBlink()
{
    m_BlinkStart = GetTickCount();
} // JEditBox::ResetCaretBlink
