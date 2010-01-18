//****************************************************************************/
//  File:  JFade.cpp
//  Desc:  
//****************************************************************************/
#include "precompile.h"
#include "Timer.h"
#include "JAnimation.h"
#include "JWidget.h"
#include "JFade.h"
#include "IWindowServer.h"

//****************************************************************************/
/*  JFade implementation
//****************************************************************************/
decl_class(JFade);
JFade::JFade()
{
    SetDuration( 1.0f );
    m_FadeMode      = FadeMode_In;
    m_Color         = 0;
    m_pFadeObject   = NULL;
    m_FadeObject    = "FadeObject";
    m_bFadeColor    = false;
} // JFade::JFade

void JFade::Init()
{
    m_pFadeObject = g_pObjectServer->FindObject<JWidget>( m_FadeObject.c_str(), this );
} // JFade::Init

void JFade::OnStop()
{
    if (m_pFadeObject)
    {
        m_pFadeObject->SetVisible( false );
    }
} // JFade::OnStop

void JFade::Render()
{   
    if (!IsPlaying()) return;

    if (!m_pFadeObject)
    {
        Init();
    }

    if (!m_pFadeObject)
    {
        Stop();
        return;
    }

    JAnimation::Render();
    if (!IsPlaying()) return;

    
    if (m_FadeMode == FadeMode_None) 
    {
        m_pFadeObject->SetVisible( false );
        return;
    }

    Frame ext;
    g_pWindowServer->GetRootExt( ext );

    uint32_t clr = m_Color;
    if (m_Color == 0) 
    {
        clr = m_pFadeObject->GetBgColor();
    }
    clr &= 0x00FFFFFF;
    float amt = GetCurTime()/GetDuration();
    if (amt >= 1.0f)
    {
        Stop();
        amt = 1.0f;
    }
    else if (IsPlaying())
    {
        m_pFadeObject->SetVisible( true );
    }

    if (m_FadeMode == FadeMode_In)
    {
        amt = 1 - amt;
    }

    if (m_bFadeColor)
    {
        ColorF clrF( m_Color );
        clrF *= amt;
        clr = clrF;
    }
    else
    {
        uint32_t alpha = uint32_t( amt*255.0f );
        alpha = clamp<uint32_t>( alpha, 0, 255 );
        alpha <<= 24;
        clr |= alpha;
    }

    m_pFadeObject->SetBgColor( clr );
    m_pFadeObject->SetFgColor( clr );
} // JFade::Render





