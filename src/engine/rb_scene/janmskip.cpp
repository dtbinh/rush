/***********************************************************************************/
//  File:   JAnmSkip.cpp
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JAnimation.h"
#include "JAnimServer.h"
#include "JAnmSkip.h"
#include <algorithm>
#include "windows.h"

/***********************************************************************************/
/*  JAnmSkip implementation
/***********************************************************************************/
decl_class(JAnmSkip);
JAnmSkip::JAnmSkip()
{
    m_pAnimation = NULL;
    m_TimeDelta  = 0.005f;
    m_bRewinding = false;
} // JAnmSkip::JAnmSkip

void JAnmSkip::Init()
{
    m_pAnimation = g_pObjectServer->FindObject<JAnimation>( m_Animation.c_str(), this );
} // JAnmSkip::Init

void JAnmSkip::Skip()
{
    if (!m_pAnimation || !m_pAnimation->IsPlaying()) return;
    m_bRewinding = true;

    JAnmContext& ctx  = JAnimation::PushContext();
    ctx.m_bStateOnly  = true;
    ctx.m_TimeDelta   = m_TimeDelta;
    float  cTime      = m_pAnimation->GetCurTime();

    bool bVisible = m_pAnimation->IsVisible();
    m_pAnimation->SetVisible( true );
    while (m_pAnimation->IsPlaying() && m_bRewinding)
    {
        m_pAnimation->RenderTree();
    }

    JAnimation::PopContext();
} // JAnmSkip::Skip

void JAnmSkip::SkipAll()
{
    if (!m_pAnimation || !m_pAnimation->IsPlaying()) return;
    while (m_pAnimation->IsPlaying())
    {
        Skip();
    }
} // JAnmSkip::SkipAll

void JAnmSkip::OnKey( JKeyEvent& e )
{
    if (!m_pAnimation || !m_pAnimation->IsPlaying() || e.Action() != aKeyUp)
    {
        return;
    }

    if (e.Key() == VK_SPACE)
    {
        e.Consume();
        Skip();
    }
    /*else if (e.Key() == VK_ESCAPE)
    {
        e.Consume();
        SkipAll();
    }*/

} // JAnmSkip::OnKey
