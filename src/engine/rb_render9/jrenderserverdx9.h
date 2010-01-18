//****************************************************************************/
//  File:  JRenderServerDX9.h
//  Desc:  
//****************************************************************************/
#ifndef __DeviceDX9_H__
#define __DeviceDX9_H__

#include "IRenderServer.h"
#include "Mat3.h"
#include "Mat4.h"
#include "Frame.h"
#include "Timer.h"

//#define DEBUG_VS
//#define DEBUG_PS

typedef IDirect3D9                 DXAPI;
typedef IDirect3DDevice9           DXDevice;
typedef IDirect3DSurface9          DXSurface;
typedef IDirect3DTexture9          DXTexture;
typedef IDirect3DBaseTexture9      DXBaseTexture;

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
    VertexDeclaration               m_VDecl;
    IDirect3DVertexDeclaration9*    m_pVDecl;
    uint32_t                           m_FVF;
}; // struct VDeclInstance

struct VBInstance
{
    IDirect3DVertexBuffer9*         m_pBuffer;
    std::string                     m_Name;
    int                             m_CurIteration;
    int                             m_CurLastByte;
    bool                            m_bDynamic;
    int                             m_Size;

    VBInstance() : m_pBuffer(NULL), m_CurIteration(0), m_Size(0), m_CurLastByte(0) {}
}; // struct VBInstance

struct IBInstance
{
    IDirect3DIndexBuffer9*  m_pBuffer;
    std::string             m_Name;
    int                     m_CurIteration;
    int                     m_CurLastByte;
    bool                    m_bDynamic;
    int                     m_Size;

    IBInstance() : m_pBuffer(NULL), m_CurIteration(0), m_Size(0), m_CurLastByte(0) {}
}; // struct IBInstance

struct ShaderInstance
{
    std::string             m_Name;
    ID3DXEffect*            m_pEffect;


                            ShaderInstance() : m_pEffect( NULL ) {}
    const char*             GetName() const { return m_Name.c_str(); }
    void                    SetName( const char* name ) { m_Name = name; }
}; // struct ShaderInstance

const int c_QuadIBSize = 65536*2*6;
//****************************************************************************/
//  Class:  JRenderServerDX9
//  Desc:   
//****************************************************************************/
class JRenderServerDX9 : public JObject, public IRenderServer
{
public:
                            JRenderServerDX9   ();
    virtual                 ~JRenderServerDX9  ();
    virtual bool            Shut            ();
    virtual void            Init            ();
    virtual bool            IsInited        () const { return m_bInited; }
    virtual const Frame&    GetViewport     () const { return m_Viewport; }
    virtual void            SetViewport     ( const Frame& rc );
    virtual bool            StartFrame      ();
    virtual void            EndFrame        ();
    virtual void            ClearViewport   ( uint32_t color );
    virtual void            ClearDepthStencil( bool bDepth, bool bStencil );
    virtual void            ShowCursor      ( bool bShow = true );

    virtual int             GetVDeclID      ( const VertexDeclaration& vdecl );

    virtual void            SetAmbient      ( uint32_t color );
    virtual int             AddPointLight   ( const Vec3& pos, float radius, float falloff, 
                                              uint32_t diffuse, uint32_t specular, bool bPerPixel );
    virtual int             AddDirLight     ( const Vec3& dir, uint32_t diffuse, uint32_t specular, bool bPerPixel );

    virtual int             GetFPS          () const { return (int)m_FPS; }
    virtual uint32_t           GetCurFrame     () const { return m_CurFrame; }

    virtual void            SetViewTM       ( const Mat4& tm );
    virtual void            SetWorldTM      ( const Mat4& tm );
    virtual void            SetProjTM       ( const Mat4& tm );

    virtual const Mat4&     GetViewTM       () const { return m_ViewTM; }
    virtual const Mat4&     GetWorldTM      () const { return m_WorldTM; }
    virtual const Mat4&     GetProjTM       () const { return m_ProjTM; }

    virtual void            SetTextureTM    ( const Mat4& tm );
    virtual const Mat4&     GetTextureTM    () const { return m_TextureTM; }

    virtual void            SetColorTint    ( uint32_t color );
    
    virtual int             GetTextureID    ( const char* texName );
    virtual bool            GetTextureProp  ( int texID, TextureProperties& texProp );
    virtual void            SetTexture      ( int texID, int stage );
    virtual void            DeleteTexture   ( int texID );
    virtual int             CreateTexture   ( const TextureProperties& texProp );
    virtual BYTE*           LockTexture     ( int texID, const Frame& rect, int level = 0, uint32_t* pStride = NULL );
    virtual bool            UnlockTexture   ( int texID, int level = 0 );
    virtual bool            CopyTexture     ( int destID, int srcID, const Frame* rct = NULL, int nRect = 0 );


    virtual int             GetShaderID     ( const char* name, bool bReload = false );
    virtual int             GetConstantID   ( int shaderID, const char* name );
    virtual bool            SetConstant     ( int shaderID, int constID, const Mat4& val );
    virtual bool            SetConstant     ( int shaderID, int constID, float x, float y, float z, float w );
    virtual bool            SetConstant     ( int shaderID, int constID, float val );

