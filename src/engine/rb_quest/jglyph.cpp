/***********************************************************************************/
//  File:   JGlyph.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JGlyph.h"
#include "JAnimation.h"
#include "IWindowServer.h"
#include "JQuestGame.h"
#include "Mat3.h"

/***********************************************************************************/
/*  JGlyph implementation
/***********************************************************************************/
decl_class(JGlyph);

JGlyph::JGlyph()
{
    m_Pos               = Vec3::null;
    m_Scale             = 1.0f;

    m_Rotation          = 0.0f;
    m_bMirrored         = false;
    m_Color             = 0xFFFFFFFF;
    m_DragMode          = GlyphDrag_None;

    m_bHideCursor       = false;
    m_bDragging         = false;
    m_DragHotspot       = Vec2::null;
    m_DragStartPos      = Vec3::null;
    m_Transform         = Mat3::identity;

    m_bPickChildren     = false;

    const float c_BigNum = 10000.0f;
    SetLocalExt( Frame( -c_BigNum, -c_BigNum, c_BigNum*2.0f, c_BigNum*2.0f ) );
} // JGlyph::JGlyph

void JGlyph::PostRender()
{
    JAnimation::PopContext();
} // JGlyph::PostRender

void JGlyph::Render()
{
    JAnmContext& ctx = JAnimation::PushContext();

    if (m_bDragging)
    {
        Vec2 mousePos;
        g_pWindowServer->GetMousePos( mousePos );
        m_Pos.x = m_DragHotspot.x + mousePos.x;
        m_Pos.y = m_DragHotspot.y + mousePos.y;
    }

    ctx.m_Pos.x += m_Pos.x;
    ctx.m_Pos.y += m_Pos.y;

    ctx.m_Scale *= m_Scale;
    ctx.m_Angle += m_Rotation;

    int spID = GetSkinPackID();
    int frID = GetSkinFrame();
    Vec2 pivot = g_pDrawServer->GetFramePivot( spID, frID );
    //  cache global transform matrix
    m_Transform.srt( Vec2( m_bMirrored ? -ctx.m_Scale : ctx.m_Scale, ctx.m_Scale ), 
        DegToRad( ctx.m_Angle ), Vec2( ctx.m_Pos.x, ctx.m_Pos.y ) );
    
    float w = g_pDrawServer->GetFrameW( spID, frID );
    float h = g_pDrawServer->GetFrameH( spID, frID );
    float scale = ctx.m_Scale;

    Frame rect( ctx.m_Pos.x, ctx.m_Pos.y, w*scale, h*scale );
    //  snap to integer coordinates
    rect.x = floorf( rect.x );
    rect.y = floorf( rect.y );
    rect.w = floorf( rect.w );
    rect.h = floorf( rect.h );

    bool bLinFilter = (_fabs( m_Scale - 1.0f ) > c_FltEpsilon) || (_fabs( ctx.m_Angle ) > c_FltEpsilon);
    if (bLinFilter)
    {
        g_pDrawServer->SetAdjustUV( true );
    }
    g_pDrawServer->SetLinFilter( bLinFilter );
    g_pDrawServer->DrawSprite( rect, spID, frID, m_Color, DegToRad( ctx.m_Angle ), m_bMirrored );
    g_pDrawServer->Flush();
    if (bLinFilter)
    {
        g_pDrawServer->SetAdjustUV( false );
    }
} // JGlyph::Render

