/***********************************************************************************/
//  File:   JRenderTarget.cpp
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "jlight.h"
#include "jrendertarget.h"

/***********************************************************************************/
/*  JRenderTarget implementation
/***********************************************************************************/
decl_class(JRenderTarget);
JRenderTarget::JRenderTarget()
{
    m_Width         = 0;
    m_Height        = 0;
    m_ClearColor    = 0;
    m_TexID         = -1;
}

void JRenderTarget::Render()
{
}

void JRenderTarget::PostRender()
{
}

void JRenderTarget::Init()
{
}

