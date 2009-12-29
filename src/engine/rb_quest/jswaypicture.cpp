//****************************************************************************/
//  File:   JSwayPicture.cpp
//  Date:   28.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "Mesh.h"
#include "JPicture.h"
#include "JSwayPicture.h"

//****************************************************************************/
/*  JSwayPicture implementation 
//****************************************************************************/
decl_class(JSwayPicture);
JSwayPicture* JSwayPicture::s_pThis = NULL;
JSwayPicture::JSwayPicture()
{
    m_Pivot         = Vec2( 0.5f, 0.5f );
    m_Axis          = Vec2( 1.0f, 0.0f );
    m_Frequency     = 5.0f;
    m_Amplitude     = 5.0f;
    m_Fade          = 0.0f;
    m_FadeBias      = 1.0f;
} // JSwayPicture::JSwayPicture

void JSwayPicture::RippleCallback( void* pVert, int nVert, VertexType vType )
{
    if (!s_pThis || !pVert || nVert == 0 || vType != VertexType_XYZWDSUV) 
    {
        return;
    }
    s_pThis->ProcessVert( (VertexS*)pVert, nVert );
} // JSwayPicture::RippleCallback

void JSwayPicture::ProcessVert( VertexS* pVert, int nVert )
{
    Frame ext = GetExt();
    Vec3 center( ext.x + ext.w*m_Pivot.x, ext.y + ext.h*m_Pivot.y, 1.0f );
    float cTime = m_Timer.Seconds();
    float diag = sqrtf( ext.w*ext.w + ext.h*ext.h );
    for (int i = 0; i < nVert; i++)
    {
        VertexS& v = pVert[i];
        
        Vec3 pos( v.x, v.y, 1.0f );
        float dist = (pos - center).norm()/diag;
        float fadeFactor = expf( -(dist*dist)*m_Fade );

        float ang = m_Amplitude*fadeFactor;
        ang *= sinf( cTime*m_Frequency )*m_Amplitude;
        
        Mat3 tm1( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -center.x, -center.y, 1.0f );
        Mat3 tm2( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, center.x, center.y, 1.0f );
        Mat3 tm;
        tm.srt( 1.0f, ang, Vec2::null );
        Mat3 ftm = tm1*tm*tm2;
        ftm.tm( pos );

        v.x = pos.x;
        v.y = pos.y;
    }
} // JSwayPicture::ProcessVert

void JSwayPicture::DrawBounds()
{
    g_pDrawServer->SetVertexCB( NULL );
    Frame ext = GetExt();
    Vec2 pos( m_Pivot.x*ext.w + ext.x, m_Pivot.y*ext.h + ext.y );
    static int spID = g_pDrawServer->GetSpriteID( "quest_base" );
    g_pDrawServer->DrawSprite( pos.x - 4, pos.y - 4, spID, 0 );
    Vec2 end = pos;
    float len = 100.0f;
    end += m_Axis*len;
    g_pDrawServer->DrawLine( pos.x, pos.y, end.x, end.y, 0XFF7777FC, 0XFF7777FC );
    g_pDrawServer->Flush();
} 

void JSwayPicture::Init()
{
    m_Timer.Start();
}  

void JSwayPicture::Render()
{   
    s_pThis = this;
    g_pDrawServer->Flush();
    g_pDrawServer->SetVertexCB( RippleCallback );
    JPicture::Render();
} 

void JSwayPicture::PostRender()
{
    g_pDrawServer->Flush();
    g_pDrawServer->SetVertexCB( NULL );
    s_pThis = NULL;
}  
