/***********************************************************************************/
/*  File:   JPuzzlePiece.cpp
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JLocation.h"
#include "JQuestGame.h"
#include "JPuzzleGame.h"
#include "JPuzzlePiece.h"

/***********************************************************************************/
/*  JPuzzlePiece implementation
/***********************************************************************************/
decl_class(JPuzzlePiece);
JPuzzlePiece::JPuzzlePiece()
{
    m_HomePos       = Vec2::null;
    m_InitPos       = Vec2::null;
    m_bAtHome       = false;
    m_pPuzzle       = NULL;
    m_HomeDir       = 0.0f;
    m_InitDir       = 0.0f;
    m_Scale         = 1.0f;
    SetFocusable( true );
    SetExt( Frame( 0.0f, 0.0f, 140.0f, 130.0f ) );
} // JPuzzlePiece::JPuzzlePiece

void JPuzzlePiece::Init()
{
    JObject* pParent = GetParent();
    m_pPuzzle = NULL;
    while (pParent && !m_pPuzzle)
    {
        m_pPuzzle = dynamic_cast<JPuzzleGame*>( pParent );
        pParent = pParent->GetParent();
    }
} // JPuzzlePiece:Init

void JPuzzlePiece::MoveToInitPos()
{
    Frame ext = GetLocalExt();
    ext.x = m_InitPos.x;
    ext.y = m_InitPos.y;
    SetLocalExt( ext );
    m_bAtHome = false;
} // JPuzzlePiece::MoveToInitPos

void JPuzzlePiece::Render()
{
    DWORD color = GetFgColor();
    if (IsHovered()) color = GetHoverFgColor();

    float snapDist = 0.0f;
    if (m_pPuzzle) snapDist = m_pPuzzle->GetSnapDist();

    Frame ext = GetLocalExt();
    if (ext.lt().dist( m_HomePos ) < snapDist)
    {
        ext.x = m_HomePos.x;
        ext.y = m_HomePos.y;
        SetLocalExt( ext );
        m_bAtHome = true;
    }
    else
    {
        m_bAtHome = false;
    }

    if (IsDragged())
    {
        //g_pQuestGame->SetCursor( "none" );
    }

    ext = GetExt();
    ext.w = m_Scale*g_pDrawServer->GetFrameW( GetSkinPackID(), GetSkinFrame() );
    ext.h = m_Scale*g_pDrawServer->GetFrameH( GetSkinPackID(), GetSkinFrame() );
    
    g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame(), color );
    g_pDrawServer->Flush();
} // JPuzzlePiece::Render

void JPuzzlePiece::DrawBounds()
{
    Frame ext = GetExt();

    g_pDrawServer->DrawFrame( ext,  0xFFFFFF00 );

    char str[256];
    sprintf( str, "%s: {%d, %d}", GetName(), (int)ext.x, (int)ext.y );
    Vec2 spos( ext.x + 10, ext.cy() );
    g_pDrawServer->DrawString( spos.x, spos.y, -1, str, 0xFFFF4444 );

    ext.w = m_Scale*g_pDrawServer->GetFrameW( GetSkinPackID(), GetSkinFrame() );
    ext.h = m_Scale*g_pDrawServer->GetFrameH( GetSkinPackID(), GetSkinFrame() );

    ext.x = m_HomePos.x;
    ext.y = m_HomePos.y;
    g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame(), 0x4400FF00 );

    ext.x = m_InitPos.x;
    ext.y = m_InitPos.y;
    g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame(), 0x44FF0000 );
} // JPuzzlePiece::DrawBounds

bool JPuzzlePiece::PtIn( int px, int py ) const
{
    int spID = GetSkinPackID();
    int frID = GetSkinFrame();
    Frame ext = GetExt();
    if (spID == -1 || frID == -1)
    {
        return ext.PtIn( px, py );
    }
    Vec2 pixPos( float( px ) - ext.x, float( py ) - ext.y );
    pixPos /= m_Scale;
    DWORD clr = g_pDrawServer->GetPixel( spID, frID, pixPos );
    DWORD alpha = (clr&0xFF000000)>>24;
    return (alpha > 0x50);
} // JPuzzlePiece::PtIn

void JPuzzlePiece::OnDrag( JDragEvent& e )
{
    if (e.GetType() == deDragStart)
    {
        if (m_bAtHome && m_pPuzzle && m_pPuzzle->IsHomeFreeze()) return;
        e.SetObject( this );
        e.Consume();
        BringToFront();
        SetFocus();
        if (m_pPuzzle) m_pPuzzle->OnTakePiece();
    }
    else if (e.GetType() == deDrag && e.GetObject() == this)
    {
        Frame ext = GetExt();
        ext.x = e.GetCurPos().x - e.GetHotSpot().x;
        ext.y = e.GetCurPos().y - e.GetHotSpot().y;
        SetExt( ext );
        e.Consume();
    }
    else if (e.GetType() == deDrop && e.GetObject() == this)
    {
        if (m_pPuzzle) 
        {
            if (m_bAtHome) 
            {
                m_pPuzzle->OnDropRight();
                m_pPuzzle->SetPiecePos( m_HomePos );
            }
            else
            {
                Frame ext = GetLocalExt();
                if (m_pPuzzle->IsDropReturn())
                {
                    ext.x = m_InitPos.x;
                    ext.y = m_InitPos.y;
                    SetLocalExt( ext );
                    m_pPuzzle->SetPiecePos( m_InitPos );
                }
                else
                {
                    m_pPuzzle->SetPiecePos( Vec2( ext.x, ext.y ) );
                }
                m_pPuzzle->OnDropWrong();
                
            }
            SetFocus( false );
        }
    }
} // JPuzzlePiece::OnDrag