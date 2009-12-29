/***********************************************************************************/
/*  File:   JBlotPiece.cpp
/*  Date:   01.08.2006
/*  Author: Juriy Skolyar
/***********************************************************************************/
#include "stdafx.h"
#include "JPicture.h"
#include "Timer.h"
#include "JRipplePicture.h"
#include "JAnimation.h"
#include "JDialog.h"
#include "JLocation.h"
#include "JQuestGame.h"
#include "JAnimServer.h"
#include "JBlotGame.h"
#include "JBlotPiece.h"


/***********************************************************************************/
/*  JBlotPiece implementation
/***********************************************************************************/
decl_class(JBlotPiece);
JBlotPiece::JBlotPiece()
{
    m_HomePos       = Vec2::null;
    m_MovePos       = Vec2::null;
    m_bAtHome       = false;
    m_pBlot         = NULL;
    m_Scale         = 1.2f;
    m_bSelected     = false;
    m_bRipple       = false;
    m_bMoving       = false;
    SetFocusable( true );
    SetExt( Frame( 0.0f, 0.0f, 140.0f, 130.0f ) );
} // JBlotPiece::JBlotPiece

void JBlotPiece::Init()
{
    JObject* pParent = GetParent();
    m_pBlot = NULL;
    while (pParent && !m_pBlot)
    {
        m_pBlot = dynamic_cast<JBlotGame*>( pParent );
        pParent = pParent->GetParent();
    }
} // JBlotPiece:Init

void JBlotPiece::MoveToHomePos()
{
    Frame ext   = GetLocalExt();
    ext.x       = m_HomePos.x;
    ext.y       = m_HomePos.y;
    m_MovePos.x = ext.x;
    m_MovePos.y = ext.y;
    m_bAtHome   = true;

    SetLocalExt( ext );
} // JBlotPiece::MoveToHomePos

void JBlotPiece::MoveTo( Vec2 pos, bool bImmediately )
{ 
    if (bImmediately)
    {
        Frame ext   = GetLocalExt();
        ext.x       = pos.x;
        ext.y       = pos.y;
        SetLocalExt( ext );
        m_bAtHome = (m_HomePos.dist( pos ) < c_FltEpsilon);
    }
    else
    {
        m_bMoving = true;
        m_StartMoveTime = JAnimServer::s_pInstance->GetCurTime(); 
    }
    m_MovePos = pos; 
}

void JBlotPiece::Render()
{
    DWORD color = GetFgColor();
    float scale;
    if (m_bSelected) scale = m_Scale; else scale = 1.0f;

    //Moving the piece to m_MovePos
    if (m_bMoving)
    {
        Frame ext   = GetExt();
        float t     = JAnimServer::s_pInstance->GetCurTime() - m_StartMoveTime; 
        if (t >= 1.0f)
        {
            t = 1.0f;
            m_bMoving = false;
        }
        else if (t <= 0.0f) 
        {
            t = 0.0f;
        }
        Vec2 cur_pos( ext.x , ext.y );
        Vec2 pos    = cur_pos + t * (m_MovePos - cur_pos);
        ext.x       = pos.x;
        ext.y       = pos.y;
        SetExt( ext );
    }

    if (IsHovered()||m_bSelected) 
    {
        color = GetHoverFgColor();
    }

    Frame ext = GetLocalExt();
    Vec2 pos( ext.x, ext.y );
    float dist = pos.dist( m_HomePos );
    float c_SnapBias = 10.0f;
    if (dist < c_SnapBias)
    {
        m_bAtHome = true; 
    }
    else 
    {
        m_bAtHome = false;
    }

    ext = GetExt();
    ext.w = scale*g_pDrawServer->GetFrameW( GetSkinPackID(), GetSkinFrame() );
    ext.h = scale*g_pDrawServer->GetFrameH( GetSkinPackID(), GetSkinFrame() );
    
    if (m_bRipple)
    {
        //Start Rippling
        s_pThis = this;
        g_pDrawServer->Flush();
        g_pDrawServer->SetVertexCB( RippleCallback );
        g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame(), color );
        g_pDrawServer->Flush();
        g_pDrawServer->SetVertexCB( NULL );
        s_pThis = NULL;
        //Stop Rippling
    } else
    {
        //Simple drawing
        g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame(), color );
        g_pDrawServer->Flush();
    }

    JAnmContext& ctx = JAnimation::PushContext();

    ctx.m_Pos.x = ext.x;
    ctx.m_Pos.y = ext.y;
} // JBlotPiece::Render

void JBlotPiece::PostRender()
{
    JAnimation::PopContext();
} // JBlotPiece::PostRender

void JBlotPiece::DrawBounds()
{
    Frame ext = GetExt();
    float scale;
    if (m_bSelected) scale = m_Scale; else scale = 1.0f;

    g_pDrawServer->DrawFrame( ext,  0xFFFFFF00 );

    char str[256];
    sprintf( str, "%s: {%d, %d}", GetName(), (int)ext.x, (int)ext.y );
    Vec2 spos( ext.x + 10, ext.cy() );
    g_pDrawServer->DrawString( spos.x, spos.y, -1, str, 0xFFFF4444 );

    ext.w = scale*g_pDrawServer->GetFrameW( GetSkinPackID(), GetSkinFrame() );
    ext.h = scale*g_pDrawServer->GetFrameH( GetSkinPackID(), GetSkinFrame() );

    ext.x = m_HomePos.x;
    ext.y = m_HomePos.y;
    g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame(), 0x4400FF00 );

} // JBlotPiece::DrawBounds

void JBlotPiece::OnMouse( JMouseEvent& e )
{
    if (e.MouseKey() == mkLeft && e.Action() == aKeyDown)
    {
        m_bSelected = true;
        //If m_pBlot has no selected pieces
        if (m_pBlot->m_pSelected1 == NULL)
        {
            m_pBlot->m_pSelected1 = this;
        } else
        //If m_pBlot has no second selected piece
        {
            m_pBlot->m_pSelected2 = this;
        }
        m_pBlot->OnSelectPiece();
    }
} // JBlotPiece::OnMouse


bool JBlotPiece::PtIn( int px, int py ) const
{
    int spID = GetSkinPackID();
    int frID = GetSkinFrame();
    Frame ext = GetExt();
    if (spID == -1 || frID == -1)
    {
        return ext.PtIn( px, py );
    }
    Vec2 pixPos( float( px ) - ext.x, float( py ) - ext.y );

    float scale;
    if (m_bSelected) scale = m_Scale; else scale = 1.0f;
    pixPos /= scale;

    DWORD clr = g_pDrawServer->GetPixel( spID, frID, pixPos );
    DWORD alpha = (clr&0xFF000000)>>24;
    return (alpha > 0x50);
} // JBlotPiece::PtIn
