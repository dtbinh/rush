/***********************************************************************************/
//  File:   JSpritePack.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JSpritePack.h"

/***********************************************************************************/
/*  JSpritePack implementation
/***********************************************************************************/
decl_class(JSpritePack);
JSpritePack::JSpritePack()
{
    m_ColorFormat   = ColorFormat_Unknown;
    m_UpdateCB      = NULL;
    m_bAlwaysCache  = false;

    m_HeaderSize    = 0;
    m_LastFrame     = 0;
} // JSpritePack::JSpritePack