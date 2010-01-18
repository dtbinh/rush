//****************************************************************************/
//  File:  MockRenderServer.h
//  Desc:  
//****************************************************************************/
#ifndef __MOCKRENDERSERVER_H__
#define __MOCKRENDERSERVER_H__

#include "mat4.h"
#include "frame.h"

#include "irenderserver.h"
//****************************************************************************/
//  Class:  MockRenderServer
//  Desc:   
//****************************************************************************/
class MockRenderServer : public IRenderServer
{
public:
                            MockRenderServer    ();
    virtual                 ~MockRenderServer   ();
    virtual bool            Shut                ();
    virtual void            Init                ();
    virtual bool            IsInited            () const { return true; }
    virtual const Frame&    GetViewport         () const { return m_Viewport; }
    virtual void            SetViewport         ( const Frame& rc );
    virtual bool            StartFrame          ();
    virtual void            EndFrame            ();
    virtual void            ClearViewport       ( uint32_t color );
    virtual void            ClearDepthStencil   ( bool bDepth, bool bStencil );
    virtual void            ShowCursor          ( bool bShow = true );

    virtual int             GetVDeclID          ( const VertexDeclaration& vdecl );

    virtual void            SetAmbient          ( uint32_t color );
    virtual int             AddPointLight       ( const Vec3& pos, float radius, float falloff, 
                                                  uint32_t diffuse, uint32_t specular, bool bPerPixel );
    virtual int             AddDirLight         ( const Vec3& dir, uint32_t diffuse, uint32_t specular, bool bPerPixel );

    virtual int             GetFPS              () const { return 60; }
    virtual uint32_t           GetCurFrame         () const { return m_CurFrame; }

    virtual void            SetViewTM           ( const Mat4& tm );
    virtual void            SetWorldTM          ( const Mat4& tm );
    virtual void            SetProjTM           ( const Mat4& tm );

    virtual const Mat4&     GetViewTM           () const { return m_ViewTM; }
    virtual const Mat4&     GetWorldTM          () const { return m_WorldTM; }
    virtual const Mat4&     GetProjTM           () const { return m_ProjTM; }

    virtual void            SetTextureTM        ( const Mat4& tm );
    virtual const Mat4&     GetTextureTM        () const { return m_TextureTM; }

    virtual void            SetColorTint        ( uint32_t color );
    
    virtual int             GetTextureID        ( const char* texName );
    virtual bool            GetTextureProp      ( int texID, TextureProperties& texProp );
    virtual void            SetTexture          ( int texID, int stage );
    virtual void            DeleteTexture       ( int texID );
    virtual int             CreateTexture       ( const TextureProperties& texProp );
    virtual uint8_t*           LockTexture         ( int texID, const Frame& rect, int level = 0, uint32_t* pStride = NULL );
    virtual bool            UnlockTexture       ( int texID, int level = 0 );
    virtual bool            CopyTexture         ( int destID, int srcID, const Frame* rct = NULL, int nRect = 0 );


    virtual int             GetShaderID         ( const char* name, bool bReload = false );
    virtual int             GetConstantID       ( int shaderID, const char* name );
    virtual bool            SetConstant         ( int shaderID, int constID, const Mat4& val );
    virtual bool            SetConstant         ( int shaderID, int constID, float x, float y, float z, float w );
    virtual bool            SetConstant         ( int shaderID, int constID, float val );

    virtual void            SetShader           ( int shID );

    virtual void            ReloadResources     ();

    virtual int             CreateRenderTarget  ( const char* texName, int w, int h, ColorFormat fmt = ColorFormat_RGB565 );
    virtual int             CreateDepthStencil  ( const char* texName, int w, int h, DepthFormat fmt = DepthFormat_D16 );
    virtual bool            SetRenderTarget     ( int texID, int dsID = -1 );
    virtual int             GetRenderTarget     ( int* pDSID = NULL ) const { return 0; }
    virtual bool            SaveTexture         ( int texID, const char* fname );

    virtual int             CreateVB            ( const char* name, int numBytes, bool bStatic = false );
    virtual int             GetVBID             ( const char* name );
    virtual uint8_t*           LockVB              ( int vbID, int firstByte, int numBytes );        
    virtual bool            UnlockVB            ( int vbID );
    virtual bool            SetVB               ( int vbID, int vdeclID, int stream );
    virtual int             ClearVB             ( int vbID );
    virtual bool            DeleteVB            ( int vbID );

    virtual int             CreateIB            ( const char* name, int numBytes, bool bStatic = false );
    virtual int             GetIBID             ( const char* name );
    virtual uint8_t*           LockIB              ( int ibID, int firstByte, int numBytes );        
    virtual bool            UnlockIB            ( int ibID );
    virtual bool            SetIB               ( int ibID, int baseIndex );
    virtual int             ClearIB             ( int ibID );
    virtual bool            DeleteIB            ( int ibID );
    
    virtual bool            Draw                ( int firstIdx, int numIdx, int firstVert, int numVert, 
                                                    PrimitiveType primType = PrimitiveType_TriangleList );

    virtual bool            CacheIB             ( int ibID, uint8_t* pData, int size, int& iteration, int& firstByte );
    virtual bool            CacheVB             ( int vbID, uint8_t* pData, int size, int stride, int& iteration, int& firstByte );
    virtual int             GetVBIteration      ( int vbID );
    virtual int             GetIBIteration      ( int ibID );

    virtual void            ResetTime           ();

    virtual void            Register            ( IRenderServerClient* pClient );
    virtual void            Unregister          ( IRenderServerClient* pClient );

private:
    Frame                   m_Viewport;
    Mat4                    m_ViewTM;
    Mat4                    m_ProjTM;
    Mat4                    m_WorldTM;
    Mat4                    m_TextureTM;
    uint32_t                m_CurFrame;          

}; // class MockRenderServer

#endif // __MOCKRENDERSERVER_H__
