//****************************************************************************/
//  File:  JUVScroll.cpp
//  Desc:  
//****************************************************************************/
#include "precompile.h"
#include "Timer.h"
#include "IRenderServer.h"
#include "JAnimation.h"
#include "JUVScroll.h"

//****************************************************************************/
/*  JUVScroll implementation
//****************************************************************************/
decl_class(JUVScroll);
JUVScroll::JUVScroll()
{
    m_USpeed = 0.1f;
    m_VSpeed = 0.1f;
} // JUVScroll::JUVScroll

void JUVScroll::Render()
{   
    JAnimation::Render();
    float cTime = GetContext().m_Time;
    float cx = m_USpeed*cTime;
    float cy = m_VSpeed*cTime;

    cx = fmodf( cx, 1.0f );
    cy = fmodf( cy, 1.0f );
    
    Mat4 tm = Mat4::identity;
    tm.e20 = cx;
    tm.e21 = cy;
    g_pRenderServer->SetTextureTM( tm );
} // JUVScroll::Render

void JUVScroll::PostRender()
{
    JAnimation::PostRender();
    g_pRenderServer->SetTextureTM( Mat4::identity );
} // JUVScroll::PostRender