void JGlyph::DrawBounds()
{
    JAnmContext& ctx = JAnimation::GetContext();
    
    int spID = GetSkinPackID();
    int frID = GetSkinFrame();
    Vec2 pivot = g_pDrawServer->GetFramePivot( spID, frID );
    Frame r( -pivot.x, -pivot.y, 
             g_pDrawServer->GetFrameW( spID, frID ), g_pDrawServer->GetFrameH( spID, frID ) );
    Vec3 lt( r.x,   r.y,    1.0f );
    Vec3 rt( r.r(), r.y,    1.0f );
    Vec3 rb( r.r(), r.b(),  1.0f );
    Vec3 lb( r.x,   r.b(),  1.0f );

    m_Transform.tm( lt );
    m_Transform.tm( rt );
    m_Transform.tm( rb );
    m_Transform.tm( lb );

    const DWORD c_BoundClr = 0xFF5555FF;
    g_pDrawServer->DrawLine( lt.x, lt.y, rt.x, rt.y, c_BoundClr, c_BoundClr );
    g_pDrawServer->DrawLine( rt.x, rt.y, rb.x, rb.y, c_BoundClr, c_BoundClr );
    g_pDrawServer->DrawLine( rb.x, rb.y, lb.x, lb.y, c_BoundClr, c_BoundClr );
    g_pDrawServer->DrawLine( lb.x, lb.y, lt.x, lt.y, c_BoundClr, c_BoundClr );
    
    Vec2 pos( m_Transform.e20, m_Transform.e21 );
    g_pDrawServer->DrawLine( pos.x - 3, pos.y, pos.x + 3, pos.y, c_BoundClr, c_BoundClr );
    g_pDrawServer->DrawLine( pos.x, pos.y - 3, pos.x, pos.y + 3, c_BoundClr, c_BoundClr );

    g_pDrawServer->Flush();
} // JGlyph::DrawBounds

bool JGlyph::PtIn( int px, int py ) const
{
    //  probably it is not the most intuitive thing to do, but I suppose, that if glyph is being dragged
    //  mouse should be always inside it
    if (m_bDragging) 
    {
        return true;
    }

    int spID = GetSkinPackID();
    int frID = GetSkinFrame();
    if (spID != -1 && frID != -1)
    {
        Vec2 pivot = g_pDrawServer->GetFramePivot( spID, frID );
        Vec3 pos( float( px ), float( py ), 1.0f );
        Mat3 tm;
        tm.inverse( m_Transform );
        tm.tm( pos );
        DWORD clr = g_pDrawServer->GetPixel( spID, frID, Vec2( pos.x + pivot.x, pos.y + pivot.y ) );
        DWORD alpha = (clr&0xFF000000)>>24;
        bool bIn = (alpha > 0x50);
        if (bIn) 
        {
            return true;
        }
    }
    if (!m_bPickChildren)
    {
        return false;
    }
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JGlyph* pChild = obj_cast<JGlyph>( GetChild( i ) );
        if (!pChild) continue;
        if (pChild->PtIn( px, py ))
        {
            return true;
        }
    }
    return false;
} // JGlyph::PtIn

void JGlyph::StartDrag()
{
    Vec2 mousePos;
    g_pWindowServer->GetMousePos( mousePos );
    m_DragHotspot.x = m_Pos.x - mousePos.x;
    m_DragHotspot.y = m_Pos.y - mousePos.y;
    m_DragStartPos = m_Pos;
    m_bDragging = true;
    if (m_bHideCursor)
    {
        g_pQuestGame->ShowCursor( false );
    }
    
    SendSignal( "onstartdrag" );
} // JGlyph::StartDrag

void JGlyph::StopDrag()
{
    if (!m_bDragging) return;
    
    m_bDragging  = false;
    if (m_bHideCursor)
    {
        g_pQuestGame->ShowCursor( true );
    }
    
    SendSignal( "ondrop" );

    m_DragStartPos = m_Pos;
} // JGlyph::StopDrag

void JGlyph::OnMouse( JMouseEvent& e )
{
    if (e.Action() == aKeyDown)
    {
        if (e.MouseKey() == mkLeft)     
        {
            if (m_DragMode == GlyphDrag_Sticky)
            {
                if (m_bDragging)
                {
                    StopDrag();
                }
                else
                {
                    StartDrag();
                }
                e.Consume();
            }
            if (m_DragMode == GlyphDrag_Normal)
            {
                StartDrag();
                e.Consume();
            }
        }
    }

    if (e.Action() == aKeyUp)
    {
        if (e.MouseKey() == mkLeft)     
        {
            if (m_DragMode == GlyphDrag_Normal)
            {
                StopDrag();
                e.Consume();
            }
        }
    }
} // JGlyph::OnMouse