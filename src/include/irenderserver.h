/*****************************************************************************/
/*	File:  IRenderServer.h
/*****************************************************************************/
#ifndef __IRENDERDEVICE_H__
#define __IRENDERDEVICE_H__

#include "VertexDeclaration.h"
#include "TextureProperties.h"

class Frame;
/*****************************************************************************/
/*	Enum:  PrimitiveType
/*  Desc:   
/*****************************************************************************/
enum PrimitiveType
{
    PrimitiveType_Unknown               = 0,
    PrimitiveType_PointList				= 1,
    PrimitiveType_LineList				= 2,
    PrimitiveType_LineStrip				= 3,
    PrimitiveType_TriangleList			= 4,
    PrimitiveType_TriangleStrip			= 5,
    PrimitiveType_TriangleFan			= 6,
    PrimitiveType_QuadList              = 7
}; // enum PrimitiveType

inline int GetNumPrimitives( PrimitiveType pType, int indNum )
{
    switch (pType)
    {
    case PrimitiveType_Unknown:			return 0;
    case PrimitiveType_TriangleList:	return indNum/3; 	
    case PrimitiveType_LineStrip:	    return indNum - 2;
    case PrimitiveType_LineList:		return indNum/2;
    case PrimitiveType_TriangleStrip:	return indNum - 1;
    case PrimitiveType_PointList:		return indNum; 
    case PrimitiveType_TriangleFan:		return indNum - 2;
    case PrimitiveType_QuadList:        return indNum/6;
    }	
    return 0;
} // GetNumPrimitives

class IRenderServerClient;

class Mat4;
class Vec4;
class Vec3;
/*****************************************************************************/
/*	Class:  IRenderServer
/*  Desc:   
/*****************************************************************************/
class IRenderServer 
{
public:
    virtual bool            IsInited        () const = 0;
    virtual const Frame&    GetViewport     () const = 0;
    virtual void            SetViewport     ( const Frame& rc ) = 0;
    virtual void            EndFrame        () = 0;
    virtual bool            StartFrame      () = 0;
    virtual void            ClearViewport   ( DWORD color ) = 0;
    virtual void            ClearDepthStencil( bool bDepth, bool bStencil ) = 0;
    virtual void            ShowCursor      ( bool bShow = true ) = 0;
    virtual int             GetFPS          () const = 0;
    virtual DWORD           GetCurFrame     () const = 0;

    virtual void            Register        ( IRenderServerClient* pClient ) = 0;
    virtual void            Unregister      ( IRenderServerClient* pClient ) = 0;

    virtual void            SetViewTM       ( const Mat4& tm ) = 0;
    virtual void            SetWorldTM      ( const Mat4& tm ) = 0;
    virtual void            SetProjTM       ( const Mat4& tm ) = 0;
    virtual void            SetTextureTM    ( const Mat4& tm ) = 0;

    virtual void            SetColorTint    ( DWORD color ) = 0;

    virtual const Mat4&     GetViewTM       () const = 0;
    virtual const Mat4&     GetWorldTM      () const = 0;
    virtual const Mat4&     GetProjTM       () const = 0;
    virtual const Mat4&     GetTextureTM    () const = 0;

    //  texture operations
    virtual int             GetTextureID    ( const char* texName ) = 0;
    virtual void            SetTexture      ( int texID, int stage ) = 0;
    virtual void            DeleteTexture   ( int texID ) = 0;
    virtual int             CreateTexture   ( const TextureProperties& texProp ) = 0;
    virtual bool            GetTextureProp  ( int texID, TextureProperties& texProp ) = 0;
    virtual BYTE*           LockTexture     ( int texID, const Frame& rect, int level = 0, DWORD* pStride = NULL ) = 0;
    virtual bool            UnlockTexture   ( int texID, int level = 0 ) = 0;
    virtual bool            CopyTexture     ( int destID, int srcID, const Frame* rct = NULL, int nRect = 0 ) = 0;

