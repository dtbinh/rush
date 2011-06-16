//****************************************************************************/
//  File:  JRenderServer.h
//  Desc:  
//****************************************************************************/
#ifndef __JRENDERSERVER8_H__
#define __JRENDERSERVER8_H__

#include "IRenderServer.h"
#include "Mat3.h"
#include "Mat4.h"

#include "Shader.h"

typedef IDirect3D8                 DXAPI;
typedef IDirect3DDevice8           DXDevice;
typedef IDirect3DSurface8          DXSurface;
typedef IDirect3DTexture8          DXTexture;
typedef IDirect3DBaseTexture8      DXBaseTexture;

struct TextureFile
{
    DXBaseTexture*      m_pTexture;
    DXSurface*          m_pDepthStencil;
    TextureProperties   m_Prop;
    std::string         m_Name;

    TextureFile() : m_pTexture( NULL ), m_pDepthStencil( NULL ) {}
}; // struct TextureFile

struct VDeclInstance
{
    VertexDeclaration   m_VDecl;
    DWORD               m_FVF;
}; // struct VDeclInstance


struct VBInstance
{
    IDirect3DVertexBuffer8* m_pBuffer;
    std::string             m_Name;
    int                     m_CurIteration;
    int                     m_CurLastByte;
    bool                    m_bDynamic;
    int                     m_Size;

    VBInstance() : m_pBuffer(NULL), m_CurIteration(0), m_Size(0), m_CurLastByte(0) {}
}; // struct VBInstance

struct IBInstance
{
    IDirect3DIndexBuffer8*  m_pBuffer;
    std::string             m_Name;
    int                     m_CurIteration;
    int                     m_CurLastByte;
    bool                    m_bDynamic;
    int                     m_Size;

    IBInstance() : m_pBuffer(NULL), m_CurIteration(0), m_Size(0), m_CurLastByte(0) {}
}; // struct IBInstance

const int c_QuadIBSize = 65536*2*6;
//****************************************************************************/
//  Class:  JRenderServer
//  Desc:   
//****************************************************************************/
class JRenderServer : public JObject, public IRenderServer
{
    Frame                            m_Viewport;

    DXAPI*                          m_pD3D;
    DXDevice*                       m_pDevice;
    HWND                            m_hWnd;
    DEVMODE                         m_dmDesktop;
    DWORD                           m_CurFrame;
    bool                            m_bInited;
    float                           m_FPS;
    bool                            m_bCursorVisible;
    bool                            m_bWireFrame;

    Mat4                            m_ViewTM;
    Mat4                            m_ProjTM;
    Mat4                            m_WorldTM;
    Mat4                            m_TextureTM;

    int                             m_RTID;
    int                             m_DSID;

    DXSurface*                      m_pBackBuffer;
    DXSurface*                      m_pDepthStencil;

    std::vector<TextureFile>        m_Textures;
    std::vector<Shader*>            m_Shaders;
    std::vector<VDeclInstance>      m_VertexDeclarations;
    std::vector<VBInstance>         m_VBuffers;
    std::vector<IBInstance>         m_IBuffers;

    std::vector<IRenderServerClient*> m_Clients;

    DWORD                           m_CurFVF;
    int                             m_QuadIB;
    
    DWORD                           m_Ambience;
    bool                            m_bHasStencil;
    bool                            m_bDeviceLost;
    D3DPRESENT_PARAMETERS           m_PresParam;

public:
                            JRenderServer       ();
    virtual                 ~JRenderServer      ();
    virtual void            Init            ();
    virtual bool            IsInited        () const { return m_bInited; }
    virtual bool            Shut            ();
    virtual const Frame&    GetViewport     () const { return m_Viewport; }
    virtual void            SetViewport     ( const Frame& rc );
    virtual bool            StartFrame      ();
    virtual void            EndFrame        ();
    virtual void            ClearViewport   ( DWORD color );
    virtual void            ClearDepthStencil( bool bDepth, bool bStencil );
    virtual void            ShowCursor      ( bool bShow = true );

    virtual int             GetVDeclID      ( const VertexDeclaration& vdecl );

    virtual void            Register        ( IRenderServerClient* pClient );
    virtual void            Unregister      ( IRenderServerClient* pClient );

    virtual void            SetAmbient      ( DWORD color );
    virtual int             AddPointLight   ( const Vec3& pos, float radius, float falloff, 
                                              DWORD diffuse, DWORD specular, bool bPerPixel );
    virtual int             AddDirLight     ( const Vec3& dir, DWORD diffuse, DWORD specular, bool bPerPixel );

    virtual int             GetFPS          () const { return (int)m_FPS; }
    virtual DWORD           GetCurFrame     () const { return m_CurFrame; }

