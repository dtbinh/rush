//****************************************************************************/
//  File:  ColorConversion.h
//  Desc:
//****************************************************************************/
#ifndef __COLORCONVERSION_H__
#define __COLORCONVERSION_H__


#include "color.h"

bool ConvertPixels( uint8_t* pDst, ColorFormat dstFmt,
                    const uint8_t* pSrc, ColorFormat srcFmt,
                    int nPixels );

template<ColorFormat CTo, ColorFormat CFrom>
inline void ConvertPixel( uint8_t* &dst, const uint8_t* &src )
{
    assert( false );
}

template<>
inline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_ARGB4444>( uint8_t* &dst, const uint8_t* &src )
{
    uint32_t a = uint32_t(src[1]&0xF0);
    uint32_t r = uint32_t(src[1]&0x0F);
    uint32_t g = uint32_t(src[0]&0xF0);
    uint32_t b = uint32_t(src[0]&0x0F);
    uint32_t clr = (   (a<<24) | (a<<20) |
                    (r<<20) | (r<<16) |
                    (g<<8)  | (g<<4)  |
                    (b<<4)  | b);
    *((uint32_t*)dst) = clr;
    dst += 4;
    src += 2;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_RGB565>( uint8_t* &dst, const uint8_t* &src )
{
    uint32_t r   = uint32_t(src[1]&0xF8);
    r |= (r>>5);
    uint32_t g   = (uint32_t(src[1]&0x07)<<5) |
                (uint32_t(src[0]&0xE0)>>3);
    g |= (g>>6);
    uint32_t b   = uint32_t(src[0]&0x1F)<<3;
    g |= (g>>5);
    uint32_t clr = (0xFF000000 | (r<<16) | (g<<8) | b);
    *((uint32_t*)dst) = clr;
    dst += 4;
    src += 2;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_RGB888>( uint8_t* &dst, const uint8_t* &src )
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = 0xFF;
    dst += 4;
    src += 3;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_A8>( uint8_t* &dst, const uint8_t* &src )
{
    *((uint32_t*)dst) = (((uint32_t)(*src))<<24)|0x00FFFFFF;
    dst += 4;
    src += 1;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_ARGB32F>( uint8_t* &dst, const uint8_t* &src )
{
    const float* fc = (const float*)src;
    uint32_t ba = (uint32_t( fc[0]*255.0f )) << 24;
    uint32_t br = (uint32_t( fc[1]*255.0f )) << 16;
    uint32_t bg = (uint32_t( fc[2]*255.0f )) << 8;
    uint32_t bb = (uint32_t( fc[3]*255.0f ));
    *((uint32_t*)dst) = ba|br|bg|bb;
    dst += 4;
    src += 16;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB32F, ColorFormat_ARGB8888>( uint8_t* &dst, const uint8_t* &src )
{
    float* fdst = (float*)dst;
    uint32_t clr = *((const uint32_t*)src);
    uint32_t ba = (clr&0xFF000000)>>24;
    uint32_t br = (clr&0x00FF0000)>>16;
    uint32_t bg = (clr&0x0000FF00)>>8;
    uint32_t bb = (clr&0x000000FF);
    const float c_Invuint8_t = 1.0f/255.0f;
    fdst[0] = float( ba )*c_Invuint8_t;
    fdst[1] = float( br )*c_Invuint8_t;
    fdst[2] = float( bg )*c_Invuint8_t;
    fdst[3] = float( bb )*c_Invuint8_t;
    dst += 16;
    src += 4;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB32F, ColorFormat_RGB565>( uint8_t* &dst, const uint8_t* &src )
{
    float* fdst = (float*)dst;
    uint32_t r   = uint32_t(src[1]&0xF8);
    r |= (r>>5);
    uint32_t g   = (uint32_t(src[1]&0x07)<<5) |
                (uint32_t(src[0]&0xE0)>>3);
    g |= (g>>6);
    uint32_t b   = uint32_t(src[0]&0x1F)<<3;
    g |= (g>>5);

    const float c_Invuint8_t = 1.0f/255.0f;
    fdst[0] = 1.0f;
    fdst[1] = float( r )*c_Invuint8_t;
    fdst[2] = float( g )*c_Invuint8_t;
    fdst[3] = float( b )*c_Invuint8_t;
    dst += 16;
    src += 2;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB4444, ColorFormat_RGB565>( uint8_t* &dst, const uint8_t* &src )
{
    uint16_t br = uint16_t(src[1]&0xF8);
    uint16_t bg = (uint16_t(src[1]&0x07)<<5) | (uint16_t(src[0]&0xE0)>>3);
    uint16_t bb = uint16_t(src[0]&0x1F)<<3;
    *((uint16_t*)dst) = 0xF0|br|bg|bb;
    dst += 2;
    src += 2;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB4444, ColorFormat_ARGB8888>( uint8_t* &dst, const uint8_t* &src )
{
    uint16_t ba = (uint16_t(src[3]&0xF0))<<8;
    uint16_t br = (uint16_t(src[2]&0xF0))<<4;
    uint16_t bg = (uint16_t(src[1]&0xF0));
    uint16_t bb = (uint16_t(src[0]&0xF0))>>4;
    *((uint16_t*)dst) = ba|br|bg|bb;
    dst += 2;
    src += 4;
}

template<>
inline void ConvertPixel<ColorFormat_ARGB4444, ColorFormat_ARGB32F>( uint8_t* &dst, const uint8_t* &src )
{
    const float* fc = (const float*)src;
    uint16_t ba = (uint16_t( fc[0]*15.0f )) << 12;
    uint16_t br = (uint16_t( fc[1]*15.0f )) << 8;
    uint16_t bg = (uint16_t( fc[2]*15.0f )) << 4;
    uint16_t bb = (uint16_t( fc[3]*15.0f ));
    *((uint16_t*)dst) = ba|br|bg|bb;
    dst += 2;
    src += 16;
}

template<>
inline void ConvertPixel<ColorFormat_RGB565, ColorFormat_ARGB8888>( uint8_t* &dst, const uint8_t* &src )
{
    uint16_t br = (uint16_t(src[2]&0xF8))<<8;
    uint16_t bg = (uint16_t(src[1]&0xFC))<<3;
    uint16_t bb = (uint16_t(src[0]&0xF8))>>3;
    *((uint16_t*)dst) = br|bg|bb;
    dst += 2;
    src += 4;
}

template<>
inline void ConvertPixel<ColorFormat_RGB565, ColorFormat_ARGB32F>( uint8_t* &dst, const uint8_t* &src )
{
    const float* fc = (const float*)src;
    uint16_t br = (uint16_t( fc[1]*31.0f )) << 11;
    uint16_t bg = (uint16_t( fc[2]*63.0f )) << 5;
    uint16_t bb = (uint16_t( fc[3]*31.0f ));

    *((uint16_t*)dst) = br|bg|bb;
    dst += 2;
    src += 4;
}


template<ColorFormat CTo, ColorFormat CFrom>
inline void ConvertPixels( uint8_t* &dst, const uint8_t* &src, int nPixels )
{
    for (int i = 0; i < nPixels; i++)
    {
        ConvertPixel<CTo, CFrom>( dst, src );
    }
} // ConvertPixels

#endif // __COLORCONVERSION_H__

