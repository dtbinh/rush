/***********************************************************************************/
//  File:   LoaderTGA.h
//  Date:   17.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __LOADERTGA_H__
#define __LOADERTGA_H__

class InStream;
class ColorSurface;

bool LoadTGA( InStream& is, ColorSurface& image );

#endif // __LOADERTGA_H__