    virtual int             CreateRenderTarget  ( const char* texName, int w, int h, ColorFormat fmt = ColorFormat_RGB565 ) = 0;
    virtual int             CreateDepthStencil  ( const char* texName, int w, int h, DepthFormat fmt = DepthFormat_D16 ) = 0;
    virtual bool            SetRenderTarget     ( int texID, int dsID = -1 ) = 0;
    virtual int             GetRenderTarget     ( int* pDSID = NULL ) const = 0;
    virtual bool            SaveTexture         ( int texID, const char* fname ) = 0;
    virtual void            ReloadResources     () = 0;

    //  vertex buffer operations
    virtual int             CreateVB        ( const char* name, int numBytes, bool bStatic = false ) = 0;
    virtual int             GetVBID         ( const char* name ) = 0;
    virtual BYTE*           LockVB          ( int vbID, int firstByte, int numBytes ) = 0;        
    virtual bool            UnlockVB        ( int vbID ) = 0;
    virtual bool            SetVB           ( int vbID, int vdeclID, int stream = 0 ) = 0;
    virtual int             ClearVB         ( int vbID ) = 0;
    virtual int             GetVBIteration  ( int vbID ) = 0;
    virtual bool            DeleteVB        ( int vbID ) = 0;

    virtual int             GetVDeclID      ( const VertexDeclaration& vdecl ) = 0;

    //  index buffer operations
    virtual int             CreateIB        ( const char* name, int numBytes, bool bStatic = false ) = 0;
    virtual int             GetIBID         ( const char* name ) = 0;
    virtual BYTE*           LockIB          ( int ibID, int firstByte, int numBytes ) = 0;        
    virtual bool            UnlockIB        ( int ibID ) = 0;
    virtual bool            SetIB           ( int ibID, int baseIndex = 0 ) = 0;
    virtual int             ClearIB         ( int ibID ) = 0;
    virtual int             GetIBIteration  ( int ibID ) = 0;
    virtual bool            DeleteIB        ( int ibID ) = 0;

    virtual bool            CacheIB         ( int ibID, BYTE* pData, int size, int& iteration, int& firstByte ) = 0;
    virtual bool            CacheVB         ( int vbID, BYTE* pData, int size, int stride, int& iteration, int& firstByte ) = 0;
    
    
    virtual bool            Draw            ( int firstIdx, int numIdx, int firstVert, int numVert, 
                                                PrimitiveType primType = PrimitiveType_TriangleList ) = 0;

    //  shader operations
    virtual int             GetShaderID     ( const char* name, bool bReload = false ) = 0;
    virtual int             GetConstantID   ( int shaderID, const char* name ) = 0;
    virtual bool            SetConstant     ( int shaderID, int constID, const Mat4& val ) = 0;
    virtual bool            SetConstant     ( int shaderID, int constID, float x, float y, float z, float w ) = 0;
    virtual bool            SetConstant     ( int shaderID, int constID, float val ) = 0;

    virtual void            SetShader       ( int shID ) = 0;

    virtual void            SetAmbient      ( DWORD color ) = 0;
    virtual int             AddPointLight   ( const Vec3& pos, float radius, float falloff, 
                                               DWORD diffuse, DWORD specular, bool bPerPixel ) = 0;
    virtual int             AddDirLight     ( const Vec3& dir, DWORD diffuse, DWORD specular, bool bPerPixel ) = 0;
    
    virtual void*           GetRenderSurface() { return NULL; }
    virtual bool            ResizeBackBuffer( int w, int h ) { return false; }

}; // class IRenderServer

extern IRenderServer* g_pRenderServer;

/*****************************************************************************/
/*	Class:  IRenderServerClient
/*  Desc:   interface for receiver of the device notifications
/*****************************************************************************/
class IRenderServerClient
{
public:

    virtual ~IRenderServerClient()
    {
        if (g_pRenderServer) g_pRenderServer->Unregister( this );
    }

    virtual void OnFrame()       {}
    virtual void OnResetDevice() {}
}; // class IRenderServerClient

#endif // __IRenderServer_H__


