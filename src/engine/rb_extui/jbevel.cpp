//****************************************************************************/
//  File:   JBevel.cpp
//  Date:   24.10.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "JBevel.h"

//****************************************************************************/
/*  JBevel implementation
//****************************************************************************/
decl_class(JBevel);
JBevel::JBevel()
{
    SetSkinPack( "sys_bevel" );

    m_Center        = 0;

    m_LeftTop       = 1;
    m_RightTop      = 2;
    m_LeftBottom    = 3;
    m_RightBottom   = 4;
    m_Left          = 5;
    m_Top           = 6;
    m_Right         = 7;
    m_Bottom        = 8;
} // JBevel::JBevel

void JBevel::Init()
{
    ResInit();
}

void JBevel::Render()
{    
    int spID = GetSkinPackID();
    if (spID == 0)
    {
        return;
    }

    g_pDrawServer->Flush();

    Frame ext = GetExt();
    DWORD color = GetFgColor();
    
    Frame cExt  = g_pDrawServer->GetFrameBounds( spID, m_Center      );
    Frame ltExt = g_pDrawServer->GetFrameBounds( spID, m_LeftTop     );
    Frame rtExt = g_pDrawServer->GetFrameBounds( spID, m_RightTop    );
    Frame lbExt = g_pDrawServer->GetFrameBounds( spID, m_LeftBottom  );
    Frame rbExt = g_pDrawServer->GetFrameBounds( spID, m_RightBottom );
    Frame lExt  = g_pDrawServer->GetFrameBounds( spID, m_Left        );
    Frame tExt  = g_pDrawServer->GetFrameBounds( spID, m_Top         );
    Frame rExt  = g_pDrawServer->GetFrameBounds( spID, m_Right       );
    Frame bExt  = g_pDrawServer->GetFrameBounds( spID, m_Bottom      );
    
    //  draw center
    Frame rct = ext;
    rct.x += lExt.w;
    rct.y += tExt.h;
    rct.w -= lExt.w + rExt.w;
    rct.h -= tExt.h + bExt.h;
    if (rct.h > 0.0f && rct.w > 0.0f)
    {
        g_pDrawServer->TileSprite( rct, spID, m_Center, color );
    }

    //  draw bottom
    rct = ext;
    rct.y = rct.b() - bExt.h;
    rct.x += lbExt.w;
    rct.w -= lbExt.w + rbExt.w;
    rct.h = bExt.h;
    g_pDrawServer->TileSprite( rct, spID, m_Bottom, color );

    //  draw top
    rct = ext;
    rct.x += ltExt.w;
    rct.w -= ltExt.w + rtExt.w;
    rct.h = tExt.h;
    g_pDrawServer->TileSprite( rct, spID, m_Top, color );

    //  draw left
    rct = ext;
    rct.y += ltExt.h;
    rct.w = lExt.w;
    rct.h -= lbExt.h + ltExt.h;
    g_pDrawServer->TileSprite( rct, spID, m_Left, color );

    //  draw right
    rct = ext;
    rct.x = rct.r() - rExt.w;
    rct.y += rtExt.h;
    rct.w = rExt.w;
    rct.h -= rbExt.h + rtExt.h;
    g_pDrawServer->TileSprite( rct, spID, m_Right, color );
    
    //  draw corners
    g_pDrawServer->DrawSprite( ext.x, ext.y, spID, m_LeftTop, color );
    g_pDrawServer->DrawSprite( ext.r() - rtExt.w, ext.y, spID, m_RightTop, color );
    g_pDrawServer->DrawSprite( ext.x, ext.b() - lbExt.h, spID, m_LeftBottom, color );
    g_pDrawServer->DrawSprite( ext.r() - rbExt.w, ext.b() - rbExt.h, spID, m_RightBottom, color );
    
    g_pDrawServer->Flush();

} // JBevel::Render
