//****************************************************************************/
//  File:  JDrawServer.cpp
//  Desc:  
//****************************************************************************/
#include "precompile.h"
#include "JDrawServer.h"

#include "IDrawServer.h"
#include "IRenderServer.h"
#include "VertexType.h"
#include "MappedFile.h"
#include "FStream.h"
#include "direct.h"
#include "BitOps.h"
#include "Vec4.h"
#include "Vec2.h"
#include "AABox.h"
#include "Vec3.h"
#include "TriMesh2.h"
#include "Frustum.h"
#include "PolyLine2.h"
#include "JModel.h"
#include "JModelAnim.h"
#include "JSpritePack.h"
#include "Path.h"
#include "ColorConversion.h"
#include <algorithm>
#include "IFileServer.h"

//****************************************************************************/
/*	JDrawServer implementation
//****************************************************************************/
decl_class(JDrawServer);

uint16_t                JDrawServer::s_QuadIdx[c_MaxVertDrawn*6/4];
std::vector<uint8_t>   JDrawServer::s_UnpackBuffer;

JDrawServer::JDrawServer() 
{
    int cI = 0;
    for (int i = 0; i < c_MaxVertDrawn; i += 4)
    {
        s_QuadIdx[cI++] = i + 0;
        s_QuadIdx[cI++] = i + 1;
        s_QuadIdx[cI++] = i + 2;
        s_QuadIdx[cI++] = i + 2;
        s_QuadIdx[cI++] = i + 1;
        s_QuadIdx[cI++] = i + 3;
    }

    m_bLinFilter        = false;
    m_bZEnable          = false;
    m_bZWriteEnable     = false;
    m_bMod2x            = false;
    m_bMod4x            = false;
    m_bAdditive         = false;
    m_bAdjustUV         = false;

    m_shBase            = -1;
    m_shAdd             = -1; 
    m_shZEnable         = -1; 

    m_shLinf            = -1; 
    m_shMod2x           = -1; 
    m_shMod4x           = -1; 
    m_shNoTex           = -1; 

    m_PrimVBID          = -1;
    m_CurVBPos          = 0;
    VertProcessCB       = NULL;

    m_NDrawnSprites     = 0;
    m_NFlushes          = 0;

    m_CurFrame          = 0;

    m_MemoryBudget      = 128*1024*1024;
    m_CurBytesAllocated = 0;
    m_pBuffer           = NULL;

    m_NSpriteSurfaces   = 64;
    m_NSpriteSurfaces16 = 32;

    m_WorldTM           = Mat4::identity;

    SetName( "draw" );
    
    g_pDrawServer = this;
} // JDrawServer::JDrawServer

JDrawServer::~JDrawServer()
{
    Cleanup();
} // JDrawServer::~JDrawServer

void JDrawServer::Cleanup()
{
    delete []m_pBuffer;
    int nSP = m_SpritePacks.size();
    for (int i = 0; i < nSP; i++)
    {
        JSpritePack& sp = *(m_SpritePacks[i]);
        if (sp.m_bAlwaysCache)
        {
            int nF = sp.m_Frames.size();
            for (int j = 0; j < nF; j++)
            {
                JSpriteFrame& fr = sp.m_Frames[j];
                delete []fr.m_pPixels;
            }
        }
    }
    m_Surfaces.clear();
} // JDrawServer::Cleanup

void JDrawServer::SetNSpriteSurf( int numTotalSurfaces, int num16BitSurfaces ) 
{ 
    m_NSpriteSurfaces = numTotalSurfaces; 
    m_NSpriteSurfaces16 = num16BitSurfaces; 
} // JDrawServer::SetNSpriteSurf

void JDrawServer::Init()
{   
    g_pRenderServer->Register( this );
    //  initialize sprite surfaces
    m_Surfaces.reserve( m_NSpriteSurfaces );
    for (int i = 0; i < m_NSpriteSurfaces; i++)
    {
        ColorFormat colorFormat = (i > m_NSpriteSurfaces16) ? 
            ColorFormat_ARGB8888 : ColorFormat_ARGB4444;
        int texID = -1;
        char texName[_MAX_PATH];
        sprintf( texName, "spr%s_%03d", (colorFormat == ColorFormat_ARGB8888 ? "32" : "16"), i );
        TextureProperties tp;
        tp.m_Width          = c_SpriteSurfaceSide;
        tp.m_Height         = c_SpriteSurfaceSide;
        tp.m_NMips          = 1;
        tp.m_bAutoGenMips   = true;
        tp.m_PoolType       = PoolType_Managed;
        tp.m_Format         = colorFormat;
        tp.SetName( texName );
        
        texID = g_pRenderServer->CreateTexture( tp );
        if (texID >= 0)
        {
            JSpriteSurface surf;
            surf.m_TexID      = texID;
            surf.m_ColorFormat = colorFormat;
            m_Surfaces.push_back( surf );
        }
    }

    m_PrimVBID          = g_pRenderServer->CreateVB( "DrawSystemDynamic", c_DynamicVBSize, false );

    m_shBase            = g_pRenderServer->GetShaderID( "base_sprite" );
    m_shAdd             = g_pRenderServer->GetShaderID( "blend_additive" );
    m_shZEnable         = g_pRenderServer->GetShaderID( "enablez" );
    m_shZWriteDisable   = g_pRenderServer->GetShaderID( "zwritedisable" );

    m_shLinf            = g_pRenderServer->GetShaderID( "ts0_linfilter" );
    m_shMod2x           = g_pRenderServer->GetShaderID( "ts0_mod2x" );
    m_shMod4x           = g_pRenderServer->GetShaderID( "ts0_mod4x" );
    m_shNoTex           = g_pRenderServer->GetShaderID( "ts0_notex" );

    //  allocate sprite cache memory pool
    if (!m_pBuffer)
    {
        m_pBuffer = new uint8_t[m_MemoryBudget];
    }
} // JDrawServer::Init

int JDrawServer::GetStatistics( DrawSystemStat stat ) const
{
    switch (stat)
    {
    case DSStat_SpriteCacheSize: return m_CurBytesAllocated;
    case DSStat_NumSpritesDrawn: return m_NDrawnSprites;
    case DSStat_NumFlushes     : return m_NFlushes;
    }
    return 0;
} // JDrawServer::GetStatistics

void JDrawServer::OnFrame()
{
    Flush();
    
    SetWorldTM( Mat4::identity );
    m_NDrawnSprites = 0;
    m_NFlushes      = 0;
    m_CurFrame++;
} // JDrawServer::OnFrame

void JDrawServer::SetupRS( uint32_t flags )
{
    g_pRenderServer->SetShader( m_shBase );
    if (flags&rfAdditive) 
    {
        g_pRenderServer->SetShader( m_shAdd );
    }
    if (flags&rfLine || flags&rfQuad || flags&rfPoly) 
    {
        g_pRenderServer->SetShader( m_shNoTex );
    }
    if (flags&rfSprite)
    {
        if (flags&rfTexMod2x) 
        {
            g_pRenderServer->SetShader( m_shMod2x );
        }
        else if (flags&rfTexMod4x) 
        {
            g_pRenderServer->SetShader( m_shMod4x );
        }
    }
    if (flags&rfLinearFilter) 
    {
        g_pRenderServer->SetShader( m_shLinf );
    }
    if (flags&rfZEnable) 
    {
        g_pRenderServer->SetShader( m_shZEnable );
    }

    if (flags&rfZWriteDisable) 
    {
        g_pRenderServer->SetShader( m_shZWriteDisable );
    }
} // JDrawServer::SetupRS

inline bool RBCompare( const RenderBit* e1, const RenderBit* e2 )
{
    return (*e1) < (*e2);
}


