//****************************************************************************/
//  File:  JSlider.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JSlider.h"

//****************************************************************************/
/*  JSlider implementation
//****************************************************************************/
decl_class(JSlider);
JSlider::JSlider()
{
    m_ThumbBeg          = -1;
    m_ThumbMid          = -1;
    m_ThumbEnd          = -1;

    m_Position          = 0.0f;
    m_Expand            = 0.0f;
    m_bSnapToClick      = false;
    m_bVertical         = false;
    m_bCaptureOnDrag    = true;
} // JSlider::JSlider

void JSlider::OnMouse( JMouseEvent& m )
{
    Frame ext = GetExt();
    if (m.MouseKey() == mkLeft)
    {
        if (m.Action() == aKeyDown && m_bCaptureOnDrag) CaptureMouse( true ); 
        if (m.Action() == aKeyUp && m_bCaptureOnDrag)   CaptureMouse( false );

        if (m.Action() == aKeyDown || m.Action() == aMouseMove)
        {
            m_Position = m_bVertical ? (m.MouseY() - ext.y)/ext.h : (m.MouseX() - ext.x)/ext.w;
            m_Position = clamp( m_Position, 0.0f, 1.0f - m_Expand );
            SendSignal( "Position" );
        }

        if (m.Action() == aMouseMove)
        {
        
        }
        m.Consume();
    }
} // JSlider::OnMouse

void JSlider::Render()
{   
    Frame    ext = GetExt();
    DWORD   clr  = GetFgColor();
    
    //  draw background
    g_pDrawServer->TileSprite( ext, GetSkinPackID(), GetSkinFrame(), GetBgColor() );
    
    float bw = g_pDrawServer->GetFrameW( GetSkinPackID(), m_ThumbBeg );
    float bh = g_pDrawServer->GetFrameH( GetSkinPackID(), m_ThumbBeg );
    
    float mw = g_pDrawServer->GetFrameW( GetSkinPackID(), m_ThumbMid );
    float mh = g_pDrawServer->GetFrameH( GetSkinPackID(), m_ThumbMid );

    float ew = g_pDrawServer->GetFrameW( GetSkinPackID(), m_ThumbEnd );
    float eh = g_pDrawServer->GetFrameH( GetSkinPackID(), m_ThumbEnd );

    //  draw thumb
    Frame rBeg( ext.x, ext.y, bw, bh );
    Frame rMid( ext.x, ext.y, mw, mh );
    Frame rEnd( ext.x, ext.y, ew, eh );

    if (m_bVertical)
    {
        if (m_ThumbBeg != -1)
        {
            rMid.h = floorf( tmax( rMid.h, ext.h*m_Expand - bh  - eh ) );
            float thumbH = bh + rMid.h + eh;
            rBeg.y = ext.y + floorf( ext.h*m_Position );
            rMid.y = rBeg.b();
            rEnd.y = rMid.b();
        }
        else
        {
            rMid.y = ext.y + ext.h*m_Position - mh*0.5f;
        }
    }
    else
    {
        if (m_ThumbBeg != -1)
        {
            rMid.w = floorf( tmax( rMid.w, ext.w*m_Expand - bw - ew ) );
            float thumbW = bw + rMid.w + ew;
            rBeg.x = ext.y + floorf( ext.w*m_Position );
            rMid.x = rBeg.r();
            rEnd.x = rMid.r();
        }
        else
        {
            rMid.x = ext.x + ext.w*m_Position - mw*0.5f;
        }
    }
    
    g_pDrawServer->DrawSprite( rBeg, GetSkinPackID(), m_ThumbBeg, GetFgColor() );
    g_pDrawServer->TileSprite( rMid, GetSkinPackID(), m_ThumbMid, GetFgColor() );
    g_pDrawServer->DrawSprite( rEnd, GetSkinPackID(), m_ThumbEnd, GetFgColor() );

} // JSlider::Render


