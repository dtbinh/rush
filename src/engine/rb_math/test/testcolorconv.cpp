#include "stdafx.h"
#include "Color.h"
#include "ColorConversion.h"

DWORD c_TestColors[] = { 0xFF00FF00, 0x80FFFF00, 0x00FFFFFF, 0x00000000, 0xFF00FF00, 0xFF0000FF, 0xFFFF00FF, 
                         0xFFFFFFFF, 0x80FFFF80, 0x80FFFFFF, 0x80000000, 0x8000FF00, 0xFF0080FF, 0x008080FF };

ColorFormat c_FormatPairs[][2] = {  
    {ColorFormat_ARGB8888, ColorFormat_RGB565}, 
    {ColorFormat_ARGB32F,  ColorFormat_RGB565},
/*
    {ColorFormat_ARGB8888, ColorFormat_RGB888},
    {ColorFormat_ARGB32F,  ColorFormat_RGB888},

    {ColorFormat_RGB888,   ColorFormat_RGB565},
    {ColorFormat_RGB888,   ColorFormat_ARGB8888},
    {ColorFormat_RGB888,   ColorFormat_RGB888},
    {ColorFormat_RGB888,   ColorFormat_ARGB32F},

    {ColorFormat_RGB565,   ColorFormat_RGB565},
    {ColorFormat_RGB565,   ColorFormat_ARGB8888},
    {ColorFormat_RGB565,   ColorFormat_RGB888},
    {ColorFormat_RGB565,   ColorFormat_ARGB32F},

    {ColorFormat_ARGB4444, ColorFormat_RGB565}*/

};

ColorFormat c_FormatPairsA[][2] = { 
    {ColorFormat_ARGB8888, ColorFormat_ARGB4444}, 
    {ColorFormat_ARGB8888, ColorFormat_ARGB8888},
    {ColorFormat_ARGB8888, ColorFormat_ARGB32F}, 
/*
    {ColorFormat_ARGB4444, ColorFormat_ARGB4444}, 
    {ColorFormat_ARGB4444, ColorFormat_ARGB8888},
    {ColorFormat_ARGB4444, ColorFormat_ARGB32F}, 

    {ColorFormat_ARGB32F,  ColorFormat_ARGB8888},
    {ColorFormat_ARGB32F,  ColorFormat_ARGB32F}*/
};


TEST( ColorBufferConversion )
{
    const int c_NColors = sizeof( c_TestColors )/sizeof( DWORD );

    const int c_BufSize = c_NColors*16;
    BYTE srcBuf[c_BufSize];
    BYTE dstBuf[c_BufSize];
    DWORD checkBuf[c_NColors];

    int nPairs = sizeof( c_FormatPairs )/sizeof(ColorFormat)/2;
    for (int i = 0; i < nPairs; i++)
    {
        ColorFormat dstFmt = c_FormatPairs[i][0];
        ColorFormat srcFmt = c_FormatPairs[i][1];
        ConvertPixels( srcBuf, srcFmt, (const BYTE*)c_TestColors, ColorFormat_ARGB8888, c_NColors );
        ConvertPixels( dstBuf, dstFmt, srcBuf, srcFmt, c_NColors );
        ConvertPixels( (BYTE*)checkBuf, ColorFormat_ARGB8888, dstBuf, dstFmt, c_NColors );
        for (int j = 0; j < c_NColors; j++)
        {
            checkBuf[j] &= 0x00FFFFFF;
            checkBuf[j] |= c_TestColors[j]&0xFF000000;
            ColorF clrOrg( c_TestColors[j] );
            ColorF clrDst( checkBuf[j] );
            CHECK( clrOrg.isEqual( clrDst, 0.06f ) );
        }
    }
    
    int nPairsA = sizeof( c_FormatPairsA )/sizeof(ColorFormat)/2;
    for (int i = 0; i < nPairsA; i++)
    {
        ColorFormat dstFmt = c_FormatPairsA[i][0];
        ColorFormat srcFmt = c_FormatPairsA[i][1];
        ConvertPixels( srcBuf, srcFmt, (const BYTE*)c_TestColors, ColorFormat_ARGB8888, c_NColors );
        ConvertPixels( dstBuf, dstFmt, srcBuf, srcFmt, c_NColors );
        ConvertPixels( (BYTE*)checkBuf, ColorFormat_ARGB8888, dstBuf, dstFmt, c_NColors );
        for (int j = 0; j < c_NColors; j++)
        {
            ColorF clrOrg( c_TestColors[j] );
            ColorF clrDst( checkBuf[j] );
            CHECK( clrOrg.isEqual( clrDst, 0.06f ) );
        }
    }
}