inline void FillChunkVertsS( const RenderBit& rb, const JFrameChunk& chunk, VertexS* pVert )
{
    Vec3 pt0( chunk.m_X0, chunk.m_Y0, 0.0f );
    Vec3 pt1( chunk.m_X1, chunk.m_Y0, 0.0f );
    Vec3 pt2( chunk.m_X0, chunk.m_Y1, 0.0f );
    Vec3 pt3( chunk.m_X1, chunk.m_Y1, 0.0f );

    rb.m_TM.tmpt( pt0 );
    rb.m_TM.tmpt( pt1 );
    rb.m_TM.tmpt( pt2 );
    rb.m_TM.tmpt( pt3 );

    VertexS& v0 = pVert[0];
    v0.x        = pt0.x - c_HalfPixel;
    v0.y        = pt0.y - c_HalfPixel;
    v0.z        = 0.0f;
    v0.w        = 1.0f;
    v0.diffuse  = rb.m_Clr[0];

    VertexS& v1 = pVert[1];
    v1.x        = pt1.x - c_HalfPixel;
    v1.y        = pt1.y - c_HalfPixel;
    v1.z        = 0.0f;
    v1.w        = 1.0f;
    v1.diffuse  = rb.m_Clr[1];

    VertexS& v2 = pVert[2];
    v2.x        = pt2.x - c_HalfPixel;
    v2.y        = pt2.y - c_HalfPixel;
    v2.z        = 0.0f;
    v2.w        = 1.0f;
    v2.diffuse  = rb.m_Clr[2];

    VertexS& v3 = pVert[3];
    v3.x        = pt3.x - c_HalfPixel;
    v3.y        = pt3.y - c_HalfPixel;
    v3.z        = 0.0f;
    v3.w        = 1.0f;
    v3.diffuse  = rb.m_Clr[3];

    float du = 0.0f;
    float dv = 0.0f;
    
    if (rb.m_Flags&rfAdjustUV)
    {
        du = 0.5f/c_fSpriteSurfaceSide; 
        dv = 0.5f/c_fSpriteSurfaceSide; 
    }

    v0.u        = chunk.m_U0 + du;
    v0.v        = chunk.m_V0 + dv;

    v1.u        = chunk.m_U1 - du;
    v1.v        = chunk.m_V0 + dv;

    v2.u        = chunk.m_U0 + du;
    v2.v        = chunk.m_V1 - dv;

    v3.u        = chunk.m_U1 - du;
    v3.v        = chunk.m_V1 - dv;
} // FillChunkVertsS

inline void FillChunkVertsW( const RenderBit& rb, const JFrameChunk& chunk, VertexW* pVert )
{
    Vec4 v[4];
    v[0].x = chunk.m_X0; v[0].y = chunk.m_Y0; v[0].z = chunk.m_U0; v[0].w = chunk.m_V0;
    v[1].x = chunk.m_X1; v[1].y = chunk.m_Y0; v[1].z = chunk.m_U1; v[1].w = chunk.m_V0;
    v[2].x = chunk.m_X0; v[2].y = chunk.m_Y1; v[2].z = chunk.m_U0; v[2].w = chunk.m_V1;
    v[3].x = chunk.m_X1; v[3].y = chunk.m_Y1; v[3].z = chunk.m_U1; v[3].w = chunk.m_V1;

    for (int j = 0; j < 4; j++)
    {
        Vec3 pt( v[j].x, v[j].y, 0.0f );
        rb.m_TM.tmpt( pt );
        VertexW& cv = pVert[j];
        cv.x        = pt.x;
        cv.y        = pt.y;
        cv.z        = pt.z;
        cv.u        = v[j].z;
        cv.v        = v[j].w;
        cv.diffuse  = rb.m_Clr[j];
    }
} // FillChunkVertsW

VertexW   vertW[c_MaxVertDrawn];
VertexS   vertS[c_MaxVertDrawn];
void JDrawServer::Flush()
{
    if (m_RenderBits.size() == 0 || !g_pRenderServer->IsInited()) 
    {
        return;
    }
    m_NFlushes++;
    
    g_pRenderServer->SetWorldTM( Mat4::identity );

    int nB = m_RenderBits.size();
    m_SortedRenderBits.clear();
    for (int i = 0; i < nB; i++) 
    {
        m_SortedRenderBits.push_back( &m_RenderBits[i] );
    }
    std::sort( m_SortedRenderBits.begin(), m_SortedRenderBits.end(), RBCompare );
    uint32_t flags = m_SortedRenderBits[0]->m_Flags;
    int   cV    = 0;
    int   cPri  = 0;

    VertexW*  pVW = vertW;
    VertexS*  pVS = vertS;

    for (int i = 0; i <= nB; i++)
    {
        if (i == nB || m_SortedRenderBits[i]->m_Flags != flags || cV >= c_MaxVertDrawn - 4)
        {
            g_pRenderServer->SetTexture( m_Surfaces[flags>>16].m_TexID, 0 );
            SetupRS( flags );
            
            PrimitiveType priType = PrimitiveType_QuadList;
            if (flags&rfLine) priType = PrimitiveType_LineList;
            if (flags&rfPoly) priType = PrimitiveType_TriangleList;

            if (flags&rfScreenSpace)
            {
                Draw( vertS, cV, priType );
            }
            else
            {
                Draw( vertW, cV, priType );
            }

            cPri        = 0;
            cV          = 0;
            pVW         = vertW;
            pVS         = vertS;
            if (i == nB) break;
            flags       = m_SortedRenderBits[i]->m_Flags;
        }

        const RenderBit& rb = *m_SortedRenderBits[i];

        if (rb.m_Flags&rfSprite)
        {
            const JFrameChunk& chunk = *((JFrameChunk*)rb.m_pData);
            cV += 4; cPri += 1;
            if (rb.m_Flags&rfScreenSpace)
            {
                FillChunkVertsS( rb, chunk, pVS );
                pVS += 4;
            }
            else
            {
                FillChunkVertsW( rb, chunk, pVW );
                pVW += 4;
            }
        }
        else if (rb.m_Flags&rfLine)
        {
            cV += 2; cPri += 1;
            const Vec4& v1 = rb.m_TM.v0();
            const Vec4& v2 = rb.m_TM.v1();

            if (rb.m_Flags&rfScreenSpace)
            {
                VertexS& cv1 = *(pVS++);
                VertexS& cv2 = *(pVS++);
                cv1.x = v1.x; cv1.y = v1.y; cv1.z = v1.z; cv1.w = 1.0f; cv1.diffuse = rb.m_Clr[0];
                cv2.x = v2.x; cv2.y = v2.y; cv2.z = v2.z; cv2.w = 1.0f; cv2.diffuse = rb.m_Clr[1];
            }
            else
            {
                VertexW& cv1 = *(pVW++);
                VertexW& cv2 = *(pVW++);
                cv1.x = v1.x; cv1.y = v1.y; cv1.z = v1.z; cv1.diffuse = rb.m_Clr[0];
                cv2.x = v2.x; cv2.y = v2.y; cv2.z = v2.z; cv2.diffuse = rb.m_Clr[1];
            }
        }
        else if (rb.m_Flags&rfQuad)
        {
            Vec4 v[4];
            v[0] = rb.m_TM.v0();
            v[1] = rb.m_TM.v1();
            v[2] = rb.m_TM.v2();
            v[3] = rb.m_TM.v3();

            cV += 4; cPri += 1;
            if (rb.m_Flags&rfScreenSpace)
            {
                for (int j = 0; j < 4; j++, pVS++)
                {
                    VertexS& cv = *pVS;
                    cv.x        = v[j].x;
                    cv.y        = v[j].y;
                    cv.z        = v[j].z;
                    cv.w        = 1.0f;
                    cv.diffuse  = rb.m_Clr[j];
                }
            }
            else
            {
                for (int j = 0; j < 4; j++, pVW++)
                {
                    VertexW& cv = *pVW;
                    cv.x        = v[j].x;
                    cv.y        = v[j].y;
                    cv.z        = v[j].z;
                    cv.diffuse  = rb.m_Clr[j];
                }
            }
        }
        else if (rb.m_Flags&rfPoly)
        {
            cV += 3; cPri += 1;
            const Vec4& v1 = rb.m_TM.v0();
            const Vec4& v2 = rb.m_TM.v1();
            const Vec4& v3 = rb.m_TM.v2();

            if (rb.m_Flags&rfScreenSpace)
            {
                VertexS& cv1 = *(pVS++);
                VertexS& cv2 = *(pVS++);
                VertexS& cv3 = *(pVS++);

                cv1.x = v1.x; cv1.y = v1.y; cv1.z = v1.z; cv1.w = 1.0f; cv1.diffuse = rb.m_Clr[0];
                cv2.x = v2.x; cv2.y = v2.y; cv2.z = v2.z; cv2.w = 1.0f; cv2.diffuse = rb.m_Clr[1];
                cv3.x = v3.x; cv3.y = v3.y; cv3.z = v3.z; cv3.w = 1.0f; cv3.diffuse = rb.m_Clr[2];
            }
            else
            {
                VertexW& cv1 = *(pVW++);
                VertexW& cv2 = *(pVW++);
                VertexW& cv3 = *(pVW++);

                cv1.x = v1.x; cv1.y = v1.y; cv1.z = v1.z; cv1.diffuse = rb.m_Clr[0];
                cv2.x = v2.x; cv2.y = v2.y; cv2.z = v2.z; cv2.diffuse = rb.m_Clr[1];
                cv3.x = v3.x; cv3.y = v3.y; cv3.z = v3.z; cv3.diffuse = rb.m_Clr[2];
            }
        }

    }

    m_RenderBits.clear();
} // JDrawServer::Flush

