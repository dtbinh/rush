
//  File:   JSpriteProbe.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "IDrawServer.h"
#include "JSpriteProbe.h"

/***********************************************************************************/
/*  JSpriteProbe implementation
/***********************************************************************************/
decl_class(JSpriteProbe);
JSpriteProbe::JSpriteProbe()
{
    m_SkinPack = "";
    m_SkinFrame = -1;
    m_Position = Vec2::null;
} 

float JSpriteProbe::GetAlpha() const
{
    ColorF color( GetColor() );
    return color.a;
}

DWORD JSpriteProbe::GetColor() const
{
    int spID = g_pDrawServer->GetSpriteID( m_SkinPack.c_str() );
    return g_pDrawServer->GetPixel( spID, m_SkinFrame, m_Position );
}