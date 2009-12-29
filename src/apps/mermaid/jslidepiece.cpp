/***********************************************************************************/
/*  File:   JSlidePiece.cpp
/*  Date:   07.08.2006
/*  Author: Juriy Skolyar
/***********************************************************************************/
#include "stdafx.h"
#include "JLocation.h"
#include "JQuestGame.h"
#include "JAnimServer.h"
#include "JSlideGame.h"
#include "JSlidePiece.h"

/***********************************************************************************/
/*  JSlidePiece implementation
/***********************************************************************************/
decl_class(JSlidePiece);
JSlidePiece::JSlidePiece()
{
    m_pSlideGame        = NULL;
    SetFocusable( true );
    SetExt( Frame( 0.0f, 0.0f, 140.0f, 130.0f ) );
} // JSlidePiece::JSlidePiece

void JSlidePiece::Init()
{
    JObject* pParent = GetParent();
    m_pSlideGame = NULL;

    while (pParent && !m_pSlideGame)
    {
        m_pSlideGame = dynamic_cast<JSlideGame*>( pParent );
        pParent = pParent->GetParent();
    }
} // JSlidePiece:Init

void JSlidePiece::StartFalling()
{
    SetVisible( true );

    int dLine = m_pSlideGame->m_RightLine - m_pSlideGame->m_LeftLine;
    m_Pos.x     = m_pSlideGame->m_LeftLine + rand()%dLine;
    m_Pos.y     = m_pSlideGame->m_TopLine;

    int dSpeed = m_pSlideGame->m_MaxSpeed - m_pSlideGame->m_MinSpeed;
    m_Speed     = m_pSlideGame->m_MinSpeed + rand()%dSpeed;
    m_StartTime = JAnimServer::s_pInstance->GetCurTime();
} // JSlidePiece:StartFalling

void JSlidePiece::Render()
{
    m_Pos.y = m_pSlideGame->m_TopLine + m_Speed*(JAnimServer::s_pInstance->GetCurTime() - m_StartTime);

    if (IsVisible())
    {   
        DWORD color = GetFgColor();
        if (IsHovered()) color = GetHoverFgColor();

        Frame ext = GetExt();
        ext.x = m_Pos.x;
        ext.y = m_Pos.y;
        SetExt( ext );

        ext.w = g_pDrawServer->GetFrameW( GetSkinPackID(), GetSkinFrame() );
        ext.h = g_pDrawServer->GetFrameH( GetSkinPackID(), GetSkinFrame() );
       
        g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame(), color );
        g_pDrawServer->Flush();
    }
} // JSlidePiece::Render

void JSlidePiece::DrawBounds()
{
    Frame ext = GetExt();

    g_pDrawServer->DrawFrame( ext,  0xFFFFFF00 );

    char str[256];
    sprintf( str, "%s: {%d, %d}", GetName(), (int)ext.x, (int)ext.y );
    Vec2 spos( ext.x + 10, ext.cy() );
    g_pDrawServer->DrawString( spos.x, spos.y, -1, str, 0xFFFF4444 );

    ext.w = g_pDrawServer->GetFrameW( GetSkinPackID(), GetSkinFrame() );
    ext.h = g_pDrawServer->GetFrameH( GetSkinPackID(), GetSkinFrame() );
} // JSlidePiece::DrawBounds

void JSlidePiece::OnMouse( JMouseEvent& e )
{
    if (e.MouseKey() == mkLeft && e.Action() == aKeyDown && 
        IsVisible())
    {
        if (GetTag() == m_pSlideGame->GetCatchID())
        {
            m_pSlideGame->OnTakeRight();
        } 
        else
        {
            m_pSlideGame->OnTakeWrong();
        }
        SetVisible( false );
        StartFalling();
        e.Consume();
    }
} // JSlidePiece::OnMouse

bool JSlidePiece::PtIn( int px, int py ) const
{
    int spID = GetSkinPackID();
    int frID = GetSkinFrame();
    Frame ext = GetExt();
    if (spID == -1 || frID == -1)
    {
        return ext.PtIn( px, py );
    }
    Vec2 pixPos( float( px ) - ext.x, float( py ) - ext.y );

    DWORD clr = g_pDrawServer->GetPixel( spID, frID, pixPos );
    DWORD alpha = (clr&0xFF000000)>>24;
    return (alpha > 0x50);
} // JSlidePiece::PtIn
