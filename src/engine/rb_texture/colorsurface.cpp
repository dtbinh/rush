/***********************************************************************************/
//  File:   ColorSurface.cpp
//  Date:   17.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "Color.h"
#include "Frame.h"
#include "Vec2.h"
#include "Stream.h"
#include "ColorConversion.h"
#include "ColorSurface.h"

OutStream& operator <<( OutStream& os, const ColorSurface& surf )
{
    os << surf.m_Width << surf.m_Height;
    os.Write( (BYTE*)surf.m_Pixels, surf.GetNPixels()*sizeof( ColorF ) );
    return os;
} 

InStream& operator >>( InStream&  is, ColorSurface& surf )
{
    int w = 0, h = 0;
    is >> w >> h;
    surf.SetExt( w, h );
    is.Read( surf.m_Pixels, w*h*sizeof( ColorF ) );
    return is;
}

/***********************************************************************************/
/*  ColorSurface implementation
/***********************************************************************************/
ColorSurface::ColorSurface()
{
    m_Pixels = NULL;
    m_Width  = 0;
    m_Height = 0;
}

ColorSurface::~ColorSurface()
{
    delete []m_Pixels;
} // ColorSurface::~ColorSurface

void ColorSurface::operator =( const ColorSurface& surf )
{
    m_Width  = surf.m_Width;
    m_Height = surf.m_Height;

    delete []m_Pixels;

    if (surf.GetNPixels() == 0)
    {
        m_Pixels = NULL;
        return;
    }
    m_Pixels = new ColorF[GetNPixels()];
    memcpy( m_Pixels, surf.m_Pixels, GetNPixels()*sizeof( ColorF ) );
} // ColorSurface::operator =

bool ColorSurface::operator ==( const ColorSurface& surf ) const
{
    if (m_Width != surf.m_Width || m_Height != surf.m_Height) return false;
    int nPix = m_Width*m_Height;
    for (int i = 0; i < nPix; i++)
    {
        if (m_Pixels[i] != surf.m_Pixels[i])
        {
            return false;
        }
    }
    return true;
} // ColorSurface::operator ==

bool ColorSurface::ReadFromTexture( int texID, const Frame* frame, int mipLevel )
{
    if (texID == -1) return false;
    TextureProperties tp;
    if (!g_pRenderServer->GetTextureProp( texID, tp ))
    {
        return false;
    }
    
    Frame texExt( 0, 0, tp.m_Width, tp.m_Height );
    Frame rct = texExt;
    if (frame)
    {
        rct = *frame;
        rct.Clip( texExt );
    }
    
    int surfID = -1;
    if (tp.m_Format == ColorFormat_Unknown)
    {
        tp.m_Format = ColorFormat_ARGB8888;
        strcat( tp.m_Name, "_color_conversion_ARGB8888" );
        surfID = g_pRenderServer->CreateTexture( tp );
        g_pRenderServer->CopyTexture( surfID, texID );
        texID = surfID;
    }
    DWORD stride = 0;
    BYTE* pData = g_pRenderServer->LockTexture( texID, rct, mipLevel, &stride );
    if (!pData)
    {
        return false;
    }
    SetExt( int( rct.w ), int( rct.h ) );

    int nRows       = rct.h;
    int nPixInRow   = rct.w;
    BYTE* pDst = (BYTE*)m_Pixels;
    for (int j = 0; j < nRows; j++)
    {
        bool bRes = ConvertPixels( pDst, ColorFormat_ARGB32F, pData, tp.m_Format, nPixInRow );
        if (!bRes)
        {
            return false;
        }
        pDst += nPixInRow*sizeof( float )*4;
        pData += stride;
    }

    g_pRenderServer->UnlockTexture( texID, mipLevel );

    if (surfID != -1)
    {
        g_pRenderServer->DeleteTexture( surfID );
    }
    return true;
} // ColorSurface::ReadFromTexture

bool ColorSurface::WriteToTexture( int texID, const Vec2& pos, int mipLevel )
{

    return false;
} // ColorSurface::WriteToTexture

bool ColorSurface::Downsample()
{
    if (m_Width == 0 || m_Height == 0) return false; 
    int nw = m_Width/2;
    int nh = m_Height/2;
    ColorF* nPix = new ColorF[nw*nh];
    int cPix = 0;
    for (int j = 0; j < nh; j++)
    {
        for (int i = 0; i < nw; i++)
        {
            const int cI = i*2;
            const int cJ = j*2;
            ColorF clr = (GetColor( cI, cJ ) + GetColor( cI + 1, cJ ) + 
                          GetColor( cI, cJ + 1 ) + GetColor( cI + 1, cJ + 1 ))*0.25f;
            nPix[cPix] = clr;
            cPix++;
        }
    }

    m_Width = nw;
    m_Height = nh;
    delete []m_Pixels;
    m_Pixels = nPix;
    return true;
} // ColorSurface::Downsample

bool ColorSurface::ToGreyScale()
{
    int nP = GetNPixels();
    for (int i = 0; i < nP; i++) 
    {
        m_Pixels[i].toGreyScale();
    }
    return false;
} // ColorSurface::ToGreyScale

void ColorSurface::Clear( const ColorF& color )
{
    int nP = GetNPixels();
    for (int i = 0; i < nP; i++) m_Pixels[i] = color;
} // ColorSurface::Clear

void ColorSurface::SetExt( int w, int h )
{
    if (w*h == m_Width*m_Height)
    {
        m_Width  = w;
        m_Height = h;
        return;
    }
    delete []m_Pixels;
    m_Width  = w;
    m_Height = h;
    m_Pixels = new ColorF[w*h];
} // ColorSurface::SetExt

void ColorSurface::CopyRect( const ColorSurface& surf, const Frame& rect, const Vec2& to )
{

} // ColorSurface::CopyRect

