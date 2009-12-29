//****************************************************************************/
//  File:   JProgress.cpp
//  Date:   18.09.2005
//  Author: Ruslan Shestopalyuk 
//****************************************************************************/
#include "stdafx.h"
#include "JProgress.h"

//****************************************************************************/
/*  JProgress implementation
//****************************************************************************/
decl_class(JProgress);
JProgress::JProgress()
{
    m_FillFrame     = -1;
    m_Position      = 0.0f;
    m_bVertical     = false;
    m_bEditable     = true;
    m_LeftBorder    = 0;
    m_TopBorder     = 0;
    m_RightBorder   = 0;
    m_BottomBorder  = 0;

    m_MinVal        = 0.0f;
    m_MaxVal        = 1.0f;

    m_bDiscrete     = false;
    m_DiscreteStep  = 1.0f;

    m_bCaptureOnDrag = true;

} // JProgress::JProgress

void JProgress::OnMouse( JMouseEvent& m )
{
    Frame ext = GetExt();
    if (m.MouseKey() == mkLeft)
    {
        if (m.Action() == aKeyDown && m_bCaptureOnDrag) CaptureMouse( true ); 
        if (m.Action() == aKeyUp && m_bCaptureOnDrag)   CaptureMouse( false );

        Frame fExt( ext );
        fExt.Inflate( -m_LeftBorder, -m_TopBorder, -m_RightBorder, -m_BottomBorder );
        if (m.Action() == aKeyDown || m.Action() == aMouseMove)
        {
            if (m_bVertical)
            {
                m_Position = (m.MouseY() - fExt.y)/fExt.h;
            }
            else
            {
                m_Position = (m.MouseX() - fExt.x)/fExt.w;
            }
            m_Position = clamp( m_Position, 0.0f, 1.0f );

            if (m_bDiscrete)
            {
                int n = int( m_Position*(m_MaxVal - m_MinVal)/m_DiscreteStep );
                m_Position = m_DiscreteStep*float( n )/(m_MaxVal - m_MinVal);
            }

            SendSignal( "Position" );
            SendSignal( "ScaledPosition" );
        }

        if (m.Action() == aMouseMove)
        {
        
        }
        m.Consume();
    }
} // JProgress::OnMouse

void JProgress::Render()
{   
    Frame ext = GetExt();
    DWORD clr = GetFgColor();
    
    Frame vp = g_pRenderServer->GetViewport();
    g_pDrawServer->Flush();
    g_pRenderServer->SetViewport( ext );
    //  draw background
    g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame(), clr );
    
    //  draw fill
    Frame fExt( ext );
    fExt.Inflate( -m_LeftBorder, -m_TopBorder, -m_RightBorder, -m_BottomBorder );
    fExt.w *= m_Position;
    g_pDrawServer->Flush();
    g_pRenderServer->SetViewport( fExt );
    g_pDrawServer->TileSprite( fExt, GetSkinPackID(), m_FillFrame, clr );
    g_pRenderServer->SetViewport( vp );
} // JProgress::Render