void JDrawServer::ReloadSprites()
{
    assert( false );
} // JDrawServer::ReloadSprites

void JDrawServer::DrawLine( const Vec3& a, const Vec3& b, uint32_t ca, uint32_t cb )
{
    if (m_RenderBits.size() == c_MaxRenderBits) Flush();
    RenderBit& rb = m_RenderBits.expand();
    rb.m_Clr[0]         = ca;
    rb.m_Clr[1]         = cb;
    rb.m_pData          = NULL;
    rb.m_Flags          = rfLine;
    rb.m_TM.v0().x      = a.x;
    rb.m_TM.v0().y      = a.y;
    rb.m_TM.v0().z      = a.z;
    rb.m_TM.v1().x      = b.x;
    rb.m_TM.v1().y      = b.y;
    rb.m_TM.v1().z      = b.z;
    
    m_WorldTM.tm( rb.m_TM.v0() );
    m_WorldTM.tm( rb.m_TM.v1() );
} // JDrawServer::DrawLine

void JDrawServer::DrawPoly( const Vec3& a, const Vec3& b, const Vec3& c, uint32_t ca, uint32_t cb, uint32_t cc )
{
    if (m_RenderBits.size() == c_MaxRenderBits) Flush();
    RenderBit& rb = m_RenderBits.expand();
    rb.m_Clr[0]         = ca;
    rb.m_Clr[1]         = cb;
    rb.m_Clr[2]         = cc;
    rb.m_pData          = NULL;
    rb.m_Flags          = rfPoly;
    Mat4& m = rb.m_TM;
    m.e00 = a.x; m.e01 = a.y; m.e02 = a.z; m.e03 = 1.0f;
    m.e10 = b.x; m.e11 = b.y; m.e12 = b.z; m.e13 = 1.0f;
    m.e20 = c.x; m.e21 = c.y; m.e22 = c.z; m.e23 = 1.0f;

    m_WorldTM.tm( rb.m_TM.v0() );
    m_WorldTM.tm( rb.m_TM.v1() );
    m_WorldTM.tm( rb.m_TM.v2() );
} // JDrawServer::DrawPoly

void JDrawServer::DrawPoly( const Vec2& a, const Vec2& b, const Vec2& c, uint32_t ca, uint32_t cb, uint32_t cc )
{
    if (m_RenderBits.size() == c_MaxRenderBits) Flush();
    RenderBit& rb = m_RenderBits.expand();
    rb.m_Clr[0]         = ca;
    rb.m_Clr[1]         = cb;
    rb.m_Clr[2]         = cc;
    rb.m_pData          = NULL;
    rb.m_Flags          = rfPoly;
    rb.m_Flags          |= rfScreenSpace;
    Mat4& m = rb.m_TM;
    m.e00 = a.x; m.e01 = a.y; m.e02 = 0.0f; m.e03 = 1.0f;
    m.e10 = b.x; m.e11 = b.y; m.e12 = 0.0f; m.e13 = 1.0f;
    m.e20 = c.x; m.e21 = c.y; m.e22 = 0.0f; m.e23 = 1.0f;

    m_WorldTM.tm( rb.m_TM.v0() );
    m_WorldTM.tm( rb.m_TM.v1() );
    m_WorldTM.tm( rb.m_TM.v2() );
} // JDrawServer::DrawPoly

void JDrawServer::DrawLine( int x1, int y1, int x2, int y2, uint32_t ca, uint32_t cb )
{
    if (m_RenderBits.size() == c_MaxRenderBits) Flush();
    RenderBit& rb = m_RenderBits.expand();
    rb.m_Clr[0]         = ca;
    rb.m_Clr[1]         = cb;
    rb.m_pData          = NULL;
    rb.m_Flags          = rfLine;
    rb.m_Flags          |= rfScreenSpace;
    rb.m_TM.v0().x      = x1;
    rb.m_TM.v0().y      = y1;
    rb.m_TM.v0().z      = 0.0f;
    rb.m_TM.v1().x      = x2;
    rb.m_TM.v1().y      = y2;
    rb.m_TM.v1().z      = 0.0f;

    m_WorldTM.tm( rb.m_TM.v0() );
    m_WorldTM.tm( rb.m_TM.v1() );
} // JDrawServer::DrawLine

void JDrawServer::DrawRect( const Frame& rc, uint32_t color )
{
    if (m_RenderBits.size() == c_MaxRenderBits) Flush();
    RenderBit& rb = m_RenderBits.expand();
    rb.m_Clr[0]           = color;
    rb.m_Clr[1]           = color;
    rb.m_Clr[2]           = color;
    rb.m_Clr[3]           = color;
    rb.m_pData          = NULL;
    rb.m_Flags          = rfQuad;
    rb.m_Flags          |= rfScreenSpace;
    rb.m_TM.v0()        = Vec4( rc.x,   rc.y,   0.0f, 1.0f );
    rb.m_TM.v1()        = Vec4( rc.r(), rc.y,   0.0f, 1.0f );
    rb.m_TM.v2()        = Vec4( rc.x,   rc.b(), 0.0f, 1.0f );
    rb.m_TM.v3()        = Vec4( rc.r(), rc.b(), 0.0f, 1.0f );

    m_WorldTM.tm( rb.m_TM.v0() );
    m_WorldTM.tm( rb.m_TM.v1() );
    m_WorldTM.tm( rb.m_TM.v2() );
    m_WorldTM.tm( rb.m_TM.v3() );
} // JDrawServer::DrawRect

void JDrawServer::DrawFrame( const Frame& rc, uint32_t color )
{
    DrawLine( rc.x, rc.y, rc.x + rc.w, rc.y, color, color );
    DrawLine( rc.x + rc.w, rc.y, rc.x + rc.w, rc.y + rc.h, color, color );
    DrawLine( rc.x + rc.w, rc.y + rc.h, rc.x, rc.y + rc.h, color, color );
    DrawLine( rc.x, rc.y + rc.h, rc.x, rc.y, color, color ); 
} // JDrawServer::DrawRect

void JDrawServer::OnResetDevice()
{
    int nSurf = m_Surfaces.size();
    for (int i = 0; i < nSurf; i++)
    {
        m_Surfaces[i].Drop();
    }
} // JDrawServer::OnResetDevice

Vec2 JDrawServer::GetFramePivot( int sprID, int frameID )
{
    if (sprID < 0 || sprID >= (int)m_SpritePacks.size()) return Vec2::null;
    JSpritePack& sp = *(m_SpritePacks[sprID]);
    if (frameID < 0 || frameID >= (int)sp.m_Frames.size()) return Vec2::null;
    JSpriteFrame& sf = sp.m_Frames[frameID];
    return sf.m_Pivot;
} // JDrawServer::GetFramePivot

Vec2 JDrawServer::GetFrameShift( int sprID, int frameID )
{
    if (sprID < 0 || sprID >= (int)m_SpritePacks.size()) return Vec2::null;
    JSpritePack& sp = *(m_SpritePacks[sprID]);
    if (frameID < 0 || frameID >= (int)sp.m_Frames.size()) return Vec2::null;
    JSpriteFrame& sf = sp.m_Frames[frameID];
    return sf.m_Shift;
} // JDrawServer::GetFrameShift

