//****************************************************************************/
//  File:   WangTiling.h
//  Desc:
/*  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __WANGTILING_H__
#define __WANGTILING_H__

#include "rand.h"

const int c_TileSetSize = 8;
//****************************************************************************/
//  Class:  WangTiling
//  Desc:   Creates non-periodic stochastic tiling of the plane with the set
/*            of 8 square tiles, which are not rotated (so-called Wang tiles).
/*          Each tile has "colouring" of the sides, and same-colour sides
/*          of the neighboring tiles must match seamlessly.
/*          It is supposed, that tiles are coloured like this:
/*                r           r           g           g
/*             |-----|     |-----|     |-----|     |-----|
/*            b|  1  |y   y|  2  |y   y|  3  |y   y|  4  |b
/*             |-----|     |-----|     |-----|     |-----|
/*                g           r           g           r
/*
/*                g           r           g           r
/*             |-----|     |-----|     |-----|     |-----|
/*            b|  5  |b   b|  6  |b   b|  7  |y   y|  8  |b
/*             |-----|     |-----|     |-----|     |-----|
/*                g           r           r           g
/*
/*
//****************************************************************************/
class WangTiling
{
    RandomGen   m_Random;
public:

    WangTiling(){}

    void        Reset  ( uint32_t seed ) { m_Random.Reset( seed ); }
    int         GetTile( int leftN, int topN, int rightN, int bottomN );

protected:


}; // class WangTiling

#endif // __WANGTILING_H__
