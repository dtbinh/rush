/***********************************************************************************/
//  File:   JScrollBox.cpp
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JScrollBar.h"
#include "JScrollBox.h"

/***********************************************************************************/
/*  JScrollBox implementation
/***********************************************************************************/
decl_class(JScrollBox);

JScrollBox::JScrollBox()
{
    m_pVBar = NULL;
    m_pHBar = NULL;
}

void JScrollBox::Init()
{
    m_pVBar = g_pObjectServer->FindObject<JScrollBar>( "vbar", this );
    m_pHBar = g_pObjectServer->FindObject<JScrollBar>( "hbar", this );
} // JScrollBox::Init