int JDrawServer::GetFrameW( int sprID, int frameID )
{
    if (sprID < 0 || sprID >= (int)m_SpritePacks.size()) return 0;
    JSpritePack& sp = *(m_SpritePacks[sprID]);
    if (frameID < 0 || frameID >= (int)sp.m_Frames.size()) return 0;
    JSpriteFrame& sf = sp.m_Frames[frameID];
    return sf.m_Width;
} // JDrawServer::GetFrameW

int JDrawServer::GetFrameH( int sprID, int frameID )
{
    if (sprID < 0 || sprID >= (int)m_SpritePacks.size()) return 0;
    JSpritePack& sp = *(m_SpritePacks[sprID]);
    if (frameID < 0 || frameID >= (int)sp.m_Frames.size()) return 0;
    JSpriteFrame& sf = sp.m_Frames[frameID];
    return sf.m_Height;
} // JDrawServer::GetFrameH

Frame JDrawServer::GetFrameBounds( int sprID, int frameID )
{
    Frame rct;
    if (sprID < 0 || sprID >= (int)m_SpritePacks.size()) return rct;
    JSpritePack& sp = *(m_SpritePacks[sprID]);
    if (frameID < 0 || frameID >= (int)sp.m_Frames.size()) return rct;
    JSpriteFrame& sf = sp.m_Frames[frameID];
    return sf.m_Frame;
} // JDrawServer::GetFrameBounds

uint32_t JDrawServer::GetPixel( int sprID, int frameID, const Vec2& pt ) const
{
    if (sprID < 0 || sprID >= (int)m_SpritePacks.size()) return 0;
    const JSpritePack& sp = *(m_SpritePacks[sprID]);
    if (frameID < 0 || frameID >= (int)sp.m_Frames.size()) return 0;
    const JSpriteFrame& sf = sp.m_Frames[frameID];
    if (!sf.m_Frame.PtIn( pt.x, pt.y )) return 0;
    for (int i = 0; i < sf.m_Chunks.size(); i++)
    {
        const JFrameChunk& ch = sf.m_Chunks[i];
        if (pt.x < ch.m_SprX) continue;
        if (pt.y < ch.m_SprY) continue;
        if (pt.x >= ch.m_SprX + ch.m_Side) continue;
        if (pt.y >= ch.m_SprY + ch.m_Side) continue;
        //  point is inside chunk - probe color
        int dx = pt.x - ch.m_SprX;
        int dy = pt.y - ch.m_SprY;
        int side = ch.m_Side;
        if (ch.m_SprX + ch.m_Side > sf.m_Frame.r())
        {
            side = sf.m_Frame.r() - ch.m_SprX;
        }
        if (sf.m_bHasBorder)
        {
            dx++;
            dy++;
            side += 2;
        }
        if (sf.m_ColorFormat == ColorFormat_ARGB4444)
        {
            const uint8_t* pPixel = ch.m_Pixels + (dx + dy*side)*2;
            uint32_t res  = 0;
            uint8_t* pRes = (uint8_t*)&res;
            ConvertPixel<ColorFormat_ARGB8888, ColorFormat_ARGB4444>( pRes, pPixel );
            return res;
        }
        else
        {
            return uint32_t( ((uint32_t*)ch.m_Pixels)[dx + dy*side] );
        }
    }
    return 0;
} // JDrawServer::GetPixel

uint8_t* JDrawServer::AllocateBuffer( int nBytes, bool bAlwaysCache )
{
    if (bAlwaysCache)
    {
        return new uint8_t[nBytes];
    }
    if (!m_pBuffer) 
    {
        m_pBuffer = new uint8_t[m_MemoryBudget];
    }

    uint8_t* pData = &m_pBuffer[m_CurBytesAllocated];
    m_CurBytesAllocated += nBytes;

    //  if no more place - drop the whole cache
    if (m_CurBytesAllocated > m_MemoryBudget)
    {
        int nSP = m_SpritePacks.size();
        for (int i = 0; i < nSP; i++)
        {
            JSpritePack& sp = *(m_SpritePacks[i]);
            if (sp.m_bAlwaysCache)
            {
                continue;
            }
            int nF = sp.m_Frames.size();
            for (int j = 0; j < nF; j++)
            {
                JSpriteFrame& frame = sp.m_Frames[j];
                frame.m_pPixels = NULL;
            }
        }
        m_CurBytesAllocated = 0;
        pData = m_pBuffer;
        m_CurBytesAllocated = nBytes;
    }
    return pData;
} // JDrawServer::AllocateBuffer

int JDrawServer::GetSpriteID( const char* packageName, PSpriteUpdateCB updateCB, bool bAlwaysCache )
{
    if (!packageName || packageName[0] == 0) return -1;
    Flush();

    int nP = (int)m_SpritePacks.size();
    for (int i = 0; i < nP; i++)
    {
        if (!stricmp( packageName, m_SpritePacks[i]->GetName() ))
        {
            return i;
        }
    }
    
    FInStream is;
    if (!g_pFileServer->OpenMedia( packageName, "rspr", is )) 
    {
        return -1;
    }

    JSpritePack* pSpritePack = new JSpritePack();
    m_SpritePacks.push_back( pSpritePack );
    int packID = m_SpritePacks.size() - 1;
    pSpritePack->SetName( packageName );
    pSpritePack->m_UpdateCB       = updateCB;
    pSpritePack->m_bAlwaysCache   = bAlwaysCache;

    int curByte = 0;
    uint16_t nSpr;
    uint32_t magic, rsrv1, rsrv2;
    is >> magic >> nSpr >> rsrv1 >> rsrv2;
    pSpritePack->m_Frames.resize( nSpr );
    pSpritePack->m_LastFrame = 0;
    for (int i = 0; i < nSpr; i++)
    {
        uint16_t w, h, cf, fx, fy, fw, fh, nChunks, flags;
        uint16_t pivotX, pivotY, shiftX, shiftY;
        is >> w >> h >> cf >> fx >> fy >> fw >> fh >> 
            flags >> nChunks >> pivotX >> pivotY >> shiftX >> shiftY;

        JSpriteFrame& frame = pSpritePack->m_Frames[i];
        frame.m_Width       = w;
        frame.m_Height      = h;
        frame.m_ColorFormat = (ColorFormat)cf;
        frame.m_Frame.x     = fx;
        frame.m_Frame.y     = fy;
        frame.m_Frame.w     = fw;
        frame.m_Frame.h     = fh;   
        frame.m_Pivot.x     = pivotX;
        frame.m_Pivot.y     = pivotY;
        frame.m_Shift.x     = shiftX;
        frame.m_Shift.y     = shiftY;
        frame.m_bHasBorder  = ((flags&FrameFlags_HasBorder) != 0);
        frame.m_pPixels     = NULL;

        if (flags&FrameFlags_Difference)
        {
            frame.m_RefFrame = 0;
            for (int j = i - 1; j >= 0; j--)
            {
                if (pSpritePack->m_Frames[j].m_RefFrame == -1)
                {
                    frame.m_RefFrame = j;
                    break;
                }
            }
        }
        else
        {
            frame.m_RefFrame = -1;
        }

        frame.m_Chunks.resize( nChunks );

        int nPixTotal = 0;
        for (int j = 0; j < nChunks; j++)
        {
            JFrameChunk& chunk = frame.m_Chunks[j];
            uint16_t cx, cy, cside, flags;
            is >> cx >> cy >> cside >> flags;
            chunk.m_SprX        = cx;
            chunk.m_SprY        = cy;
            chunk.m_Side        = cside;
            chunk.m_Flags       = flags;
            chunk.m_FrameIdx    = i;
            chunk.m_PackIdx     = packID;

            int rc = chunk.m_SprX + cside;
            int rf = frame.m_Frame.x + frame.m_Frame.w;
            int bc = chunk.m_SprY + cside;
            int bf = frame.m_Frame.y + frame.m_Frame.h;
            chunk.m_Width  = rc > rf ? cside - rc + rf : cside;
            chunk.m_Height = bc > bf ? cside - bc + bf : cside;

            int nPix = chunk.m_Width*chunk.m_Height;
            if (frame.m_bHasBorder) nPix = (chunk.m_Width + 2)*(chunk.m_Height + 2);
            nPixTotal += nPix;
        }

        frame.m_NumPixelsTotal = nPixTotal;
        frame.m_FirstByteInPack = curByte;
        curByte += nPixTotal*GetBytesPerPixel( frame.m_ColorFormat );
    }
    pSpritePack->m_HeaderSize = is.GetReadBytes();
    
    LoadPixels( packID, 0, pSpritePack->m_Frames.size(), is );

    pSpritePack->AddRef();
    AddChild( pSpritePack );

    return packID;
} // JDrawServer::GetSpriteID

