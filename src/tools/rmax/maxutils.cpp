/***********************************************************************************/
//  File:   maxutils.cpp
//  Desc:   Max exporting related utilities
//  Date:   26.11.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "rmax.h"
#include "maxutils.h"

Mat4 Convert( const Matrix3& m )
{
    Mat4 tm;
    tm.e00 = m.GetRow(0).x; 
    tm.e01 = m.GetRow(0).y; 
    tm.e02 = m.GetRow(0).z;
    tm.e03 = 0.0f;

    tm.e10 = m.GetRow(1).x; 
    tm.e11 = m.GetRow(1).y; 
    tm.e12 = m.GetRow(1).z;
    tm.e13 = 0.0f;

    tm.e20 = m.GetRow(2).x; 
    tm.e21 = m.GetRow(2).y; 
    tm.e22 = m.GetRow(2).z;
    tm.e23 = 0.0f;

    tm.e30 = m.GetRow(3).x; 
    tm.e31 = m.GetRow(3).y; 
    tm.e32 = m.GetRow(3).z;
    tm.e33 = 1.0f;

    return tm;
} // Convert

DWORD ColorToDWORD( const Color& color )
{
    DWORD ba = 0x000000FF;
    DWORD br = (DWORD)(color.r * 255.0f);
    DWORD bg = (DWORD)(color.g * 255.0f);
    DWORD bb = (DWORD)(color.b * 255.0f);

    return (((ba & 0xFF) << 24) | 
            ((br & 0xFF) << 16) |
            ((bg & 0xFF) << 8)  |
             (bb & 0xFF));
} // ColorToDWORD

float TimeToMs( TimeValue t )
{
	return 1000.0f*float( t )/float( TIME_TICKSPERSEC );
} // TimeToMs

TimeValue MsToTime( float ms )
{
	return TimeValue( float( TIME_TICKSPERSEC )*ms/1000.0f );
} // MsToTime