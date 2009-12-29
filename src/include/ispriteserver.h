/*****************************************************************************/
/*	File:  ISpriteServer.h
/*****************************************************************************/
#ifndef __ISPRITESERVER_H__
#define __ISPRITESERVER_H__

enum SpriteAlignment
{
    saUnknown   = 0,
    Align_Vertical  = 1,
    saGround    = 2,
    saCamera    = 3
}; // enum SpriteAlignment

class Vec3;
class Vec2;

#include "VertexType.h"
#include "IRenderServer.h"

typedef void (*PVertProcessCB)( void* pVert, int nVert, VertexType vType );
typedef void (*PSpriteUpdateCB)( int sprID, const Frame& ext, BYTE* pPixels, ColorFormat fmt );

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

/*****************************************************************************/
/*	Class:  ISpriteServer
/*  Desc:   
/*****************************************************************************/
class ISpriteServer : public IRenderServerClient
{
public:
    virtual int     GetSpriteID    ( const char* packageName, PSpriteUpdateCB updateCB = NULL, bool bAlwaysCache = false ) = 0;
    virtual Frame   GetFrameBounds ( int sprID, int frameID ) = 0;
    virtual int     GetFrameW      ( int sprID, int frameID ) = 0;
    virtual int     GetFrameH      ( int sprID, int frameID ) = 0;
    virtual Vec2    GetFramePivot  ( int sprID, int frameID ) = 0;
    virtual Vec2    GetFrameShift  ( int sprID, int frameID ) = 0;
    virtual DWORD   GetPixel       ( int sprID, int frameID, const Vec2& pt ) const = 0;

    virtual int     GetNFrames     ( int sprID ) const = 0;
    virtual void    DrawSprite     ( float x, float y, int spriteID, int frameID, DWORD color = 0xFFFFFFFF ) = 0;
    virtual void    DrawSprite     ( const Frame& rct, int spriteID, int frameID, DWORD color = 0xFFFFFFFF, 
                                     float rot = 0.0f, bool bMirror = false ) = 0;
    virtual void    TileSprite     ( const Frame& rct, int spriteID, int frameID, DWORD color = 0xFFFFFFFF ) = 0;
    virtual void    DrawSprite     ( const Vec3& pos, float pivotX, float pivotY, float scale, 
                                        int spriteID, int frameID, 
                                        DWORD color = 0xFFFFFFFF,
                                        SpriteAlignment align = Align_Vertical ) = 0;
    virtual void    DrawString     ( int x, int y, int fontID, const char* str, DWORD color = 0xFFFFFFFF, 
                                     float height = -1.0f, int nChar = -1 ) = 0;
    virtual int     GetTextWidth   ( int fontID, const char* str, float height = -1.0f, int numChars = -1 ) = 0;

    virtual void    Flush          () = 0;
    virtual void    OnFrame        () = 0;

    virtual void    SetVertexCB    ( PVertProcessCB cb ) = 0;

    virtual void    SetLinFilter   ( bool bSet = true ) = 0;
    virtual void    SetAdjustUV    ( bool bSet = true ) = 0;
    virtual void    SetZEnable     ( bool bSet = true ) = 0;
    virtual void    SetMod2x       ( bool bSet = true ) = 0;
    virtual void    SetMod4x       ( bool bSet = true ) = 0;
    virtual void    SetAdditive    ( bool bSet = true ) = 0;

    virtual void    SetNSpriteSurf ( int numTotalSurfaces, int num16BitSurfaces = 0 ) = 0;

    virtual void    Init           () = 0;
    virtual void    ReloadSprites  () = 0;
    virtual void    ResetSprite    ( int sprID ) = 0;
    
    virtual int     GetStatistics  ( DrawSystemStat stat ) const = 0;

    virtual void    SetCacheBudget ( int numBytes ) = 0;
    virtual int     GetCacheBudget () const = 0;
}; // class ISpriteServer

extern ISpriteServer* g_pSpriteServer;

#endif // __ISPRITESERVER_H__