void JDrawServer::LoadPixels( int packID, int sprID )
{
    if (packID < 0 || packID >= m_SpritePacks.size()) return;
    JSpritePack& sp = *(m_SpritePacks[packID]);
    
    FInStream is;
    if (!g_pFileServer->OpenMedia( sp.GetName(), "rspr", is )) return;
    is.Rewind( sp.m_HeaderSize );
    LoadPixels( packID, 0, sp.m_Frames.size(), is );
} // JDrawServer::LoadPixels

void JDrawServer::LoadPixels( int packID, int firstSpr, int nSpr, InStream& is )
{
    if (packID < 0 || packID >= m_SpritePacks.size()) return;
    JSpritePack& sp = *(m_SpritePacks[packID]);

    if (firstSpr + nSpr > sp.m_Frames.size()) 
    {
        nSpr = sp.m_Frames.size() - firstSpr - 1;
    }

    //  read pixels
    for (int i = 0; i < nSpr; i++)
    {
        JSpriteFrame& frame = sp.m_Frames[i + firstSpr];
        int nChunks = frame.m_Chunks.size();

        int srcBPP = GetBytesPerPixel( frame.m_ColorFormat );
        int dstBPP = 4;
        if (frame.m_ColorFormat == ColorFormat_ARGB4444 && !sp.m_bAlwaysCache)
        {
            dstBPP = 2;
        }
        int nBytesTotal = frame.m_NumPixelsTotal*dstBPP;

        //  allocate buffer for pixels
        uint32_t* pPixels = (uint32_t*)AllocateBuffer( nBytesTotal, sp.m_bAlwaysCache );
        assert( pPixels );

        frame.m_pPixels = pPixels;

        //  read/convert data
        if ((frame.m_ColorFormat != ColorFormat_ARGB8888 &&
            frame.m_ColorFormat != ColorFormat_ARGB4444) || sp.m_bAlwaysCache)
        {
            int nSrcBytes = frame.m_NumPixelsTotal*srcBPP;
            //  adjust working buffer
            if (nSrcBytes > 0)
            {
                if (s_UnpackBuffer.size() < nSrcBytes)
                {
                    s_UnpackBuffer.resize( nSrcBytes );
                }
                is.Read( &s_UnpackBuffer[0], nSrcBytes );
                //  convert pixels to ARGB8888
                ConvertPixels( (uint8_t*)pPixels, ColorFormat_ARGB8888, 
                    &s_UnpackBuffer[0], frame.m_ColorFormat, frame.m_NumPixelsTotal );
            }
        }
        else
        {
            is.Read( (uint8_t*)pPixels, nBytesTotal );
        }
        
        //  assign pointers in chunks to pixel data
        uint8_t* pChunkPix = (uint8_t*)pPixels;
        for (int j = 0; j < nChunks; j++)
        {
            JFrameChunk& chunk = frame.m_Chunks[j];
            int nPix = chunk.m_Width*chunk.m_Height;
            if (frame.m_bHasBorder) nPix = (chunk.m_Width + 2)*(chunk.m_Height + 2);
            chunk.m_Pixels = pChunkPix;
            pChunkPix += nPix*dstBPP;
        }
    }
}

void JDrawServer::DrawSprite( float x, float y, int spriteID, int frameID, uint32_t color, float rot, bool bMirror )
{
    DrawSprite( Frame( x, y, -1.0f, -1.0f ), spriteID, frameID, color, rot, bMirror ); 
} // JDrawServer::DrawSprite

int JDrawServer::AllocateChunk( JFrameChunk& ch )
{
    JSpritePack& pack  = *(m_SpritePacks[ch.m_PackIdx]);
    JSpriteFrame& frame = pack.m_Frames[ch.m_FrameIdx];

    ColorFormat colorFormat = frame.m_ColorFormat;
    if (colorFormat != ColorFormat_ARGB4444 || pack.m_bAlwaysCache)
    {
        colorFormat = ColorFormat_ARGB8888;
    }

    //  Call "update" callback, if needed
    //  This callback can change contents of the chunk's pixel data 
    if (pack.m_UpdateCB)
    {
        Frame ext( ch.m_SprX, ch.m_SprY, ch.m_Width, ch.m_Height );
        (pack.m_UpdateCB)( ch.m_FrameIdx, ext, (uint8_t*)ch.m_Pixels, colorFormat );
    }

    int lru      = 0;
    int side     = ch.m_Side;
    int bpp      = GetBytesPerPixel( colorFormat );
    int srcPitch = ch.m_Width*bpp;
    if (frame.m_bHasBorder) 
    {
        side += 2;
        srcPitch = (ch.m_Width + 2)*bpp;
    }
    int sPow = GetPow2( side );
    for (int i = 0; i < m_Surfaces.size(); i++)
    {
        uint16_t ax, ay;
        JSpriteSurface& surf = m_Surfaces[i];
        if (colorFormat != surf.m_ColorFormat)
        {
            continue;
        }
        if (surf.m_Layout.AllocateQuad( sPow, ax, ay ))
        {
            ch.m_CacheStamp = surf.m_DropStamp;
            
            int w = ch.m_Width;
            int h = ch.m_Height;

            ch.m_X0 = float( ch.m_SprX );
            ch.m_Y0 = float( ch.m_SprY );
            ch.m_X1 = float( ch.m_SprX + w );
            ch.m_Y1 = float( ch.m_SprY + h );

            if (frame.m_bHasBorder)
            {
                ch.m_U0 = (float( ax + 1 ))     /c_SpriteSurfaceSide;
                ch.m_V0 = (float( ay + 1 ))     /c_SpriteSurfaceSide;
                ch.m_U1 = (float( ax + 1 + w )) /c_SpriteSurfaceSide;
                ch.m_V1 = (float( ay + 1 + h )) /c_SpriteSurfaceSide;
                w += 2;
                h += 2;
            }
            else
            {
                ch.m_U0 = (float( ax ))         /c_SpriteSurfaceSide;
                ch.m_V0 = (float( ay ))         /c_SpriteSurfaceSide;
                ch.m_U1 = (float( ax + w ))     /c_SpriteSurfaceSide;
                ch.m_V1 = (float( ay + h ))     /c_SpriteSurfaceSide;
            }
            
            ch.m_SurfID = i;
            if (surf.m_TexID < 0 || !ch.m_Pixels) return -1;
            Frame rct( ax, ay, w, h );
            uint32_t pitch = 0;
            uint8_t* pDBits = g_pRenderServer->LockTexture( surf.m_TexID, rct, 0, &pitch );
            if (!pDBits) return -1;
            uint8_t* pSBits = (uint8_t*)ch.m_Pixels;
            
            for (int j = 0; j < h; j++)
            {
                memcpy( pDBits, pSBits, srcPitch );
                pDBits += pitch;
                pSBits += srcPitch;
            }
            g_pRenderServer->UnlockTexture( surf.m_TexID );
            return i;
        }
    }
    //  Could not place chunk. Drop least recently used surface
    Flush();
    if (m_Surfaces.size() == 0) return -1;
    int dropID, firstSurf, nSurf;
    
    if (colorFormat == ColorFormat_ARGB4444)
    {
        nSurf       = m_NSpriteSurfaces16;
        dropID      = rand()%m_NSpriteSurfaces16;
        firstSurf   = 0;
    }
    else
    {
        nSurf       = m_NSpriteSurfaces - m_NSpriteSurfaces16;
        dropID      = rand()%nSurf + m_NSpriteSurfaces16;
        firstSurf   = m_NSpriteSurfaces16;
    }
    
    uint32_t lastUsed = m_Surfaces[dropID].m_LastFrameUsed;
    for (int i = 0; i < nSurf; i++)
    {
        int cSurf = i + firstSurf;
        if (colorFormat != m_Surfaces[cSurf].m_ColorFormat)
        {
            continue;
        }
        if (m_Surfaces[cSurf].m_LastFrameUsed < lastUsed)
        {
            dropID   = cSurf;
            lastUsed = m_Surfaces[cSurf].m_LastFrameUsed;
        }
    }

    JSpriteSurface& dsurf = m_Surfaces[dropID];
    dsurf.Drop();
    int chunkID = AllocateChunk( ch );
	assert( chunkID != -1 );
	return chunkID;
} // JDrawServer::AllocateChunk

