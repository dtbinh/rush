//****************************************************************************/
//  File:  ColorConversion.cpp
//  Desc:   
//****************************************************************************/
#include "stdafx.h"
#include "ColorConversion.h"

bool ConvertPixels( BYTE* pDst, ColorFormat dstFmt, 
                    const BYTE* pSrc, ColorFormat srcFmt, 
                    int nPixels )
{
    switch (dstFmt)
    {
    case ColorFormat_ARGB8888:
        {
            switch (srcFmt)
            {
            case ColorFormat_ARGB4444: 
                ConvertPixels<ColorFormat_ARGB8888, ColorFormat_ARGB4444>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_RGB565: 
                ConvertPixels<ColorFormat_ARGB8888, ColorFormat_RGB565>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_RGB888: 
                ConvertPixels<ColorFormat_ARGB8888, ColorFormat_RGB888>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_A8: 
                ConvertPixels<ColorFormat_ARGB8888, ColorFormat_A8>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB8888: 
                memcpy( pDst, pSrc, nPixels*4 );
                return true;
            case ColorFormat_ARGB32F: 
                ConvertPixels<ColorFormat_ARGB8888, ColorFormat_ARGB32F>( pDst, pSrc, nPixels );
                return true;
            default: return false;
            }
        }break;
    case ColorFormat_ARGB32F:
        {
            switch (srcFmt)
            {
            case ColorFormat_RGB565: 
                ConvertPixels<ColorFormat_ARGB32F, ColorFormat_RGB565>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_RGB888: 
                ConvertPixels<ColorFormat_ARGB32F, ColorFormat_RGB888>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB8888: 
                ConvertPixels<ColorFormat_ARGB32F, ColorFormat_ARGB8888>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB32F: 
                memcpy( pDst, pSrc, nPixels*16 );
                return true;
            default: return false;
            }
        }break;
    case ColorFormat_ARGB4444:
        {
            switch (srcFmt)
            {
            case ColorFormat_RGB565: 
                ConvertPixels<ColorFormat_ARGB4444, ColorFormat_RGB565>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB8888: 
                ConvertPixels<ColorFormat_ARGB4444, ColorFormat_ARGB8888>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB32F: 
                ConvertPixels<ColorFormat_ARGB4444, ColorFormat_ARGB32F>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB4444: 
                memcpy( pDst, pSrc, nPixels*2 );
                return true;
            default: return false;
            }
        }break;
    case ColorFormat_RGB565:
        {
            switch (srcFmt)
            {
            case ColorFormat_RGB565: 
                memcpy( pDst, pSrc, nPixels*2 );
                return true;
            case ColorFormat_RGB888: 
                ConvertPixels<ColorFormat_RGB565, ColorFormat_RGB888>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB8888: 
                ConvertPixels<ColorFormat_RGB565, ColorFormat_ARGB8888>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB32F: 
                ConvertPixels<ColorFormat_RGB565, ColorFormat_ARGB32F>( pDst, pSrc, nPixels );
                return true;
            default: return false;
            }
        }break;
    case ColorFormat_RGB888:
        {
            switch (srcFmt)
            {
            case ColorFormat_RGB565: 
                ConvertPixels<ColorFormat_RGB888, ColorFormat_RGB565>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_ARGB8888: 
                ConvertPixels<ColorFormat_RGB888, ColorFormat_ARGB8888>( pDst, pSrc, nPixels );
                return true;
            case ColorFormat_RGB888: 
                memcpy( pDst, pSrc, nPixels*3 );
                return true;
            case ColorFormat_ARGB32F: 
                ConvertPixels<ColorFormat_RGB888, ColorFormat_ARGB32F>( pDst, pSrc, nPixels );
                return true;
            default: return false;
            }
        }break;
    }

    return false;
} // ConvertPixels