    virtual void            SetShader       ( int shID );

    virtual void            ReloadResources ();

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
    
    virtual bool            Draw            ( int firstIdx, int numIdx, int firstVert, int numVert, 
                                                PrimitiveType primType = PrimitiveType_TriangleList );

    virtual bool            CacheIB         ( int ibID, BYTE* pData, int size, int& iteration, int& firstByte );
    virtual bool            CacheVB         ( int vbID, BYTE* pData, int size, int stride, int& iteration, int& firstByte );
    virtual int             GetVBIteration  ( int vbID );
    virtual int             GetIBIteration  ( int ibID );

    virtual void            ResetTime       ();

    virtual void            Register        ( IRenderServerClient* pClient );
    virtual void            Unregister      ( IRenderServerClient* pClient );

    bool                    IsFullScreen    () const;
    void                    SetFullScreen   ( bool bFullScreen );

    bool                    IsWireFrame     () const { return m_bWireFrame; }
    void                    SetWireFrame    ( bool bWireFrame );

    int                     GetDIPsPerFrame     () const { return m_CurDIPsPerFrame;       }
    int                     GetPolysPerFrame    () const { return m_CurPolysPerFrame;      }
    int                     GetShadersPerFrame  () const { return m_CurShadersPerFrame;    }
    int                     GetTexturesPerFrame () const { return m_CurTexturesPerFrame;   }
    int                     GetRTsPerFrame      () const { return m_CurRTsPerFrame;        }

    virtual void*           GetRenderSurface    ();
    virtual bool            ResizeBackBuffer    ( int w, int h );

    expose(JRenderServerDX9)
    {
        parent(JObject);
        rprop( "FPS",           GetFPS              );
        rprop( "DIPsPF",        GetDIPsPerFrame     );
        rprop( "PolysPF",       GetPolysPerFrame    );
        rprop( "ShadersPF",     GetShadersPerFrame  );
        rprop( "TexturesPF",    GetTexturesPerFrame );
        rprop( "RTsPF",         GetRTsPerFrame      );
        
        prop( "fullscreen", IsFullScreen, SetFullScreen );
        prop( "WireFrame",  IsWireFrame, SetWireFrame  );
        
        method( "ReloadResources", ReloadResources );
    }

private:
    Frame                           m_Viewport;

    DXAPI*					        m_pD3D;
    DXDevice*				        m_pDevice;
    HWND					        m_hWnd;
    DEVMODE					        m_dmDesktop;
    uint32_t					        m_CurFrame;
    bool                            m_bInited;
    float                           m_FPS;
    bool                            m_bCursorVisible;

    Mat4                            m_ViewTM;
    Mat4                            m_ProjTM;
    Mat4                            m_WorldTM;
    Mat4                            m_TextureTM;
    Timer                           m_Time;
    float                           m_CurTime;

    bool                            m_bWireFrame;

    int                             m_CurShader;

    DXSurface*                      m_pBackBuffer;
    DXSurface*                      m_pDepthStencil;
    std::vector<TextureFile>        m_Textures;
    std::vector<ShaderInstance>     m_Shaders;
    std::vector<VDeclInstance>      m_VertexDeclarations;
    std::vector<VBInstance>         m_VBuffers;
    std::vector<IBInstance>         m_IBuffers;

    std::vector<IRenderServerClient*> m_Clients;

    uint32_t                           m_CurFVF;
    int                             m_QuadIB;

    int                             m_BaseIndex;

    uint32_t                           m_Ambience;
    bool                            m_bHasStencil;

    bool                            m_bDeviceLost;
    D3DPRESENT_PARAMETERS           m_PresParam;
    
    int                             m_RTID;
    int                             m_DSID;

    ID3DXEffectPool*                m_pEffectPool;

    int                             m_PoolShaderID;

    int                             m_hWorldTM;		  
    int                             m_hViewTM;         
    int                             m_hProjTM;    
    int                             m_hCamPos;
    int                             m_hWorldViewProjTM;
    int                             m_hMtlDiffuse;     
    int                             m_hMtlAmbient;     
    int                             m_hAmbience;       
    int                             m_hSunDirection;   
    int                             m_hSunDiffuse;	  
    int                             m_hSunSpecular;	 
    int                             m_hTime;

    int                             m_DIPsPerFrame;    
    int                             m_PolysPerFrame;   
    int                             m_ShadersPerFrame; 
    int                             m_TexturesPerFrame;
    int                             m_RTsPerFrame;    

    int                             m_CurDIPsPerFrame;    
    int                             m_CurPolysPerFrame;   
    int                             m_CurShadersPerFrame; 
    int                             m_CurTexturesPerFrame;
    int                             m_CurRTsPerFrame;  

    void                            InitGlobalConstants();
    void                            SetGlobalConstants();

    void                            ReleaseDefaultRes();
    void                            RecreateDefaultRes();
    void                            CreateQuadIB();
    
    void                            ResetStats();
}; // class JRenderServerDX9

#endif // __DEVICEDX9_H__