int JDrawServer::GetNFrames( int sprID ) const 
{
    if (sprID < 0 || sprID >= m_SpritePacks.size()) return 0;
    return m_SpritePacks[sprID]->m_Frames.size();
} // JDrawServer::GetNFrames

void JDrawServer::TileSprite( const Frame& rct, int spriteID, int frameID, uint32_t color )
{
    Frame vp = g_pRenderServer->GetViewport();
    g_pRenderServer->SetViewport( rct );
    int w = GetFrameW( spriteID, frameID );
    int h = GetFrameH( spriteID, frameID );
    if (w > 0 && h > 0) 
    {
        Flush();
        for (int cx = rct.x; cx < rct.x + rct.w; cx += w)
        {
            for (int cy = rct.y; cy < rct.y + rct.h; cy += h)
            {
                DrawSprite( cx, cy, spriteID, frameID, color );
            }
        }
        Flush();
    }
    g_pRenderServer->SetViewport( vp );
} // JDrawServer::TileSprite

void JDrawServer::DrawSprite( const Frame& rct, int spriteID, int frameID, uint32_t color, float rot, bool bMirror )
{
    if (spriteID < 0 || spriteID >= m_SpritePacks.size()) return;
    JSpritePack& sp = *(m_SpritePacks[spriteID]);
    if (frameID < 0 || frameID >= sp.m_Frames.size()) return;
    JSpriteFrame& fr = sp.m_Frames[frameID];

    if (fr.m_RefFrame != -1)
    {
        DrawSprite( rct, spriteID, fr.m_RefFrame, color, rot, bMirror );
    }

    sp.m_LastFrame = g_pRenderServer->GetCurFrame();

    float w = rct.w;
    float h = rct.h;

    if (w == -1 && h == -1) 
    {
        w = fr.m_Width;
        h = fr.m_Height;
    }

    Vec3 vscale( w/float( fr.m_Width ), h/float( fr.m_Height ), 1.0f );
    if (bMirror)
    {
        vscale.x = -vscale.x;
    }
    float cr = 1.0f;
    float sr = 0.0f;
    if (rot != 0.0f)
    {
        cr = cosf( rot );
        sr = sinf( rot );
    }
    float xc = fr.m_Pivot.x * vscale.x;
    float yc = fr.m_Pivot.y * vscale.y;

    Mat4 tm(  cr*vscale.x,   sr*vscale.x,    0.0f, 0.0f,
             -sr*vscale.y,   cr*vscale.y,    0.0f, 0.0f, 
              0.0f,          0.0f,           1.0f, 0.0f,
             -cr*xc + sr*yc + rct.x, 
             -sr*xc - cr*yc + rct.y, 0.0f, 1.0f );
    
    if (fr.m_pPixels == 0)
    {
        LoadPixels( spriteID, frameID );
        if (fr.m_pPixels == 0) 
        {
            return;
        }
    }

    //  cycle through sprite chunks
    int nCh = fr.m_Chunks.size();
    for (int i = 0; i < nCh; i++)
    {
        JFrameChunk& ch = fr.m_Chunks[i];
        if (ch.m_SurfID == -1 || 
            m_Surfaces[ch.m_SurfID].m_DropStamp != ch.m_CacheStamp)
        //  allocate chunk
        {
            ch.m_SurfID = AllocateChunk( ch );
            if (ch.m_SurfID == -1) return;
        }
        //  add chunk to render queue
        if (m_RenderBits.size() == c_MaxRenderBits) Flush();
        RenderBit& rb = m_RenderBits.expand();
        rb.m_Clr[0]         = color;
        rb.m_Clr[1]         = color;
        rb.m_Clr[2]         = color;
        rb.m_Clr[3]         = color;
        rb.m_pData          = (void*)&ch;
        rb.m_Flags          = uint32_t( ch.m_SurfID ) << 16;
        rb.m_Flags          |= rfSprite;
        rb.m_Flags          |= rfScreenSpace;
        if (m_bLinFilter)       rb.m_Flags |= rfLinearFilter;
        if (m_bZEnable)         rb.m_Flags |= rfZEnable;
        if (!m_bZWriteEnable)   rb.m_Flags |= rfZWriteDisable;
        if (m_bMod2x)           rb.m_Flags |= rfTexMod2x;
        if (m_bMod4x)           rb.m_Flags |= rfTexMod4x;
        if (m_bAdditive)        rb.m_Flags |= rfAdditive;
        if (m_bAdjustUV && !fr.m_bHasBorder) rb.m_Flags |= rfAdjustUV;
        rb.m_TM = tm;

        m_Surfaces[ch.m_SurfID].m_LastFrameUsed = m_CurFrame;
    }
    m_NDrawnSprites++;
} // JDrawServer::DrawSprite

void JDrawServer::DrawSprite( const Mat4& tm, int spriteID, int frameID, uint32_t color )
{
    if (spriteID < 0 || spriteID >= m_SpritePacks.size()) return;
    JSpritePack& sp = *(m_SpritePacks[spriteID]);
    if (frameID < 0 || frameID >= sp.m_Frames.size()) return;
    JSpriteFrame& fr = sp.m_Frames[frameID];
    sp.m_LastFrame = g_pRenderServer->GetCurFrame();

    //  cycle through sprite chunks
    int nCh = fr.m_Chunks.size();
    for (int i = 0; i < nCh; i++)
    {
        JFrameChunk& ch = fr.m_Chunks[i];
        if (ch.m_SurfID == -1 || m_Surfaces[ch.m_SurfID].m_DropStamp != ch.m_CacheStamp)
            //  allocate chunk
        {
            ch.m_SurfID = AllocateChunk( ch );
            if (ch.m_SurfID == -1) return;
        }
        //  add chunk to render queue
        if (m_RenderBits.size() == c_MaxRenderBits) Flush();
        RenderBit& rb = m_RenderBits.expand();
        rb.m_Clr[0]         = color;
        rb.m_Clr[1]         = color;
        rb.m_Clr[2]         = color;
        rb.m_Clr[3]         = color;
        rb.m_pData          = (void*)&ch;
        rb.m_TM             = tm;
        rb.m_Flags          = uint32_t( ch.m_SurfID ) << 16;
        rb.m_Flags          |= rfSprite;
        if (m_bLinFilter)       rb.m_Flags |= rfLinearFilter;
        if (m_bZEnable)         rb.m_Flags |= rfZEnable;
        if (!m_bZWriteEnable)   rb.m_Flags |= rfZWriteDisable;
        if (m_bMod2x)           rb.m_Flags |= rfTexMod2x;
        if (m_bMod4x)           rb.m_Flags |= rfTexMod4x;
        if (m_bAdditive)        rb.m_Flags |= rfAdditive;
        if (m_bAdjustUV && !fr.m_bHasBorder)    rb.m_Flags |= rfAdjustUV;
    }
    m_NDrawnSprites++;
} // JDrawServer::DrawSprite

void JDrawServer::ResetSprite( int sprID )
{
    if (sprID < 0 || sprID >= m_SpritePacks.size()) return;
    JSpritePack& sp = *(m_SpritePacks[sprID]);

    for (int i = 0; i < sp.m_Frames.size(); i++)
    {
        JSpriteFrame& fr = sp.m_Frames[i];
        for (int j = 0; j < fr.m_Chunks.size(); j++)
        {
            fr.m_Chunks[j].m_SurfID = -1;
        }
    }
} // JDrawServer::ResetSprite

