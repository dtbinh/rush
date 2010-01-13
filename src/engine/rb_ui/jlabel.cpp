//****************************************************************************/
//  File:   JLabel.cpp
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "JLabel.h"

//****************************************************************************/
/*  JLabel implementation
//****************************************************************************/
decl_class(JLabel);
JLabel::JLabel()
{
    m_TextXAlign    = XAlign_None;
    m_TextYAlign    = YAlign_None;
    m_MarginX       = 3;
    m_MarginY       = 0;
    m_ShadowShiftX  = 1;
    m_ShadowShiftY  = 1;
    m_ShadowColor   = 0x00000000;
    m_bWrapText     = false;
    m_RowsGap       = 0;
    m_Spacing       = 0;

    SetBgColor( 0x00000000 );
    SetFocusable( false );

} // JLabel::JLabel 

struct TextWrap
{
    Frame       ext;
    const char* pStr;
    int         nChar;
};

void JLabel::Render()
{   
    Frame    pext = GetExt();
    Frame    ext  = pext;
    if (GetBgColor() != 0 && strlen( GetText() ) != 0 )
    {
        g_pDrawServer->DrawRect( ext, GetBgColor() );
        g_pDrawServer->Flush();
    }

    g_pDrawServer->SetLinFilter( IsFilterFont() );
    
    if (!m_bWrapText)
    {
        ext.w = g_pDrawServer->GetTextWidth( GetFontID(), GetText(), GetFontHeight() );
        ext.h = GetFontHeight();    
        ext = ApplyAlignment( ext, pext, m_TextXAlign, m_TextYAlign );
        DrawSubstring( ext, GetText() );
    }
    else
    {
        std::vector<TextWrap> lines;

        int nChar = 0;
        int nCharLeft = strlen( GetText() );
        int curW = 0;
        const char* pStr = GetText();
        while (true)
        {
            if (nCharLeft == 0) break;
            //  rewind to the end of the next token
            int len = strcspn( pStr + nChar, " ,-:;.\n\r\t!?" );
            len += strspn( pStr + nChar + len, " ,-:;.\n\r\t!?" );
            
            //  get width of the next token, in pixels
            int tokenW = g_pDrawServer->GetTextWidth( GetFontID(), pStr, GetFontHeight(), len, m_Spacing );
            
            if (curW + tokenW > pext.w  ||  //  text does not fit with the next token
                len == 0)                   //  reached the end of the string
            {
                ext.w = curW;
                Frame cext = ApplyAlignment( ext, pext, m_TextXAlign, YAlign_None );
                
                TextWrap wrap;
                wrap.ext   = cext;
                wrap.nChar = nChar;
                wrap.pStr  = pStr;

                lines.push_back( wrap );

                ext.y += GetFontHeight() + m_RowsGap;
                pStr += nChar;
                nCharLeft -= nChar;
                if (nChar == 0) 
                {
                    break;
                }
                nChar = 0;
                curW = 0;
            }
            else
            {
                nChar += len;
                curW += tokenW;
            }
        }

        float dY = 0;
        if (m_TextYAlign == YAlign_Center)
        {
            dY = (pext.h - (ext.y - pext.y))*0.5f;
        }
        else if (GetYAlign() == YAlign_Bottom)
        {
            dY = pext.h - (ext.y - pext.y);
        }
        int nLines = lines.size();
        for (int i = 0; i < nLines; i++)
        {
            const TextWrap& wrap = lines[i];
            Frame cext = wrap.ext;
            cext.y += dY;
            DrawSubstring( cext, wrap.pStr, wrap.nChar );
        }
    }

    if (GetYAlign() == YAlign_Content)
    {
        pext.h = ext.y - pext.y;
        SetExt( pext );
    }

    g_pDrawServer->Flush();
} // JLabel::Render

void JLabel::DrawSubstring( const Frame& ext, const char* text, int nChar )
{
    if (!text) return;
    if (nChar == -1) nChar = strlen( text );
    uint32_t   clr  = GetFgColor();
    if (m_ShadowColor != 0)
    {
        g_pDrawServer->DrawString( ext.x + m_MarginX + m_ShadowShiftX, ext.y + m_MarginY + m_ShadowShiftY, 
            GetFontID(), text, m_ShadowColor, GetFontHeight(), nChar, m_Spacing );
        g_pDrawServer->Flush();
    }
    g_pDrawServer->DrawString( ext.x + m_MarginX, ext.y + m_MarginY, GetFontID(), 
        text, clr, GetFontHeight(), nChar, m_Spacing ); 
} // JLabel::DrawSubstring


