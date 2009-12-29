//****************************************************************************/
//  File:	WangTiling.cpp
//  Desc:	Implementation of class WangTiling
/*	Author:	Ruslan Shestopalyuk
/*	Date:	16-05-2006
//****************************************************************************/
#include "stdafx.h"
#include "WangTiling.h"

enum WangColor
{
    wcNone    = 0,
    wcRed     = 1,
    wcGreen   = 2,
    wcBlue    = 3,
    wcYellow  = 4,
}; // enum WangColor

struct WangTileSetup
{
    WangColor   left;
    WangColor   top;
    WangColor   right;
    WangColor   bottom;
}; // struct WangTileSetup

const WangTileSetup c_TileSetup[] = 
{
    { wcNone,   wcNone,  wcNone,   wcNone  },
    { wcBlue,   wcRed,   wcYellow, wcGreen },
    { wcYellow, wcRed,   wcYellow, wcRed   },
    { wcYellow, wcGreen, wcYellow, wcGreen },
    { wcYellow, wcGreen, wcBlue,   wcRed   },
    { wcBlue,   wcGreen, wcBlue,   wcGreen },
    { wcBlue,   wcRed,   wcBlue,   wcRed   },
    { wcBlue,   wcGreen, wcYellow, wcRed   },
    { wcYellow, wcRed,   wcBlue,   wcGreen },
};

//****************************************************************************/
/*  WangTiling implementation
//****************************************************************************/
int WangTiling::GetTile( int leftN, int topN, int rightN, int bottomN )
{
    //  get allowed side colors 
    WangColor vL = c_TileSetup[leftN].right;
    WangColor vT = c_TileSetup[topN].bottom;
    WangColor vR = c_TileSetup[rightN].left;
    WangColor vB = c_TileSetup[bottomN].top;

    //  find valid tiles in the set
    bool bValid[c_TileSetSize + 1];
    int nValid = 0;
    for (int i = 1; i <= c_TileSetSize; i++)
    {
        const WangTileSetup& tile = c_TileSetup[i];
        if (vL != wcNone && vL != tile.left  ) { bValid[i] = false; continue; }
        if (vT != wcNone && vT != tile.top   ) { bValid[i] = false; continue; }
        if (vR != wcNone && vR != tile.right ) { bValid[i] = false; continue; }
        if (vB != wcNone && vB != tile.bottom) { bValid[i] = false; continue; }
        bValid[i] = true;
        nValid++;
    }
    
    if (nValid == 0) return 0;

    //  pick one random valid tile 
    int cTile = m_Random.GetValue( nValid );
    for (int i = 1; i <= c_TileSetSize; i++) 
    {
        if (bValid[i]) cTile--;
        if (cTile == -1) return i;
    }
    return 0;
} // WangTiling::GetTile