    virtual void            SetViewTM       ( const Mat4& tm );
    virtual void            SetWorldTM      ( const Mat4& tm );
    virtual void            SetProjTM       ( const Mat4& tm );

    virtual const Mat4&     GetViewTM       () const { return m_ViewTM; }
    virtual const Mat4&     GetWorldTM      () const { return m_WorldTM; }
    virtual const Mat4&     GetProjTM       () const { return m_ProjTM; }

    virtual void            SetTextureTM    ( const Mat4& tm );
    virtual const Mat4&     GetTextureTM    () const { return m_TextureTM; }

    virtual void            SetColorTint    ( DWORD color );
    
    virtual int             GetTextureID    ( const char* texName );
    virtual bool            GetTextureProp  ( int texID, TextureProperties& texProp );
    virtual void            SetTexture      ( int texID, int stage );
    virtual void            DeleteTexture   ( int texID );
    virtual int             CreateTexture   ( const TextureProperties& texProp );
    virtual BYTE*           LockTexture     ( int texID, const Frame& rect, int level = 0, DWORD* pStride = NULL );
    virtual bool            UnlockTexture   ( int texID, int level = 0 );
    virtual bool            CopyTexture     ( int destID, int srcID, const Frame* rct = NULL, int nRect = 0 );


    virtual int             GetShaderID     ( const char* name, bool bReload = false );
    virtual int             GetConstantID   ( int shaderID, const char* name ) { return -1; }
    virtual bool            SetConstant     ( int shaderID, int constID, const Mat4& val ) { return false; }
    virtual bool            SetConstant     ( int shaderID, int constID, float x, float y, float z, float w ) { return false; }
    virtual bool            SetConstant     ( int shaderID, int constID, float val ) { return false; }

    virtual void            SetShader       ( int shID );
    virtual void            ReloadResources () {}

    virtual int             CreateRenderTarget  ( const char* texName, int w, int h, ColorFormat fmt = ColorFormat_RGB565 );
    virtual int             CreateDepthStencil  ( const char* texName, int w, int h, DepthFormat fmt = DepthFormat_D16 );
    virtual bool            SetRenderTarget     ( int texID, int dsID = -1 );
    virtual int             GetRenderTarget     ( int* pDSID = NULL ) const { if (pDSID) *pDSID = m_DSID; return m_RTID; }
    virtual bool            SaveTexture         ( int texID, const char* fname );

    virtual int             CreateVB        ( const char* name, int numBytes, bool bStatic = false );
    virtual int             GetVBID         ( const char* name );
    virtual BYTE*           LockVB          ( int vbID, int firstByte, int numBytes );        
    virtual bool            UnlockVB        ( int vbID );
    virtual bool            SetVB           ( int vbID, int vdeclID, int stream );
    virtual int             ClearVB         ( int vbID );
    virtual bool            DeleteVB        ( int vbID );

    virtual int             CreateIB        ( const char* name, int numBytes, bool bStatic = false );
    virtual int             GetIBID         ( const char* name );
    virtual BYTE*           LockIB          ( int ibID, int firstByte, int numBytes );        
    virtual bool            UnlockIB        ( int ibID );
    virtual bool            SetIB           ( int ibID, int baseIndex );
    virtual int             ClearIB         ( int ibID );
    virtual bool            DeleteIB        ( int ibID );

    virtual bool            CacheIB         ( int ibID, BYTE* pData, int size, int& iteration, int& firstByte );
    virtual bool            CacheVB         ( int vbID, BYTE* pData, int size, int stride, int& iteration, int& firstByte );

    virtual int             GetIBIteration  ( int ibID );
    virtual int             GetVBIteration  ( int vbID );
    
    virtual bool            Draw            ( int firstIdx, int numIdx, int firstVert, int numVert, 
                                                PrimitiveType primType = PrimitiveType_TriangleList );

    bool                    IsFullScreen    () const;
    void                    SetFullScreen   ( bool bFullScreen );

    bool                    IsWireFrame     () const { return m_bWireFrame; }
    void                    SetWireFrame    ( bool bWireFrame );

    friend IDirect3DDevice8* GetDirect3DDevice8();

    static JRenderServer*       s_pInstance;

    expose(JRenderServer)
    {
        parent(JObject);
        rprop( "FPS", GetFPS );

        prop( "fullscreen", IsFullScreen, SetFullScreen );
        prop( "WireFrame",  IsWireFrame, SetWireFrame  );
        method( "ReloadResources", ReloadResources );
    }
private:
    void                    RecreateDefaultRes();
    void                    ReleaseDefaultRes ();
    void                    CreateQuadIB      ();
}; // class JRenderServer

#endif // __JRENDERSERVER8_H__
