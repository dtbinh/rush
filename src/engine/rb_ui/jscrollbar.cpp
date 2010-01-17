/***********************************************************************************/
//  File:   JScrollBar.cpp
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "jslider.h"
#include "jbutton.h"
#include "jscrollbar.h"

/***********************************************************************************/
/*  JScrollBar implementation
/***********************************************************************************/
decl_class(JScrollBar);
JScrollBar::JScrollBar()
{
    m_pUp       = NULL;
    m_pDown     = NULL;
    m_pSlider   = NULL;
    m_pPage     = NULL;
    m_bVertical = false;
} // JScrollBar::JScrollBar

void JScrollBar::OnSize()
{
    if (!m_pUp || !m_pDown || !m_pSlider) return;

    Frame ext = GetExt();
    Frame uext = m_pUp->GetExt();
    Frame dext = m_pDown->GetExt();
    Frame sext = m_pSlider->GetExt();

    if (m_bVertical)
    {
        uext.x = ext.x;
        uext.y = ext.y;
        dext.x = ext.x;
        dext.y = ext.b() - dext.h;
        sext.y = uext.b();
        sext.h = ext.h - uext.h - dext.h;
        sext.w = ext.w;
    }
    else
    {
        uext.x = ext.x;
        uext.y = ext.y;
        dext.x = ext.r() - dext.w;
        dext.y = ext.y;
        sext.y = uext.r();
        sext.w = ext.w - uext.w - dext.w;
        sext.h = ext.h;
    }

    m_pUp->SetExt( uext );
    m_pDown->SetExt( dext );
    m_pSlider->SetExt( sext );
} // JScrollBar::OnSize

void JScrollBar::Init()
{
    m_pUp        = g_pObjectServer->FindObject<JButton>( "up", this );
    m_pDown      = g_pObjectServer->FindObject<JButton>( "down", this );
    m_pSlider    = g_pObjectServer->FindObject<JButton>( "slider", this );
} // JScrollBar::Init
