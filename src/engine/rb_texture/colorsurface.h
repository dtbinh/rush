/***********************************************************************************/
//  File:   ColorSurface.h
//  Date:   17.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLORSURFACE_H__
#define __COLORSURFACE_H__

#include "Color.h"
#include "Vec2.h"

class Frame;
class OutStream;
class InStream;

/***********************************************************************************/
//  Class:  ColorSurface
//  Desc:   
/***********************************************************************************/
class ColorSurface
{
    ColorF*     m_Pixels;
    int         m_Width;
    int         m_Height;

public:
                    ColorSurface    ();
                    ~ColorSurface   ();
                    ColorSurface    ( const ColorSurface& surf ) { *this = surf; }

    int             GetW            () const { return m_Width; }
    int             GetH            () const { return m_Height; }
    int             GetNPixels      () const { return m_Width*m_Height; }
    ColorF*         GetPixels       () const { return m_Pixels; }

    bool            ReadFromTexture ( int texID, const Frame* frame = NULL, int mipLevel = 0 );
    bool            WriteToTexture  ( int texID, const Vec2& pos = Vec2::null, int mipLevel = 0 );

    bool            Downsample      ();
    bool            ToGreyScale     ();
    void            Clear           ( const ColorF& color = ColorF::black );
    void            SetExt          ( int w, int h );
    void            CopyRect        ( const ColorSurface& surf, const Frame& rect, const Vec2& to = Vec2::null );

    void            operator =      ( const ColorSurface& surf );
    bool            operator ==     ( const ColorSurface& surf ) const;

    inline void     SetColor        ( int x, int y, const ColorF& col );
    inline void     SetAlpha        ( int x, int y, float alpha );
    inline ColorF&  GetColor        ( int xIdx, int yIdx ) const;
    inline ColorF&  operator ()     ( int xIdx, int yIdx ) const;


    friend OutStream&  operator <<( OutStream& os, const ColorSurface& surf );
    friend InStream&   operator >>( InStream&  is, ColorSurface& surf );
}; // class ColorSurface

inline void ColorSurface::SetColor( int x, int y, const ColorF& col )
{
    if (!m_Pixels || x < 0 || y < 0 || x >= m_Width || y >= m_Height)
    {
        return;
    }
    m_Pixels[x + y*m_Width] = col;
}

inline void ColorSurface::SetAlpha( int x, int y, float alpha )
{
    if (!m_Pixels || x < 0 || y < 0 || x >= m_Width || y >= m_Height)
    {
        return;
    }
    m_Pixels[x + y*m_Width].a = alpha;
}

inline ColorF& ColorSurface::GetColor( int xIdx, int yIdx ) const
{
    if (!m_Pixels) return ColorF::black;
    xIdx = clamp( xIdx, 0, m_Width  - 1 );
    yIdx = clamp( yIdx, 0, m_Height - 1 );
    return m_Pixels[xIdx + yIdx*m_Width];
}

inline ColorF&  ColorSurface::operator ()( int xIdx, int yIdx ) const
{
    return GetColor( xIdx, yIdx );
}

#endif // __COLORSURFACE_H__