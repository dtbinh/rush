//****************************************************************************/
//  File:  JDrawServer.h
//  Desc:
//****************************************************************************/
#ifndef __JDRAWSERVER_H__
#define __JDRAWSERVER_H__
#include <vector>
#include "staticarray.h"
#include "color.h"
#include "quadpacker.h"
#include "mat4.h"
#include "jspritesurface.h"

//****************************************************************************/
//  Enum:  RenderModeFlags
//****************************************************************************/
enum RenderModeFlags
{
    rfScreenSpace   = 0x0001,
    rfSprite        = 0x0002,
    rfQuad          = 0x0004,
    rfLine          = 0x0008,
    rfPoly          = 0x0010,
    rfLinearFilter  = 0x0020,
    rfTexMod2x      = 0x0040,
    rfTexMod4x      = 0x0080,
    rfAdditive      = 0x0100,
    rfZEnable       = 0x0200,
    rfAdjustUV      = 0x0400,
    rfZWriteDisable = 0x0800,

}; // enum RenderModeFlags

//****************************************************************************/
//  Class:  RenderBit
//  Desc:   Atomic rendering task - corresponds to rendering single
/*          sprite chunk, or line, or quad
//****************************************************************************/
struct RenderBit
{
    void*           m_pData;
    uint32_t           m_Clr[4];
    uint32_t           m_Flags;
    Mat4            m_TM;

    inline bool operator <( const RenderBit& r ) const
    {
        if (m_Flags < r.m_Flags) return true;
        return false;
    }

}; // struct RenderBit

const int c_DynamicVBSize = 2048*1024;
const int c_MaxRenderBits = 2048;
typedef static_array<RenderBit,  c_MaxRenderBits> RenderBitsArray;
typedef static_array<RenderBit*, c_MaxRenderBits> PRenderBitsArray;

class JSpritePack;
struct JFrameChunk;
//****************************************************************************/
//  Class:  JDrawServer
//  Desc:
//****************************************************************************/
class JDrawServer : public JObject, public IDrawServer
{
public:
                JDrawServer     ();
    virtual     ~JDrawServer    ();
    void        DrawRect        ( const Frame& rc, uint32_t color );
    void        DrawFrame       ( const Frame& rc, uint32_t color );
    void        DrawLine        ( int x1, int y1, int x2, int y2, uint32_t ca, uint32_t cb );
    void        DrawLine        ( const Vec3& a, const Vec3& b, uint32_t ca, uint32_t cb );
    void        DrawQuad        ( const Vec3& lt, const Vec3& rt, const Vec3& lb, const Vec3& rb, uint32_t color );

    void        DrawPoly        ( const Vec3& a, const Vec3& b, const Vec3& c, uint32_t ca, uint32_t cb, uint32_t cc );
    void        DrawPoly        ( const Vec2& a, const Vec2& b, const Vec2& c, uint32_t ca, uint32_t cb, uint32_t cc );

    void        DrawFrustum     ( const Frustum& frustum,  uint32_t clrLines, uint32_t clrFill );

    void        Init            ();
    void        OnResetDevice   ();
    void        OnFrame         ();

    void        SetWorldTM      ( const Mat4& tm ) { m_WorldTM = tm; }

    void        SetLinFilter    ( bool bSet = true ) { m_bLinFilter = bSet; }
    void        SetAdjustUV     ( bool bSet = true ) { m_bAdjustUV = bSet; }
    void        SetZEnable      ( bool bSet = true ) { m_bZEnable = bSet; }
    void        SetZWriteEnable ( bool bSet = true ) { m_bZWriteEnable = bSet; }
    void        SetMod2x        ( bool bSet = true ) { m_bMod2x = bSet; }
    void        SetMod4x        ( bool bSet = true ) { m_bMod4x = bSet; }
    void        SetAdditive     ( bool bSet = true ) { m_bAdditive = bSet; }

    void        SetNSpriteSurf  ( int numTotalSurfaces, int num16BitSurfaces = 0 );
    int         GetSpriteID     ( const char* packageName, PSpriteUpdateCB updateCB = NULL, bool bAlwaysCache = false );
    int         GetNFrames      ( int sprID ) const;
    void        DrawSprite      ( float x, float y, int spriteID, int frameID, uint32_t color = 0xFFFFFFFF, float rot = 0.0f, bool bMirror = false );
    void        DrawSprite      ( const Frame& rct, int spriteID, int frameID, uint32_t color = 0xFFFFFFFF,
                                  float rot = 0.0f, bool bMirror = false );
    void        TileSprite      ( const Frame& rct, int spriteID, int frameID, uint32_t color = 0xFFFFFFFF );
    void        DrawSprite      ( const Mat4& tm, int spriteID, int frameID, uint32_t color = 0xFFFFFFFF );
    void        DrawString      ( int x, int y, int fontID, const char* str, uint32_t color = 0xFFFFFFFF,
                                    float height = -1.0f, int nChar = -1, int spacing = 0 );
    int         GetTextWidth    ( int fontID, const char* str, float height = -1.0f, int numChars = -1, int spacing = 0 );

