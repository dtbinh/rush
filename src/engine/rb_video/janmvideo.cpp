/***********************************************************************************/
//  File:   JAnmVideo.cpp
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JReflect.h"
#include "IDrawServer.h"
#include "JAnimation.h"
#include "IRenderServer.h"
#include "JAnmVideo.h"

/***********************************************************************************/
/*  JAnmVideo implementation
/***********************************************************************************/
decl_class(JAnmVideo);
JAnmVideo::JAnmVideo()
{
} // JAnmVideo::JAnmVideo

JAnmVideo::~JAnmVideo()
{
    if (IsPlaying()) 
    {
        Stop();
    }
} // JAnmVideo::~JAnmVideo

void JAnmVideo::Render()
{
    JAnimation::Render();

    if (GetContext().m_bStateOnly) 
    {
        Stop();
        return;
    }

    if (IsPlaying())
    {
        //  check whether playback has ended
        Stop();
    }
} // JAnmVideo::Render

void JAnmVideo::OnStop()
{
} // JAnmVideo::OnStop

void JAnmVideo::OnPlay()
{
    HRESULT hRes = S_OK;

} // JAnmVideo::OnPlay
