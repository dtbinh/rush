#include "stdafx.h"
#include "JString.h"
#include "FStream.h"
#include "IFileServer.h"
#include "LoaderTGA.h"
#include "colorsurface.h"
#include "Buffer.h"
#include "Logger.h"
#include "IWindowServer.h"
#include "drawservergdi.h"

/***********************************************************************************/
/*  DrawServerGDI implementation
/***********************************************************************************/
decl_class(DrawServerGDI);

DrawServerGDI::DrawServerGDI()
{
    SetName( "draw" );
    g_pDrawServer = this;

    m_hDC           = NULL;
    m_hBackDC       = NULL;
    m_hBackBitmap   = NULL;

    m_BackW         = 0;
    m_BackH         = 0;
}

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif 

void DrawServerGDI::Init() 
{
    HWND hWnd = (HWND)g_pWindowServer->GetRootHandle();
    //SetWindowLong( hWnd, GWL_EXSTYLE, GetWindowLong( hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );

    RECT rt;
    GetClientRect( hWnd, &rt );

    m_BackW = rt.right - rt.left;
    m_BackH = rt.bottom - rt.top;

    m_hDC = GetDC( hWnd );
    m_hBackDC = CreateCompatibleDC( m_hDC );
    m_hBackBitmap = CreateCompatibleBitmap( m_hDC, m_BackW, m_BackH );
    SelectObject( m_hBackDC, m_hBackBitmap );
}

int DrawServerGDI::GetSpriteID( const char* packageName, PSpriteUpdateCB updateCB, bool bAlwaysCache) 
{
    int nPacks = m_Packages.size();
    for (int i = 0; i < nPacks; i++)
    {
        if (!stricmp( m_Packages[i].m_Name.c_str(), packageName ))
        {
            return i;
        }
    }

    m_Packages.push_back( BitmapPackage() );
    BitmapPackage& bp = m_Packages.back();
    bp.m_Name = packageName;

    //  search for the frames
    int curFrame = 0;
    JString frameName;
    JString path;
    while (true)
    {
        frameName = bp.m_Name;
        frameName += ToString( curFrame );
        if (!g_pFileServer->FindMedia( frameName.c_str(), "tga", &path ))
        {
            break;
        }
        //  load bitmap
        FInStream is;
        g_pFileServer->OpenMedia( frameName.c_str(), "tga", is );
        ColorSurface image;
        bool bRes = LoadTGA( is, image );
        if (bRes)
        {
            int nPixels = image.GetW()*image.GetH();
            
            BitmapFrame bf;
            bf.m_Width  = image.GetW();
            bf.m_Height = image.GetH();

            BITMAPINFO bmi;
            bf.m_hDC = CreateCompatibleDC( m_hDC );
    
            ZeroMemory( &bmi, sizeof( BITMAPINFO ) );
            
            bmi.bmiHeader.biSize        = sizeof( BITMAPINFOHEADER );
            bmi.bmiHeader.biWidth       = image.GetW();
            bmi.bmiHeader.biHeight      = image.GetH();
            bmi.bmiHeader.biPlanes      = 1;
            bmi.bmiHeader.biBitCount    = 32;         
            bmi.bmiHeader.biCompression = BI_RGB;
            bmi.bmiHeader.biSizeImage   = nPixels*4;

            // create our DIB section and select the bitmap into the dc
            void* pvPixels = NULL;
            bf.m_hBitmap = CreateDIBSection( bf.m_hDC, &bmi, DIB_RGB_COLORS, &pvPixels, NULL, 0x0 );
            DWORD* pPixels = (DWORD*)pvPixels;
            
            if (bf.m_hBitmap == NULL)
            {
                rlog.err( "Failed to create bitmap from image: %s", frameName.c_str() );
            }
            else
            {
                int curPix = 0;
                for (int y = bf.m_Height; y > 0; y--)
                {
                    for (int x = 0; x < bf.m_Width; x++)
                    {
                        ColorF c = image.GetColor( x, y );
                        //  pre-multiply alpha
                        c.r *= c.a;
                        c.g *= c.a;
                        c.b *= c.a;
                        pPixels[curPix] = c;
                        curPix++;
                    }
                }
            }
            
            SelectObject( bf.m_hDC, bf.m_hBitmap );

            bp.m_Frames.push_back( bf );
        }
        curFrame++;
    }

    return m_Packages.size() - 1;
}

void DrawServerGDI::DrawSprite( float x, float y, int spriteID, int frameID, 
                                DWORD color, float rot, bool bMirror ) 
{

}

void DrawServerGDI::DrawSprite( const Frame& rct, int spriteID, int frameID, 
                                DWORD color, float rot, bool bMirror ) 
{
    if (spriteID < 0 || spriteID >= m_Packages.size())
    {
        return;
    }
    const BitmapPackage& bp = m_Packages[spriteID];
    if (frameID < 0 || frameID >= bp.m_Frames.size())
    {
        return;
    }
    const BitmapFrame& frame = bp.m_Frames[frameID];

    BLENDFUNCTION bf;
    bf.BlendOp              = AC_SRC_OVER;
    bf.BlendFlags           = 0;
    bf.SourceConstantAlpha  = 0xFF;  
    bf.AlphaFormat          = AC_SRC_ALPHA;             

    HRESULT hRes = AlphaBlend( m_hBackDC, rct.x, rct.y, rct.w, rct.h, 
                               frame.m_hDC, 0, 0, frame.m_Width, frame.m_Height, bf );
}
    

Frame DrawServerGDI::GetFrameBounds( int spriteID, int frameID ) 
{
    if (spriteID < 0 || spriteID >= m_Packages.size())
    {
        return Frame::null;
    }
    const BitmapPackage& bp = m_Packages[spriteID];
    if (frameID < 0 || frameID >= bp.m_Frames.size())
    {
        return Frame::null;
    }

    const BitmapFrame& frame = bp.m_Frames[frameID];
    return Frame( 0.0f, 0.0f, frame.m_Width, frame.m_Height );
}

int DrawServerGDI::GetFrameW( int spriteID, int frameID ) 
{
    if (spriteID < 0 || spriteID >= m_Packages.size())
    {
        return 0;
    }
    const BitmapPackage& bp = m_Packages[spriteID];
    if (frameID < 0 || frameID >= bp.m_Frames.size())
    {
        return 0;
    }

    return bp.m_Frames[frameID].m_Width;
}

int DrawServerGDI::GetFrameH( int spriteID, int frameID ) 
{
    if (spriteID < 0 || spriteID >= m_Packages.size())
    {
        return 0;
    }
    const BitmapPackage& bp = m_Packages[spriteID];
    if (frameID < 0 || frameID >= bp.m_Frames.size())
    {
        return 0;
    }

    return bp.m_Frames[frameID].m_Height;
}

void DrawServerGDI::Render() 
{
    BitBlt( m_hDC, 0, 0, m_BackW, m_BackH, m_hBackDC, 0, 0, SRCCOPY );
}

Vec2 DrawServerGDI::GetFramePivot( int spriteID, int frameID ) 
{
    return Vec2::null;
}

Vec2 DrawServerGDI::GetFrameShift( int spriteID, int frameID ) 
{
    return Vec2::null;
}

DWORD DrawServerGDI::GetPixel( int spriteID, int frameID, const Vec2& pt ) const 
{ 
    return 0xFFFFFFFF; 
}

int DrawServerGDI::GetNFrames( int spriteID ) const
{
    if (spriteID < 0 || spriteID >= m_Packages.size())
    {
        return 0;
    }
    return m_Packages[spriteID].m_Frames.size();
}