    int         GetFrameW       ( int sprID, int frameID );
    int         GetFrameH       ( int sprID, int frameID );
    Vec2        GetFramePivot   ( int sprID, int frameID );
    Vec2        GetFrameShift   ( int sprID, int frameID );
    uint32_t       GetPixel        ( int sprID, int frameID, const Vec2& pt ) const;
    Frame       GetFrameBounds  ( int sprID, int frameID );

    int         GetStatistics   ( DrawSystemStat stat ) const;

    void        ReloadSprites   ();
    void        ResetSprite     ( int sprID );
    void        SetVertexCB     ( PVertProcessCB cb ) { VertProcessCB = cb; }

    void        DrawBox         ( const AABox& box, uint32_t clrLines, uint32_t clrFill );
    void        DrawSphere      ( const Vec3& center, float r,  uint32_t clrLines, uint32_t clrFill,
                                     int nSegments = c_NDefaultSegments );
    void        DrawSpherePart  ( const Vec3& center, float r, float startPhi, float dPhi, float startTheta, float dTheta,
                                     uint32_t clrLines, uint32_t clrFill, int nSegments = c_NDefaultSegments );

    void        DrawCylinder    ( const Vec3& base, float r, float h, uint32_t clrLines, uint32_t clrFill, bool bCapped = true,
                                     int nSegments = c_NDefaultSegments );
    void        DrawCapsule     ( const Vec3& base, float r, float h, uint32_t clrLines, uint32_t clrFill,
                                     int nSegments = c_NDefaultSegments );
    void        DrawPlane       ( const Plane& plane, const Vec2& center, uint32_t clrLines, uint32_t clrFill, float side,
                                    int nSegments = c_NDefaultSegments );

    void        DrawPolyline    ( float width, const PolyLine2& pl, uint32_t color, bool bClosed, bool bWorldSpace );
    void        DrawTriMesh     ( const TriMesh2& mesh, uint32_t color, bool bScreenSpace = false );

    void        DrawPrimList    ( VertexS* pVert, int numVert, PrimitiveType priType );
    void        DrawPrimList    ( VertexW* pVert, int numVert, PrimitiveType priType );

    void        SetSpriteCacheBudget( int numBytes ) { m_MemoryBudget = numBytes; }
    int         GetSpriteCacheBudget() const { return m_MemoryBudget; }

    expose(JDrawServer)
    {
        parent(JObject);
    }

private:
    int         AllocateChunk   ( JFrameChunk& ch );
    void        Flush           ();
    void        SetupRS         ( uint32_t flags );
    void        LoadPixels      ( int packID, int firstSpr, int nSpr, InStream& is );
    void        LoadPixels      ( int packID, int sprID );

    template <class TVert>
    void Draw( TVert* pVert, int numVert, PrimitiveType priType )
    {
        if (VertProcessCB) VertProcessCB( (void*)pVert, numVert, TVert::Type );
        const int stride = sizeof( TVert );
        g_pRenderServer->SetVB( m_PrimVBID, TVert::Type, 0 );
        int numBytes = stride*numVert;
        int iter = -1;
        g_pRenderServer->CacheVB( m_PrimVBID, (uint8_t*)pVert, numBytes, stride, iter, m_CurVBPos );
        g_pRenderServer->Draw( 0, 0, m_CurVBPos/stride, numVert, priType );
        m_CurVBPos += numBytes;
    }

    uint8_t*       AllocateBuffer   ( int nBytes, bool bAlwaysCache );
    void        Cleanup          ();

    int                         m_NSpriteSurfaces;      //  total number of the sprite surfaces
    int                         m_NSpriteSurfaces16;    //  number of 16-bit sprite surfaces
    std::vector<JSpritePack*>   m_SpritePacks;
    std::vector<JSpriteSurface> m_Surfaces;

    RenderBitsArray             m_RenderBits;
    PRenderBitsArray            m_SortedRenderBits;

    uint32_t                       m_CurFrame;

    static uint16_t                 s_QuadIdx[c_MaxVertDrawn*6/4];
    static std::vector<uint8_t>    s_UnpackBuffer;

    uint8_t*                       m_pBuffer;
    int                         m_CurBufferByte;

    bool                        m_bLinFilter;
    bool                        m_bZEnable;
    bool                        m_bZWriteEnable;
    bool                        m_bMod2x;
    bool                        m_bMod4x;
    bool                        m_bAdditive;
    bool                        m_bAdjustUV;

    Frame                       m_Viewport;
    Mat4                        m_WorldTM;

    int                         m_PrimVBID;

    int                         m_shBase;
    int                         m_shAdd;
    int                         m_shZEnable;
    int                         m_shZWriteDisable;
    int                         m_shLinf;
    int                         m_shMod2x;
    int                         m_shMod4x;
    int                         m_shNoTex;

    int                         m_MemoryBudget;
    int                         m_CurBytesAllocated;
    int                         m_NDrawnSprites;
    int                         m_NFlushes;

    int                         m_CurVBPos;
    PVertProcessCB              VertProcessCB;
}; // class JDrawServer

#endif // __JDRAWSERVER_H__