int JDrawServer::GetTextWidth( int fontID, const char* str, float height, int numChars, int spacing )
{
    if (fontID < 0 || fontID >= m_SpritePacks.size() || numChars == 0) return 0;
    int nSym = strlen( str );
    if (numChars > 0 && numChars < nSym) nSym = numChars; 
    JSpritePack& sp = *(m_SpritePacks[fontID]);
    if (sp.m_Frames.size() == 0) return 0;
    float scale = (height == -1.0f) ? 1.0f : height / float( sp.m_Frames[0].m_Height );
    int x = 0;
    for (int i = 0; i < nSym; i++)
    {
        unsigned int cID = (uint8_t)str[i] - 32;
        if (cID < 0) continue;
        JSpriteFrame& f = sp.m_Frames[cID];
        float w = float( f.m_Width )*scale;
        float h = float( f.m_Height )*scale;
        x += w + spacing;
    }
    return x;
} // JDrawServer::GetTextWidth

void JDrawServer::DrawString( int x, int y, int fontID, const char* str, uint32_t color, float height, 
                              int nChar, int spacing )
{
    if (fontID < 0 || fontID >= m_SpritePacks.size())
    {
        fontID = g_pDrawServer->GetSpriteID( "font_system" );
        if (fontID < 0) return;
    }
    int nSym = strlen( str );
    if (nSym > nChar && nChar >= 0) nSym = nChar;
    JSpritePack& sp = *(m_SpritePacks[fontID]);
    if (sp.m_Frames.size() == 0) return;
    float scale = (height == -1.0f) ? 1.0f : height / float( sp.m_Frames[0].m_Height );
    for (int i = 0; i < nSym; i++)
    {
        int cID = ((unsigned char)str[i]) - 32;
        if (cID < 0 || cID >= sp.m_Frames.size())
        {
            continue;
        }
        JSpriteFrame& f = sp.m_Frames[cID];
        float w = float( f.m_Width )*scale;
        float h = float( f.m_Height )*scale;
        DrawSprite( Frame( x, y, w, h ), fontID, cID, color );
        x += w + spacing;
    }
} // JDrawServer::DrawString

void JDrawServer::DrawQuad( const Vec3& lt, const Vec3& rt, const Vec3& lb, const Vec3& rb, uint32_t color )
{
    DrawPoly( lt, rt, lb, color, color, color );
    DrawPoly( lb, rt, rb, color, color, color );
} // JDrawServer::DrawQuad

void JDrawServer::DrawBox( const AABox& box, uint32_t clrLines, uint32_t clrFill )
{
    Vec3 ltn = Vec3( box.minv.x, box.minv.y, box.maxv.z );
    Vec3 rtn = Vec3( box.minv.x, box.maxv.y, box.maxv.z );
    Vec3 lbn = Vec3( box.minv.x, box.minv.y, box.minv.z );
    Vec3 rbn = Vec3( box.minv.x, box.maxv.y, box.minv.z );
    Vec3 ltf = Vec3( box.maxv.x, box.minv.y, box.maxv.z );
    Vec3 rtf = Vec3( box.maxv.x, box.maxv.y, box.maxv.z );
    Vec3 lbf = Vec3( box.maxv.x, box.minv.y, box.minv.z );
    Vec3 rbf = Vec3( box.maxv.x, box.maxv.y, box.minv.z );

    if (clrLines != 0)
    {
        DrawLine( ltn, rtn, clrLines, clrLines );
        DrawLine( lbn, rbn, clrLines, clrLines );
        DrawLine( ltn, lbn, clrLines, clrLines );
        DrawLine( rtn, rbn, clrLines, clrLines );
                                    
        DrawLine( ltf, rtf, clrLines, clrLines );
        DrawLine( lbf, rbf, clrLines, clrLines );
        DrawLine( ltf, lbf, clrLines, clrLines );
        DrawLine( rtf, rbf, clrLines, clrLines );

        DrawLine( ltn, ltf, clrLines, clrLines );
        DrawLine( lbn, lbf, clrLines, clrLines );
        DrawLine( rtn, rtf, clrLines, clrLines );
        DrawLine( rbn, rbf, clrLines, clrLines );
    }

    if (clrFill != 0)
    {
        DrawQuad( ltn, rtn, lbn, rbn, clrFill );
        DrawQuad( rtf, ltf, rbf, lbf, clrFill );
        DrawQuad( ltf, rtf, ltn, rtn, clrFill );
        DrawQuad( rbf, lbf, rbn, lbn, clrFill );
        DrawQuad( ltf, ltn, lbf, lbn, clrFill );
        DrawQuad( rtn, rtf, rbn, rbf, clrFill );
    }
} // JDrawServer::DrawBox

void JDrawServer::DrawSphere( const Vec3& center, float r, uint32_t clrLines, uint32_t clrFill, int nSegments )
{
    float phiStep = c_DoublePI / float( nSegments  );
    float thetaStep = phiStep;
    Vec3  a, b, c, d;

    for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
    {
        for (float theta = c_PI; theta > thetaStep; theta -= thetaStep)
        {
            a.from_spherical( r, theta, phi );
            b.from_spherical( r, theta - thetaStep, phi );
            c.from_spherical( r, theta, phi + phiStep );
            d.from_spherical( r, theta - thetaStep, phi + phiStep );

            a += center;
            b += center;
            c += center;
            d += center;

            if (clrLines != 0)
            {
                DrawLine( b, d, clrLines, clrLines );
                DrawLine( c, d, clrLines, clrLines );
            }

            if (clrFill != 0)
            {
                if (theta < c_PI) DrawQuad( b, a, d, c, clrFill );
                else DrawQuad( a, b, c, d, clrFill );
            }
        }
    }
} // JDrawServer::DrawSphere

void JDrawServer::DrawSpherePart( const Vec3& center, float r, float startPhi, float dPhi, 
    float startTheta, float dTheta, uint32_t clrLines, uint32_t clrFill, int nSegments )
{
    startPhi    = DegToRad( startPhi    );
    dPhi        = DegToRad( dPhi        );
    startTheta  = DegToRad( startTheta  );
    dTheta      = DegToRad( dTheta      );

    float phiStep = c_DoublePI / float( nSegments );
    float thetaStep = phiStep;
    Vec3  a, b, c, d;
    
    float endPhi   = startPhi   + dPhi;
    float endTheta = startTheta + dTheta;
    for (float phi = startPhi; phi < endPhi; phi += phiStep)
    {
        for (float theta = endTheta; theta > startTheta; theta -= thetaStep)
        {
            float theta2 = theta - thetaStep;
            if (theta2 < startTheta) theta2 = startTheta;
            a.from_spherical( r, theta, phi );
            b.from_spherical( r, theta2, phi );
            c.from_spherical( r, theta, phi + phiStep );
            d.from_spherical( r, theta2, phi + phiStep );

            a += center;
            b += center;
            c += center;
            d += center;

            if (clrLines != 0)
            {
                DrawLine( b, d, clrLines, clrLines );
                DrawLine( c, d, clrLines, clrLines );
            }

            if (clrFill != 0)
            {
                if (theta < c_PI) DrawQuad( b, a, d, c, clrFill );
                else DrawQuad( a, b, c, d, clrFill );
            }
        }
    }
} // JDrawServer::DrawSphereSegment

void JDrawServer::DrawCylinder( const Vec3& base, float r, float h, uint32_t clrLines, uint32_t clrFill, bool bCapped, int nSegments )
{
    float phiStep = c_DoublePI / float( nSegments );
    Vec3 top( base.x, base.y, base.z + h );
    Vec3  a, b, c, d;

    for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
    {
         a = Vec3( base.x + r*cosf( phi ), base.y + r*sinf( phi ), base.z );
         b = Vec3( base.x + r*cosf( phi + phiStep ), base.y + r*sinf( phi + phiStep ), base.z );
         c = Vec3( a.x, a.y, a.z + h );
         d = Vec3( b.x, b.y, a.z + h );

         if (clrLines != 0)
         {
             DrawLine( a, b, clrLines, clrLines );
             DrawLine( b, d, clrLines, clrLines );
             DrawLine( a, c, clrLines, clrLines );
             DrawLine( c, d, clrLines, clrLines );

             if (bCapped)
             {
                DrawLine( a, base, clrLines, clrLines );
                DrawLine( c, top,  clrLines, clrLines );
             }
         }

         if (clrFill != 0)
         {
             DrawQuad( a, b, c, d, clrFill );
             if (bCapped)
             {
                DrawPoly( a, b, base, clrFill, clrFill, clrFill );
                DrawPoly( c, d, top,  clrFill, clrFill, clrFill );
             }
         }
    } 
} // JDrawServer::DrawCylinder

