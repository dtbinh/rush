/***********************************************************************************/
//  File:   JRenderTarget.cpp
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JLight.h"
#include "JRenderTarget.h"

/***********************************************************************************/
/*  JRenderTarget implementation
/***********************************************************************************/
decl_class(JRenderTarget);
JRenderTarget::JRenderTarget()
{
    m_Width         = 0;
    m_Height        = 0;
    m_ClearColor    = 0;
    m_TexID         = -1;
} // JRenderTarget::JRenderTarget

void JRenderTarget::Render()
{
} // JRenderTarget::Render

void JRenderTarget::PostRender()
{
} // JRenderTarget::PostRender

void JRenderTarget::Init()
{
} // JRenderTarget::Init


/*

void JQuestGame::CreateThumbnail()
{
    //  render one frame of the game to the offscreen render target
    if (m_ThumbRT == -1) return;
    g_pDrawServer->SetZEnable( false );
    g_pRenderServer->SetRenderTarget( m_ThumbRT );
    g_pRenderServer->ClearDepthStencil( true, false );
    bool bShadowsEnabled = g_pModelServer->ShadowsEnabled();
    g_pModelServer->EnableShadows( false );
    
    RenderTree();
    
    g_pRenderServer->SetRenderTarget( -1 );
    g_pModelServer->EnableShadows( bShadowsEnabled );

    //  create temporary helper surface in managed pool and get pixels from render target
    TextureProperties texProp;
    texProp.m_Format    = ColorFormat_RGB565;
    texProp.m_Width     = m_ThumbSrcRct.w;
    texProp.m_Height    = m_ThumbSrcRct.h;
    texProp.m_PoolType  = PoolType_System;
    texProp.m_NMips     = 1;
    strcpy( texProp.m_Name, "TempThumbnail" );
    m_ThumbSurfID = g_pRenderServer->CreateTexture( texProp );

    //  extract pixels to floating-point surface
    g_pRenderServer->CopyTexture( m_ThumbSurfID, m_ThumbRT );
    m_ThumbSnapshot.ReadFromTexture( m_ThumbSurfID );
    g_pRenderServer->DeleteTexture( m_ThumbSurfID );

    //  create downsampled thumbnail 
    m_ThumbSnapshot.Downsample();
    m_ThumbSnapshot.Downsample();

    // add alpha mask
    int maskID = g_pDrawServer->GetSpriteID( "save_thumb_mask" );
    if (maskID != -1)
    {
        int w = m_ThumbSnapshot.GetW();
        int h = m_ThumbSnapshot.GetH();
        Vec2 pt;
        for (int j = 0; j < h; j++)
        {
            for (int i = 0; i < w; i++)
            {
                pt.x = i;
                pt.y = j;
                uint32_t pixel = g_pDrawServer->GetPixel( maskID, 0, pt );
                ColorF color( pixel );
                m_ThumbSnapshot.SetAlpha( i, j, color.a );
            }
        }
    }
} // JQuestGame::CreateThumbnail

void JQuestGame::LoadThumbnails()
{
    m_CurThumbID = -1;
    g_pDrawServer->ResetSprite( m_ThumbSpriteID );
    SendSignal( "SaveGameExists" );
} // JQuestGame::LoadThumbnails

void JQuestGame::UpdateThumbnail( int sprID, const Frame& ext, BYTE* pPixels, ColorFormat fmt )
{
    g_pQuestGame->OnUpdateThumbnail( sprID, ext, pPixels, fmt );
} // JQuestGame::UpdateThumbnail

void JQuestGame::OnUpdateThumbnail( int sprID, const Frame& ext, BYTE* pPixels, ColorFormat fmt )
{
    int thumbID = sprID/2;
    if (m_CurThumbID != thumbID)
    {
        char path[_MAX_PATH];
        GetSavegamePath( path, thumbID );

        FInStream is( path );
        if (!is) return;

        is >> m_ThumbImage;
        m_ThumbImageBW = m_ThumbImage;
        m_ThumbImageBW.ToGreyScale();
        m_CurThumbID = thumbID;
    }
    
    ColorSurface& surf = (sprID&1) ? m_ThumbImageBW : m_ThumbImage;
    int nRows  = ext.h;
    int nInRow = ext.w;
    int nDestBytesPerPixel = 4;
    ColorF* pSrcPixels = surf.GetPixels() + int( ext.x ) + int( ext.y )*surf.GetW();
    for (int j = 0; j < nRows; j++)
    {
        ConvertPixels( pPixels + j*nInRow*nDestBytesPerPixel, fmt, 
            (BYTE*)(pSrcPixels + j*surf.GetW()), ColorFormat_ARGB32F, nInRow );
    }
} // JQuestGame::UpdateThumbnail
*/
