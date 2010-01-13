/*****************************************************************************/
/*	File:  IDrawServer.h
/*****************************************************************************/
#ifndef __IDRAWSYSTEM_H__
#define __IDRAWSYSTEM_H__

class Vec3;
class Vec2;
class Mat4;
class PolyLine2;
class TriMesh2;
class AABox;
class Frame;
class Plane;
class Frustum;

#include "vertextype.h"
#include "irenderserver.h"

typedef void (*PVertProcessCB)( void* pVert, int nVert, VertexType vType );
typedef void (*PSpriteUpdateCB)( int sprID, const Frame& ext, uint8_t* pPixels, ColorFormat fmt );

/*****************************************************************************/
/*	Enum:  DrawSystemStat
/*  Desc:  Draw system statistics type
/*****************************************************************************/
enum DrawSystemStat
{
    DSStat_SpriteCacheSize  = 1,
    DSStat_NumSpritesDrawn  = 2,
    DSStat_NumFlushes       = 3,
}; // enum DrawSystemStat

class JModel;
class JModelAnim;

const int c_NDefaultSegments = 16;
/*****************************************************************************/
/*	Class:  IDrawServer
/*  Desc:   
/*****************************************************************************/
class IDrawServer : public IRenderServerClient
{
public:
    //  geometric primitives drawing
    virtual void    DrawRect       ( const Frame& rc, uint32_t color ) = 0;
    virtual void    DrawFrame      ( const Frame& rc, uint32_t color ) = 0;
    virtual void    DrawLine       ( int x1, int y1, int x2, int y2, uint32_t ca, uint32_t cb ) = 0;
    virtual void    DrawLine       ( const Vec3& a, const Vec3& b, uint32_t ca, uint32_t cb ) = 0;
    virtual void    DrawQuad       ( const Vec3& lt, const Vec3& rt, const Vec3& lb, const Vec3& rb, uint32_t color ) = 0;

    virtual void    DrawPoly       ( const Vec3& a, const Vec3& b, const Vec3& c, uint32_t ca, uint32_t cb, uint32_t cc ) = 0;
    virtual void    DrawPoly       ( const Vec2& a, const Vec2& b, const Vec2& c, uint32_t ca, uint32_t cb, uint32_t cc ) = 0;

    virtual void    DrawBox        ( const AABox& box, uint32_t clrLines, uint32_t clrFill ) = 0;
    virtual void    DrawSphere     ( const Vec3& center, float r, uint32_t clrLines, uint32_t clrFill, 
                                        int nSegments = c_NDefaultSegments ) = 0;
    virtual void    DrawSpherePart ( const Vec3& center, float r, float startPhi, float dPhi, float startTheta, float dTheta, 
                                        uint32_t clrLines, uint32_t clrFill, int nSegments = c_NDefaultSegments ) = 0;
    virtual void    DrawCylinder   ( const Vec3& base, float r, float h, 
                                        uint32_t clrLines, uint32_t clrFill, bool bCapped = true, 
                                        int nSegments = c_NDefaultSegments ) = 0;
    virtual void    DrawCapsule    ( const Vec3& base, float r, float h, 
                                        uint32_t clrLines, uint32_t clrFill, 
                                        int nSegments = c_NDefaultSegments ) = 0;
    virtual void    DrawPlane      ( const Plane& plane, const Vec2& center, 
                                        uint32_t clrLines, uint32_t clrFill, float side,
                                        int nSegments = c_NDefaultSegments ) = 0;
    
    virtual void    DrawFrustum    ( const Frustum& frustum,  uint32_t clrLines, uint32_t clrFill ) = 0;

    virtual void    DrawPolyline   ( float width, const PolyLine2& pl, uint32_t color, bool bClosed, bool bWorldSpace ) = 0;
    virtual void    DrawTriMesh    ( const TriMesh2& mesh, uint32_t color, bool bScreenSpace = false ) = 0;

    virtual void    DrawPrimList   ( VertexS* pVert, int numVert, PrimitiveType priType ) = 0;
    virtual void    DrawPrimList   ( VertexW* pVert, int numVert, PrimitiveType priType ) = 0;


    //  sprite drawing
    virtual int     GetSpriteID    ( const char* packageName, PSpriteUpdateCB updateCB = NULL, bool bAlwaysCache = false ) = 0;
    virtual Frame   GetFrameBounds ( int sprID, int frameID ) = 0;
    virtual int     GetFrameW      ( int sprID, int frameID ) = 0;
    virtual int     GetFrameH      ( int sprID, int frameID ) = 0;
    virtual Vec2    GetFramePivot  ( int sprID, int frameID ) = 0;
    virtual Vec2    GetFrameShift  ( int sprID, int frameID ) = 0;

    virtual uint32_t   GetPixel       ( int sprID, int frameID, const Vec2& pt ) const = 0;


    virtual int     GetNFrames     ( int sprID ) const = 0;
    virtual void    DrawSprite     ( float x, float y, int spriteID, int frameID, uint32_t color = 0xFFFFFFFF, 
                                     float rot = 0.0f, bool bMirror = false ) = 0;
    virtual void    DrawSprite     ( const Frame& rct, int spriteID, int frameID, uint32_t color = 0xFFFFFFFF, 
                                     float rot = 0.0f, bool bMirror = false ) = 0;
    virtual void    TileSprite     ( const Frame& rct, int spriteID, int frameID, uint32_t color = 0xFFFFFFFF ) = 0;
    virtual void    DrawSprite     ( const Mat4& tm, int spriteID, int frameID, uint32_t color = 0xFFFFFFFF ) = 0;
    virtual void    DrawString     ( int x, int y, int fontID, const char* str, uint32_t color = 0xFFFFFFFF, 
                                     float height = -1.0f, int nChar = -1, int spacing = 0 ) = 0;
    virtual int     GetTextWidth   ( int fontID, const char* str, float height = -1.0f, 
                                     int numChars = -1, int spacing = 0 ) = 0;

    virtual void    Flush          () = 0;
    virtual void    OnFrame        () = 0;

    virtual void    SetVertexCB    ( PVertProcessCB cb ) = 0;

    virtual void    SetLinFilter   ( bool bSet = true ) = 0;
    virtual void    SetAdjustUV    ( bool bSet = true ) = 0;
    virtual void    SetZEnable     ( bool bSet = true ) = 0;
    virtual void    SetZWriteEnable( bool bSet = true ) = 0;
    virtual void    SetMod2x       ( bool bSet = true ) = 0;
    virtual void    SetMod4x       ( bool bSet = true ) = 0;
    virtual void    SetAdditive    ( bool bSet = true ) = 0;

    virtual void    SetWorldTM     ( const Mat4& tm ) = 0;
    virtual void    SetNSpriteSurf ( int numTotalSurfaces, int num16BitSurfaces = 0 ) = 0;

    virtual void    Init           () = 0;

    virtual void    ReloadSprites  () = 0;
    virtual void    ResetSprite    ( int sprID ) = 0;
    
    virtual int     GetStatistics  ( DrawSystemStat stat ) const = 0;

    virtual void    SetSpriteCacheBudget( int numBytes ) = 0;
    virtual int     GetSpriteCacheBudget() const = 0;
}; // class IDrawServer

extern IDrawServer* g_pDrawServer;

#endif // __IDRAWSYSTEM_H__