void JDrawServer::DrawCapsule( const Vec3& base, float r, float h, uint32_t clrLines, uint32_t clrFill, int nSegments )
{

}

void JDrawServer::DrawPlane( const Plane& plane, const Vec2& center, 
                            uint32_t clrLines, uint32_t clrFill, float side, int nSegments )
{
    
} // JDrawServer::DrawPlane

void JDrawServer::DrawTriMesh( const TriMesh2& mesh, uint32_t color, bool bScreenSpace )
{
    int nF = mesh.GetNFaces();
    for (int i = 0; i < nF; i++)
    {
        const Face& f = mesh.GetFace( i );
        const Vec2& a = mesh.GetVert( f.va );
        const Vec2& b = mesh.GetVert( f.vb );
        const Vec2& c = mesh.GetVert( f.vc );
        if (bScreenSpace)
        {
            DrawPoly( a, b, c, color, color, color ); 
        }
        else
        {
            DrawPoly(  Vec3( a.x, a.y, 0.0f ), 
                Vec3( b.x, b.y, 0.0f ), 
                Vec3( c.x, c.y, 0.0f ), color, color, color );  
        }
    }
} // DrawTriMesh

const float c_MinBendAngle = c_PI/20.0f;
void JDrawServer::DrawPolyline( float width, const PolyLine2& pl, uint32_t color, bool bClosed, bool bWorldSpace )
{
    float hw        = width*0.5f;
    float rrad      = hw;
    float begShift  = 0.0f;
    float cu        = 0.0f;           
    int   nPoints   = pl.GetNPoints();
    if (bClosed) 
    {
        nPoints++;
    }

    for (int i = 1; i < nPoints; i++)
    {
        bool bLastSegment = !bClosed && (i + 1 == nPoints);
        const Vec2& p1 = pl.GetPoint( i - 1 );
        const Vec2& p2 = pl.GetPoint( i );
        const Vec2& pn = bLastSegment ? pl.GetPoint( i ) : pl.GetPoint( i + 1 );

        Vec2 a( p2 ); 
        a -= p1;
        
        Vec2 b( pn ); 
        b -= p2;

        float da    = a.normalize();
        float db    = b.normalize();
        float dot   = a.dot( b );
        dot = clamp( dot, -1.0f, 1.0f );
        float phi   = acosf( dot );
        float theta = (c_PI - phi)*0.5f;

        Vec2 right( -a.y*hw, a.x*hw );

        Vec2 pa( p1 ), pb( p1 );
        pa += right;
        pb -= right;

        pa += a*begShift;
        pb += a*begShift;

        float segLen = da - begShift;
        begShift = rrad/tanf( theta );
        segLen -= begShift;

        Vec2 pc( pa ), pd( pb );
        pc += a*segLen;
        pd += a*segLen;

        float du = segLen/width;
        if (bWorldSpace)
        {
            DrawPoly( Vec3( pa.x, pa.y, 0.0f ), Vec3( pb.x, pb.y, 0.0f ), 
                Vec3( pc.x, pc.y, 0.0f ), color, color, color );
            DrawPoly( Vec3( pb.x, pb.y, 0.0f ), Vec3( pc.x, pc.y, 0.0f ), 
                Vec3( pd.x, pd.y, 0.0f ), color, color, color );
        }
        else
        {
            DrawPoly( pa, pb, pc, color, color, color );
            DrawPoly( pb, pc, pd, color, color, color );
        }
        cu += du;

        if (i >= nPoints || bLastSegment)
        {
            continue;
        }
        
        //  center of the rounding circle
        Vec2 cc( b ); cc -= a;
        cc.normalize();
        cc *= rrad/sinf( theta );
        cc += p2;

        float   alpha     = phi;

        Vec3 norm;
        norm.cross( Vec3( a.x, a.y, 0.0f), Vec3( b.x, b.y, 0.0f) );
        if (norm.dot( Vec3::oZ ) < 0.0f) alpha = -alpha;

        int     nSteps    = _fabs( alpha/c_MinBendAngle );
        float   dAlpha    = alpha/float( nSteps );
        float   cA        = cosf( dAlpha );
        float   sA        = sinf( dAlpha );
        float   dx        = -cA*cc.x + sA*cc.y + cc.x;
        float   dy        = -sA*cc.x - cA*cc.y + cc.y;

        float grad  = 0.0f;
        float dgrad = grad / float(nSteps);

        for (int j = 0; j < nSteps; j++)
        {
            pa = pc; pb = pd;

            float pcx = pc.x*cA - pc.y*sA + dx;
            float pcy = pc.x*sA + pc.y*cA + dy;
            pc.x = pcx; pc.y = pcy;

            float pdx = pd.x*cA - pd.y*sA + dx;
            float pdy = pd.x*sA + pd.y*cA + dy;
            pd.x = pdx; pd.y = pdy;

            float du = dAlpha*rrad/width;

            if (bWorldSpace)
            {
                DrawPoly( Vec3( pa.x, pa.y, 0.0f ), Vec3( pb.x, pb.y, 0.0f ), 
                    Vec3( pc.x, pc.y, 0.0f ), color, color, color );
                DrawPoly( Vec3( pb.x, pb.y, 0.0f ), Vec3( pc.x, pc.y, 0.0f ), 
                    Vec3( pd.x, pd.y, 0.0f ), color, color, color );
            }
            else
            {
                DrawPoly( pa, pb, pc, color, color, color );
                DrawPoly( pb, pc, pd, color, color, color );
            }

            cu += du;
        }
    }
}  

void JDrawServer::DrawPrimList( VertexS* pVert, int numVert, PrimitiveType priType )
{
    Draw( pVert, numVert, priType );
}

void JDrawServer::DrawPrimList( VertexW* pVert, int numVert, PrimitiveType priType )
{
    Draw( pVert, numVert, priType );
}

void JDrawServer::DrawFrustum( const Frustum& frustum,  uint32_t clrLines, uint32_t clrFill )
{
    DrawLine( frustum.ltn(), frustum.rtn(), clrLines, clrLines );
    DrawLine( frustum.rtn(), frustum.rbn(), clrLines, clrLines );
    DrawLine( frustum.rbn(), frustum.lbn(), clrLines, clrLines );
    DrawLine( frustum.lbn(), frustum.ltn(), clrLines, clrLines );
                                                  
    DrawLine( frustum.ltf(), frustum.rtf(), clrLines, clrLines );
    DrawLine( frustum.rtf(), frustum.rbf(), clrLines, clrLines );
    DrawLine( frustum.rbf(), frustum.lbf(), clrLines, clrLines );
    DrawLine( frustum.lbf(), frustum.ltf(), clrLines, clrLines );
                                                  
    DrawLine( frustum.ltn(), frustum.ltf(), clrLines, clrLines );
    DrawLine( frustum.rtn(), frustum.rtf(), clrLines, clrLines );
    DrawLine( frustum.rbn(), frustum.rbf(), clrLines, clrLines );
    DrawLine( frustum.lbn(), frustum.lbf(), clrLines, clrLines );

    if (clrFill != NULL)
    {
        DrawQuad( frustum.ltn(), frustum.rtn(), frustum.lbn(), frustum.rbn(), clrFill );
        DrawQuad( frustum.rtf(), frustum.ltf(), frustum.rbf(), frustum.lbf(), clrFill );
        DrawQuad( frustum.ltf(), frustum.rtf(), frustum.ltn(), frustum.rtn(), clrFill );
        DrawQuad( frustum.rbf(), frustum.lbf(), frustum.rbn(), frustum.lbn(), clrFill );
        DrawQuad( frustum.ltf(), frustum.ltn(), frustum.lbf(), frustum.lbn(), clrFill );
        DrawQuad( frustum.rtn(), frustum.rtf(), frustum.rbn(), frustum.rbf(), clrFill );
    }

} // JDrawServer::DrawFrustum








