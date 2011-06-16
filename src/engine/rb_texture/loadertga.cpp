/***********************************************************************************/
//  File:   LoaderTGA.cpp
//  Date:   17.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "Stream.h"
#include "Buffer.h"
#include "ColorSurface.h"
#include "LoaderTGA.h"

bool LoadTGA( InStream& is, ColorSurface& image )
{
    BYTE header[18];
    is.Read( header, 18 );

    if (header[1] != 0 || 
        (header[2] != 2 && header[2] != 3)) 
    {
        return false;
    }

    int width   = header[0 + 12] + header[1 + 12]*256; 
    int height  = header[2 + 12] + header[3 + 12]*256;
    int bitspp  = header[4 + 12]; 
    
    image.SetExt( width, height );

    int bytespp = 0;
    if (bitspp == 32) 
    {
        bytespp = 4;
    }
    else if (bitspp == 24) 
    {
        bytespp = 3;
    }
    else 
    {
        return false;
    }

    int nPix = width*height;
    int nBytes = nPix*bytespp;
    Buffer buf( nBytes );
    if (!is.Read( buf.GetData(), nBytes ))
    {
        return false;
    }
    const BYTE* pPix = buf.GetData();
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            BYTE alpha = 255;
            if (bytespp == 4) 
            {
                alpha = pPix[3];
            }
            image.SetColor( i, j, ColorF( alpha, pPix[2], pPix[1], pPix[0] ) );
            pPix += bytespp;
        }
    }
    return true;
}
