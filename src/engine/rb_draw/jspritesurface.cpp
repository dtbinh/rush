//****************************************************************************/
//  File:  JSpriteSurface.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JSpriteSurface.h"

//****************************************************************************/
/*	JSpriteSurface implementation
//****************************************************************************/
DWORD JSpriteSurface::s_Stamp = 1;
JSpriteSurface::JSpriteSurface() : m_TexID(-1)
{
    m_Layout.Init( c_SpriteSurfaceSide );
    m_DropStamp     = 0;
    m_LastFrameUsed = 0;
}

JSpriteSurface::~JSpriteSurface()
{
}








