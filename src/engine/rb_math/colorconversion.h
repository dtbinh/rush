//****************************************************************************/
//  File:  ColorConversion.h
//  Desc:  
//****************************************************************************/
#ifndef __COLORCONVERSION_H__
#define __COLORCONVERSION_H__
#pragma once

#include "Color.h"

bool ConvertPixels( BYTE* pDst, ColorFormat dstFmt, 
                    const BYTE* pSrc, ColorFormat srcFmt, 
                    int nPixels );

template<ColorFormat CTo, ColorFormat CFrom>
__forceinline void ConvertPixel( BYTE* &dst, const BYTE* &src )
{
    assert( false );
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_ARGB4444>( BYTE* &dst, const BYTE* &src )
{
    DWORD a = DWORD(src[1]&0xF0);
    DWORD r = DWORD(src[1]&0x0F);
    DWORD g = DWORD(src[0]&0xF0);
    DWORD b = DWORD(src[0]&0x0F);
    DWORD clr = (   (a<<24) | (a<<20) |
                    (r<<20) | (r<<16) |
                    (g<<8)  | (g<<4)  |
                    (b<<4)  | b);
    *((DWORD*)dst) = clr;
    dst += 4;
    src += 2;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_RGB565>( BYTE* &dst, const BYTE* &src )
{
    DWORD r   = DWORD(src[1]&0xF8);
    r |= (r>>5);
    DWORD g   = (DWORD(src[1]&0x07)<<5) | 
                (DWORD(src[0]&0xE0)>>3);
    g |= (g>>6);
    DWORD b   = DWORD(src[0]&0x1F)<<3;
    g |= (g>>5);
    DWORD clr = (0xFF000000 | (r<<16) | (g<<8) | b);
    *((DWORD*)dst) = clr;
    dst += 4;
    src += 2;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_RGB888>( BYTE* &dst, const BYTE* &src )
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = 0xFF;
    dst += 4;
    src += 3;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_A8>( BYTE* &dst, const BYTE* &src )
{
    *((DWORD*)dst) = (((DWORD)(*src))<<24)|0x00FFFFFF;
    dst += 4;
    src += 1;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB8888, ColorFormat_ARGB32F>( BYTE* &dst, const BYTE* &src )
{
    const float* fc = (const float*)src;
    DWORD ba = (DWORD( fc[0]*255.0f )) << 24;
    DWORD br = (DWORD( fc[1]*255.0f )) << 16;
    DWORD bg = (DWORD( fc[2]*255.0f )) << 8;
    DWORD bb = (DWORD( fc[3]*255.0f ));
    *((DWORD*)dst) = ba|br|bg|bb;
    dst += 4;
    src += 16;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB32F, ColorFormat_ARGB8888>( BYTE* &dst, const BYTE* &src )
{
    float* fdst = (float*)dst;
    DWORD clr = *((const DWORD*)src);
    DWORD ba = (clr&0xFF000000)>>24;
    DWORD br = (clr&0x00FF0000)>>16;
    DWORD bg = (clr&0x0000FF00)>>8;
    DWORD bb = (clr&0x000000FF);
    const float c_InvByte = 1.0f/255.0f;
    fdst[0] = float( ba )*c_InvByte;
    fdst[1] = float( br )*c_InvByte;
    fdst[2] = float( bg )*c_InvByte;
    fdst[3] = float( bb )*c_InvByte;
    dst += 16;
    src += 4;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB32F, ColorFormat_RGB565>( BYTE* &dst, const BYTE* &src )
{
    float* fdst = (float*)dst;
    DWORD r   = DWORD(src[1]&0xF8);
    r |= (r>>5);
    DWORD g   = (DWORD(src[1]&0x07)<<5) | 
                (DWORD(src[0]&0xE0)>>3);
    g |= (g>>6);
    DWORD b   = DWORD(src[0]&0x1F)<<3;
    g |= (g>>5);

    const float c_InvByte = 1.0f/255.0f;
    fdst[0] = 1.0f;
    fdst[1] = float( r )*c_InvByte;
    fdst[2] = float( g )*c_InvByte;
    fdst[3] = float( b )*c_InvByte;
    dst += 16;
    src += 2;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB4444, ColorFormat_RGB565>( BYTE* &dst, const BYTE* &src )
{
    WORD br = WORD(src[1]&0xF8);
    WORD bg = (WORD(src[1]&0x07)<<5) | (WORD(src[0]&0xE0)>>3);
    WORD bb = WORD(src[0]&0x1F)<<3;
    *((WORD*)dst) = 0xF0|br|bg|bb;
    dst += 2;
    src += 2;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB4444, ColorFormat_ARGB8888>( BYTE* &dst, const BYTE* &src )
{
    WORD ba = (WORD(src[3]&0xF0))<<8;
    WORD br = (WORD(src[2]&0xF0))<<4;
    WORD bg = (WORD(src[1]&0xF0));
    WORD bb = (WORD(src[0]&0xF0))>>4;
    *((WORD*)dst) = ba|br|bg|bb;
    dst += 2;
    src += 4;
}

template<>
__forceinline void ConvertPixel<ColorFormat_ARGB4444, ColorFormat_ARGB32F>( BYTE* &dst, const BYTE* &src )
{
    const float* fc = (const float*)src;
    WORD ba = (WORD( fc[0]*15.0f )) << 12;
    WORD br = (WORD( fc[1]*15.0f )) << 8;
    WORD bg = (WORD( fc[2]*15.0f )) << 4;
    WORD bb = (WORD( fc[3]*15.0f ));
    *((WORD*)dst) = ba|br|bg|bb;
    dst += 2;
    src += 16;
}

template<>
__forceinline void ConvertPixel<ColorFormat_RGB565, ColorFormat_ARGB8888>( BYTE* &dst, const BYTE* &src )
{
    WORD br = (WORD(src[2]&0xF8))<<8;
    WORD bg = (WORD(src[1]&0xFC))<<3;
    WORD bb = (WORD(src[0]&0xF8))>>3;
    *((WORD*)dst) = br|bg|bb;
    dst += 2;
    src += 4;
}

template<>
__forceinline void ConvertPixel<ColorFormat_RGB565, ColorFormat_ARGB32F>( BYTE* &dst, const BYTE* &src )
{
    const float* fc = (const float*)src;
    WORD br = (WORD( fc[1]*31.0f )) << 11;
    WORD bg = (WORD( fc[2]*63.0f )) << 5;
    WORD bb = (WORD( fc[3]*31.0f ));

    *((WORD*)dst) = br|bg|bb;
    dst += 2;
    src += 4;
}


template<ColorFormat CTo, ColorFormat CFrom>
__forceinline void ConvertPixels( BYTE* &dst, const BYTE* &src, int nPixels )
{
    for (int i = 0; i < nPixels; i++)
    {
        ConvertPixel<CTo, CFrom>( dst, src );
    }
} // ConvertPixels

#endif // __COLORCONVERSION_H__

