//****************************************************************************/
//  File:  BitOps.h
//  Desc:  
//****************************************************************************/
#ifndef __BITOPS_H__
#define __BITOPS_H__
#pragma once

inline DWORD ReverseBits( DWORD n )
{
    n = n - ((n >> 1) & 0x55555555);
	n = ((n >> 2) & 0x33333333) + (n & 0x33333333);
	n = ((n >> 4) + n) & 0x0F0F0F0F;
	return (n * 0x01010101) >> 24;
} // ReverseBits

inline DWORD CountBits( DWORD n )
{
    n = (n & 0x55555555) + ((n & 0xaaaaaaaa) >> 1);
    n = (n & 0x33333333) + ((n & 0xcccccccc) >> 2);
    n = (n & 0x0f0f0f0f) + ((n & 0xf0f0f0f0) >> 4);
    n = (n & 0x00ff00ff) + ((n & 0xff00ff00) >> 8);
    n = (n & 0x0000ffff) + ((n & 0xffff0000) >> 16);
    return n;
} // CountBits

inline DWORD NextPow2( DWORD n )
{
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    return n + 1;
} // NextPow2

inline bool IsPowerOfTwo( DWORD n )
{ 
    return ((n&(n - 1)) == 0);					
} // IsPowerOfTwo

inline int GetPow2( DWORD p )
{
    int res = 0;
    int np = (int)p;
    while (np > 1) { np >>= 1; res++; }
    return res;
} // GetPower

#endif // __BITOPS_H__

