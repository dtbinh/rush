//****************************************************************************/
//  File:   JRipplePicture.cpp
//  Date:   28.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "Mesh.h"
#include "JPicture.h"
#include "JRipplePicture.h"

//****************************************************************************/
/*  JRipplePicture implementation 
//****************************************************************************/
decl_class(JRipplePicture);
JRipplePicture* JRipplePicture::s_pThis = NULL;
JRipplePicture::JRipplePicture()
{
    m_RippleAxis    = Vec2( 1.0f, 0.0f );
    m_Frequency     = 5.0f;
    m_Amplitude     = 5.0f;
    m_Fade          = 0.0f;
    m_FadeBias      = 1.0f;
    m_bOffscreen    = false;
    m_CellSide      = 64.0f;
    m_OffscreenRT   = -1;
    m_OldRT         = -1;
    m_bTintChildren = false;
} // JRipplePicture::JRipplePicture

void JRipplePicture::RippleCallback( void* pVert, int nVert, VertexType vType )
{
    if (!s_pThis || !pVert || nVert == 0 || vType != VertexType_XYZWDSUV) 
    {
        return;
    }
    s_pThis->ProcessVert( (VertexS*)pVert, nVert );
} // JRipplePicture::RippleCallback

void JRipplePicture::ProcessVert( VertexS* pVert, int nVert )
{
    Frame ext = GetExt();
    Vec2 center( ext.cx(), ext.cy() );
    float side = ext.w > ext.h ? ext.w : ext.h;
    side *= 0.5f;
    float cTime = m_Timer.Seconds();

    for (int i = 0; i < nVert; i++)
    {
        VertexS& v = pVert[i];
        
        Vec2 pos( v.x, v.y );
        float dist = (pos - center).norm()/side;
        float fadeFactor = expf( -(dist*dist)*m_Fade );
        Vec2 shift = m_RippleAxis*m_Amplitude*fadeFactor;
        shift *= sinf( cTime*m_Frequency + m_RippleAxis.dot( pos )*m_Amplitude );
        v.x += shift.x;
        v.y += shift.y;
    }
} 

void JRipplePicture::Init()
{
    m_Timer.Start();

    if (m_bOffscreen)
    {
        Frame ext = GetExt();
        m_OffscreenRT = g_pRenderServer->GetTextureID( "BackBufferCopy" );
        if (m_OffscreenRT == -1)
        {
            m_OffscreenRT = g_pRenderServer->CreateRenderTarget( "BackBufferCopy", ext.w, ext.h );
        }
    }
} // JRipplePicture::Init

void JRipplePicture::Render()
{   
    s_pThis = this;
    g_pDrawServer->Flush();

    if (m_bOffscreen)
    {
        m_OldRT = g_pRenderServer->GetRenderTarget();
        g_pRenderServer->SetRenderTarget( m_OffscreenRT );
    }
    else
    {
        g_pDrawServer->SetVertexCB( RippleCallback );
    }

    JPicture::Render();


} // JRipplePicture::Render

void JRipplePicture::PostRender()
{
    g_pDrawServer->Flush();

    if (m_bOffscreen)
    {
        if (m_OffscreenRT == -1) return;
        g_pRenderServer->SetRenderTarget( m_OldRT );

        //  create ripple mesh
        Frame ext = GetExt();
        int nodesW = ext.w/m_CellSide + 1;
        int nodesH = ext.h/m_CellSide + 1;

        int nVert = (nodesW - 1)*(nodesH - 1)*4;
        if (m_RippleVert.size() != nVert)
        {
            m_RippleVert.resize( nVert );
        }

        float rw = ext.w/float( nodesW - 1 );
        float rh = ext.h/float( nodesH - 1 );

        DWORD color = 0xFFFFFFFF;
        if (m_bTintChildren) color = GetFgColor();

        //  fill in vertices
        int cV = 0;
        for (int j = 1; j < nodesH; j++)
        {
            for (int i = 1; i < nodesW; i++)
            {
                VertexS v0, v1, v2, v3;

                v0.x = ext.x + float( i - 1 )*rw;
                v0.y = ext.y + float( j - 1 )*rh;
                v0.z = 0.0f;
                v0.w = 1.0f;
                v0.diffuse = color;
                v0.u = float( i - 1 )/float( nodesW - 1 );
                v0.v = float( j - 1 )/float( nodesH - 1 );

                v1.x = ext.x + float( i )*rw;
                v1.y = ext.y + float( j - 1 )*rh;
                v1.z = 0.0f;
                v1.w = 1.0f;
                v1.diffuse = color;
                v1.u = float( i )/float( nodesW - 1 );
                v1.v = float( j - 1 )/float( nodesH - 1 );

                v2.x = ext.x + float( i - 1 )*rw;
                v2.y = ext.y + float( j )*rh;
                v2.z = 0.0f;
                v2.w = 1.0f;
                v2.diffuse = color;
                v2.u = float( i - 1 )/float( nodesW - 1 );
                v2.v = float( j )/float( nodesH - 1 );

                v3.x = ext.x + float( i )*rw;
                v3.y = ext.y + float( j )*rh;
                v3.z = 0.0f;
                v3.w = 1.0f;
                v3.diffuse = color;
                v3.u = float( i )/float( nodesW - 1 );
                v3.v = float( j )/float( nodesH - 1 );

                m_RippleVert[cV] = v0;
                cV++;
                m_RippleVert[cV] = v1;
                cV++;
                m_RippleVert[cV] = v2;
                cV++;
                m_RippleVert[cV] = v3;
                cV++;
            }
        }

        //  ripple grid
        ProcessVert( &m_RippleVert[0], m_RippleVert.size() );

        //  draw grid
        g_pRenderServer->SetTexture( m_OffscreenRT, 0 );
        g_pRenderServer->SetShader( g_pRenderServer->GetShaderID( "base_sprite" ) );
        g_pRenderServer->SetShader( g_pRenderServer->GetShaderID( "ts0_linfilter" ) );
        int vbID = g_pRenderServer->GetVBID( "DrawSystemDynamic" );
        g_pRenderServer->SetVB( vbID, VertexS::Type );
        int iter = -1;
        int firstByte = 0;
        g_pRenderServer->CacheVB( vbID, (BYTE*)&m_RippleVert[0], m_RippleVert.size()*sizeof( VertexS ), 
                      sizeof( VertexS ), iter, firstByte );
        g_pRenderServer->Draw( 0, 0, firstByte/sizeof( VertexS ), m_RippleVert.size(), PrimitiveType_QuadList );

    }
    else
    {
        g_pDrawServer->SetVertexCB( NULL );
    }
    s_pThis = NULL;
} // JRipplePicture::PostRender
