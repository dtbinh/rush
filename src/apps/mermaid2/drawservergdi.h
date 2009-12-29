#ifndef __DRAWSERVERGDI_H__
#define __DRAWSERVERGDI_H__

#include "IDrawServer.h"


struct BitmapFrame
{
    HBITMAP     m_hBitmap;
    HDC         m_hDC;
    int         m_Width;
    int         m_Height;
};

struct BitmapPackage
{
    std::vector<BitmapFrame>    m_Frames;
    JString                     m_Name;
};

class DrawServerGDI : public JObject, public IDrawServer
{
public:
    DrawServerGDI();

    //  geometric primitives drawing
    virtual void    DrawRect       ( const Frame& rc, DWORD color ) {}
    virtual void    DrawFrame      ( const Frame& rc, DWORD color ) {}
    virtual void    DrawLine       ( int x1, int y1, int x2, int y2, DWORD ca, DWORD cb ) {}
    virtual void    DrawLine       ( const Vec3& a, const Vec3& b, DWORD ca, DWORD cb ) {}
    virtual void    DrawQuad       ( const Vec3& lt, const Vec3& rt, const Vec3& lb, const Vec3& rb, DWORD color ) {}

    virtual void    DrawPoly       ( const Vec3& a, const Vec3& b, const Vec3& c, DWORD ca, DWORD cb, DWORD cc ) {}
    virtual void    DrawPoly       ( const Vec2& a, const Vec2& b, const Vec2& c, DWORD ca, DWORD cb, DWORD cc ) {}

    virtual void    DrawBox        ( const AABox& box, DWORD clrLines, DWORD clrFill ) {}
    virtual void    DrawSphere     ( const Vec3& center, float r, DWORD clrLines, DWORD clrFill, 
                                        int nSegments = c_NDefaultSegments ) {}
    virtual void    DrawSpherePart ( const Vec3& center, float r, float startPhi, float dPhi, float startTheta, float dTheta, 
                                        DWORD clrLines, DWORD clrFill, int nSegments = c_NDefaultSegments ) {}
    virtual void    DrawCylinder   ( const Vec3& base, float r, float h, 
                                        DWORD clrLines, DWORD clrFill, bool bCapped = true, 
                                        int nSegments = c_NDefaultSegments ) {}
    virtual void    DrawCapsule    ( const Vec3& base, float r, float h, 
                                        DWORD clrLines, DWORD clrFill, 
                                        int nSegments = c_NDefaultSegments ) {}
    virtual void    DrawPlane      ( const Plane& plane, const Vec2& center, 
                                        DWORD clrLines, DWORD clrFill, float side,
                                        int nSegments = c_NDefaultSegments ) {}
    
    virtual void    DrawFrustum    ( const Frustum& frustum,  DWORD clrLines, DWORD clrFill ) {}

    virtual void    DrawPolyline   ( float width, const PolyLine2& pl, DWORD color, bool bClosed, bool bWorldSpace ) {}
    virtual void    DrawTriMesh    ( const TriMesh2& mesh, DWORD color, bool bScreenSpace = false ) {}

    virtual void    DrawPrimList   ( VertexS* pVert, int numVert, PrimitiveType priType ) {}
    virtual void    DrawPrimList   ( VertexW* pVert, int numVert, PrimitiveType priType ) {}


    //  sprite drawing
    virtual int     GetSpriteID    ( const char* packageName, PSpriteUpdateCB updateCB = NULL, bool bAlwaysCache = false );
    virtual Frame   GetFrameBounds ( int sprID, int frameID );
    virtual int     GetFrameW      ( int sprID, int frameID );
    virtual int     GetFrameH      ( int sprID, int frameID );
    virtual Vec2    GetFramePivot  ( int sprID, int frameID );
    virtual Vec2    GetFrameShift  ( int sprID, int frameID );

    virtual DWORD   GetPixel       ( int sprID, int frameID, const Vec2& pt ) const;


    virtual int     GetNFrames     ( int sprID ) const;

    virtual void    DrawSprite     ( float x, float y, int spriteID, int frameID, DWORD color = 0xFFFFFFFF,
                                     float rot = 0.0f, bool bMirror = false );
    virtual void    DrawSprite     ( const Frame& rct, int spriteID, int frameID, DWORD color = 0xFFFFFFFF, 
                                     float rot = 0.0f, bool bMirror = false );
    
                                     virtual void    TileSprite     ( const Frame& rct, int spriteID, int frameID, DWORD color = 0xFFFFFFFF ) {}
    virtual void    DrawSprite     ( const Mat4& tm, int spriteID, int frameID, DWORD color = 0xFFFFFFFF ) {}
    virtual void    DrawString     ( int x, int y, int fontID, const char* str, DWORD color = 0xFFFFFFFF, 
                                     float height = -1.0f, int nChar = -1 ) {}
    virtual int     GetTextWidth   ( int fontID, const char* str, float height = -1.0f, int numChars = -1 ) { return 0; }

    virtual void    Flush          () {}
    virtual void    OnFrame        () {}

    virtual void    Render         ();

    virtual void    SetVertexCB    ( PVertProcessCB cb ) {}

    virtual void    SetLinFilter   ( bool bSet = true ) {}
    virtual void    SetAdjustUV    ( bool bSet = true ) {}
    virtual void    SetZEnable     ( bool bSet = true ) {}
    virtual void    SetZWriteEnable( bool bSet = true ) {}
    virtual void    SetMod2x       ( bool bSet = true ) {}
    virtual void    SetMod4x       ( bool bSet = true ) {}
    virtual void    SetAdditive    ( bool bSet = true ) {}

    virtual void    SetWorldTM     ( const Mat4& tm ) {}
    virtual void    SetNSpriteSurf ( int numTotalSurfaces, int num16BitSurfaces = 0 ) {}

    virtual void    Init           ();

    virtual void    ReloadSprites  () {}
    virtual void    ResetSprite    ( int sprID ) {}
    
    virtual int     GetStatistics  ( DrawSystemStat stat ) const { return 0; }

    virtual void    SetSpriteCacheBudget( int numBytes ) {}
    virtual int     GetSpriteCacheBudget() const { return 0; }

    expose(DrawServerGDI)
    {
        parent(JObject); 
    }

private:

    std::vector<BitmapPackage>  m_Packages;

    HDC                         m_hDC;
    HDC                         m_hBackDC;
    HBITMAP                     m_hBackBitmap;

    int                         m_BackW;
    int                         m_BackH;


}; // class DrawServerGDI

#endif // __DRAWSERVERGDI_H__