//****************************************************************************/
//  File:  JRenderServerGL.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "jreflect.h"
#include "irenderserver.h"
#include "jrenderservergl.h"
#include "timer.h"
#include "fstream.h"
#include "iwindowserver.h"
#include "ifileserver.h"

//****************************************************************************/
/*  JRenderServerGL
//****************************************************************************/
decl_class(JRenderServerGL);
JRenderServerGL::JRenderServerGL()
{
    m_Viewport          = Frame( 0.0f, 0.0f, 800.0f, 600.0f );
    m_CurFrame          = 0;
    m_bInited           = false;
    m_ViewTM            = Mat4::identity;
    m_ProjTM            = Mat4::identity;
    m_WorldTM           = Mat4::identity;
    m_TextureTM         = Mat4::identity;
    m_bCursorVisible    = true;
    m_bHasStencil       = false;
    m_CurFVF            = 0;
    m_QuadIB            = -1;
    m_Ambience          = 0xFF333333;
    m_CurShader         = -1;
    m_BaseIndex         = 0;
    m_RTID              = -1;
    m_DSID              = -1;
    m_bDeviceLost       = false;

    m_FPS               = 0;
    m_bWireFrame        = false;
    g_pRenderServer     = this;

    ResetStats();
}

JRenderServerGL::~JRenderServerGL()
{
    Shut();
    if (g_pRenderServer == this)
    {
        g_pRenderServer = NULL;
    }
}

void JRenderServerGL::ResetStats()
{
    m_CurDIPsPerFrame      = m_DIPsPerFrame;
    m_CurPolysPerFrame     = m_PolysPerFrame;
    m_CurShadersPerFrame   = m_ShadersPerFrame;
    m_CurTexturesPerFrame  = m_TexturesPerFrame;
    m_CurRTsPerFrame       = m_RTsPerFrame;

    m_DIPsPerFrame         = 0;
    m_PolysPerFrame        = 0;
    m_ShadersPerFrame      = 0;
    m_TexturesPerFrame     = 0;
    m_RTsPerFrame          = 0;
}

void JRenderServerGL::InitGlobalConstants()
{
    /*m_PoolShaderID = GetShaderID( "globals_pool" );

    if (m_PoolShaderID == -1)
    {
      return;
    }

    m_hWorldTM            = GetConstantID( m_PoolShaderID, "g_WorldTM"          );
    m_hViewTM           = GetConstantID( m_PoolShaderID, "g_ViewTM"           );
    m_hProjTM           = GetConstantID( m_PoolShaderID, "g_ProjTM"           );
    m_hWorldViewProjTM  = GetConstantID( m_PoolShaderID, "g_WorldViewProjTM"  );
    m_hMtlDiffuse       = GetConstantID( m_PoolShaderID, "g_MtlDiffuse"       );
    m_hMtlAmbient       = GetConstantID( m_PoolShaderID, "g_MtlAmbient"       );
    m_hAmbience         = GetConstantID( m_PoolShaderID, "g_Ambience"         );
    m_hSunDirection     = GetConstantID( m_PoolShaderID, "g_SunDirection"     );
    m_hSunDiffuse       = GetConstantID( m_PoolShaderID, "g_SunDiffuse"         );
    m_hSunSpecular      = GetConstantID( m_PoolShaderID, "g_SunSpecular"        );
    m_hCamPos           = GetConstantID( m_PoolShaderID, "g_CamPos"           );
    m_hTime             = GetConstantID( m_PoolShaderID, "g_Time"               );*/

}

void JRenderServerGL::SetGlobalConstants()
{
    /*if (m_CurShader < 0 || m_CurShader >= m_Shaders.size()) return;

    Mat4 camTM;
    camTM.inverse( m_ViewTM );
    Vec3 camPos( camTM.e30, camTM.e31, camTM.e32 );
    Mat4 wvpTM = m_WorldTM;
    wvpTM *= m_ViewTM;
    wvpTM *= m_ProjTM;
    SetConstant( m_PoolShaderID, m_hWorldTM,         m_WorldTM );
    SetConstant( m_PoolShaderID, m_hViewTM,          m_ViewTM  );
    SetConstant( m_PoolShaderID, m_hProjTM,          m_ProjTM  );
    SetConstant( m_PoolShaderID, m_hWorldViewProjTM, wvpTM     );
    SetConstant( m_PoolShaderID, m_hTime,            m_CurTime );
    SetConstant( m_PoolShaderID, m_hCamPos,          camPos.x, camPos.y, camPos.z, 0.0f );

    const ShaderInstance& shader = m_Shaders[m_CurShader];
    shader.m_pEffect->CommitChanges();*/
}

void JRenderServerGL::Init()
{
    SetName( "render" );

    //m_hWnd = (HWND)g_pWindowServer->GetRootHandle();
    //if (m_bInited)
    //{
    //    return;
    //}

    //m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    //if (m_pD3D == NULL)
    //{
    //    rlog.err( "Could not initialize d3d9." );
    //    return;
    //}
    //
    //D3DDISPLAYMODE d3ddm;
    //m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
    //
    //ZeroMemory( &m_PresParam, sizeof(m_PresParam) );

    //JScreenMode mode = g_pWindowServer->GetScreenMode();
    //BOOL bWindowed = (mode == smWindow || mode == smDesktop);

    //bool bHasStencil = true;

    //m_PresParam.Windowed                  = bWindowed;
    //m_PresParam.SwapEffect                = bWindowed ? D3DSWAPEFFECT_COPY : D3DSWAPEFFECT_FLIP;
    //m_PresParam.EnableAutoDepthStencil    = TRUE;
    //m_PresParam.AutoDepthStencilFormat    = D3DFMT_D16;
    //m_PresParam.BackBufferFormat        = d3ddm.Format;
    //m_PresParam.BackBufferCount           = 1;
    //m_PresParam.hDeviceWindow             = m_hWnd;
    //m_PresParam.PresentationInterval      = D3DPRESENT_INTERVAL_IMMEDIATE;

    //if (bHasStencil)
    //{
    //    m_PresParam.AutoDepthStencilFormat = D3DFMT_D24S8;
    //    m_bHasStencil = true;
    //}

    //Frame ext;
    //g_pWindowServer->GetRootExt( ext );

    //m_PresParam.BackBufferWidth  = int( ext.w );
    //m_PresParam.BackBufferHeight = int( ext.h );

    //uint32_t flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    //D3DDEVTYPE devType = D3DDEVTYPE_HAL;

    //rlog.msg( "Creating d3d9 render device..." );

    //HRESULT res = m_pD3D->CreateDevice(   adapterID, devType, m_hWnd, flags, &m_PresParam, &m_pDevice );
    //if (!m_pDevice)
    //{
    //    rlog.warn( "Could not create render device. Switching to software vertex processing." );
    //
    //    flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    //    res = m_pD3D->CreateDevice(   0, devType, m_hWnd, flags, &m_PresParam, &m_pDevice );
    //
    //    if (!m_pDevice)
    //    {
    //        rlog.err( "Could not create render device. %s", GetDXError( res ) );
    //        return;
    //    }
    //}

    //rlog.msg( "Render device created OK." );
    //
    //SAFE_RELEASE( m_pBackBuffer );
    //SAFE_RELEASE( m_pDepthStencil );
    //m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
    //m_pDevice->GetDepthStencilSurface( &m_pDepthStencil );

    //m_bInited = true;

    ////  register stock vertex types
    //for (int vtype = VertexType_Unknown; vtype < VertexType_Last; vtype++)
    //{
    //    int typeID = GetVDeclID( GetStockVDecl( (VertexType)vtype ) );
    //    assert( typeID == vtype );
    //}

    ////  create quad index buffer
    //CreateQuadIB();

    //D3DXCreateEffectPool( &m_pEffectPool );

    //InitGlobalConstants();

    //ResetTime();

    ////  invalidate all device resource clients
    //for (int i = 0; i < m_Clients.size(); i++)
    //{
    //    m_Clients[i]->OnResetDevice();
    //}
}

bool JRenderServerGL::IsFullScreen() const
{
    return false;
}

void JRenderServerGL::SetFullScreen( bool bFullScreen )
{
//    bool bCurFullScreen = m_PresParam.Windowed ? false : true;
//
//    if (bCurFullScreen == bFullScreen)
//    {
//        return;
//    }
//
//    g_pWindowServer->DestroyWnd();
//    g_pWindowServer->CreateWnd( bFullScreen ? smFullscreen : smWindow );
//    m_hWnd = (HWND)g_pWindowServer->GetRootHandle();
//    m_PresParam.hDeviceWindow = m_hWnd;
//
//    m_PresParam.Windowed   = !bFullScreen;
//    m_PresParam.SwapEffect = bFullScreen ? D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_COPY;
//
//    ReleaseDefaultRes();
//    HRESULT hRes = m_pDevice->Reset( &m_PresParam );
//    if (hRes == S_OK)
//    {
//        RecreateDefaultRes();
//        //  invalidate all device resource clients
//        for (int i = 0; i < m_Clients.size(); i++)
//        {
//            m_Clients[i]->OnResetDevice();
//        }
//    }
}

bool JRenderServerGL::ResizeBackBuffer( int w, int h )
{
//    if (m_pDevice == NULL ||
//        (w == m_PresParam.BackBufferWidth &&
//         h == m_PresParam.BackBufferHeight ))
//    {
//        return false;
//    }
//
//    m_PresParam.BackBufferWidth  = w;
//    m_PresParam.BackBufferHeight = h;
//
//    //  invalidate all device resource clients
//    for (int i = 0; i < m_Clients.size(); i++)
//    {
//        m_Clients[i]->OnResetDevice();
//    }
//
//    ReleaseDefaultRes();
//
//    HRESULT hRes = m_pDevice->Reset( &m_PresParam );
//    if (hRes == S_OK)
//    {
//        RecreateDefaultRes();
//        Frame ext;
//        g_pWindowServer->GetRootExt( ext );
//        ext.w = w;
//        ext.h = h;
//        g_pWindowServer->SetRootExt( ext );
//    }
//    else
//    {
//        rlog.err( "Could not resize device backbuffer to [%d %d]", w, h );
//    }
    return true;
}

void* JRenderServerGL::GetRenderSurface()
{
    return NULL;//m_pBackBuffer;
}

void JRenderServerGL::ResetTime()
{
    m_Time.Start();
}

bool JRenderServerGL::StartFrame()
{
    //if (!m_bInited || !m_pDevice)
    //{
    //    return false;
    //}

    //ResetStats();

    //if (m_bDeviceLost)
    //{
    //    //  test the cooperative level to see if it's okay to render
    //    HRESULT hRes = m_pDevice->TestCooperativeLevel();
    //    if (hRes != S_OK)
    //    {
    //        //  if the device was lost, do not render until we get it back
    //        if (hRes == D3DERR_DEVICELOST)
    //        {
    //            return false;
    //        }

    //        //  check if the device needs to be reset
    //        if (hRes == D3DERR_DEVICENOTRESET)
    //        {
    //            ReleaseDefaultRes();
    //            hRes = m_pDevice->Reset( &m_PresParam );
    //            if (hRes == S_OK)
    //            {
    //                RecreateDefaultRes();
    //                //  invalidate all device resource clients
    //                for (int i = 0; i < m_Clients.size(); i++)
    //                {
    //                    m_Clients[i]->OnResetDevice();
    //                }
    //            }
    //        }
    //        return false;
    //    }
    //    m_bDeviceLost = false;
    //}
    //

    //m_CurTime = float( m_Time.Seconds() );
    //SetColorTint( 0xFFFFFFFF );
    //m_pDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX*)&Mat4::identity );
    return true;
} // JRenderServerGL::StartFrame

void JRenderServerGL::EndFrame()
{
    m_CurFrame++;

    SetShader( -1 );

    /*
    if (!m_pDevice) return;
    HRESULT hRes = m_pDevice->Present( NULL, NULL, NULL, NULL );
    if (hRes == D3DERR_DEVICELOST)
    {
        m_bDeviceLost = true;
        return;
    }*/

    static Timer s_Timer;
    static int s_NFrames = 0;
    float dt = s_Timer.Seconds();
    s_NFrames++;
    if (dt > 0.5f)
    {
        s_Timer.Start();
        m_FPS = float( s_NFrames )/dt;
        s_NFrames = 0;
    }


    for (int i = 0; i < m_Clients.size(); i++)
    {
        m_Clients[i]->OnFrame();
    }

}

void JRenderServerGL::ReloadResources()
{
    //for (int i = 0; i < m_Shaders.size(); i++)
    //{
    //    GetShaderID( m_Shaders[i].m_Name.c_str(), true );
    //}
    //m_pEffectPool->Release();
    //D3DXCreateEffectPool( &m_pEffectPool );
    //InitGlobalConstants();
}

void JRenderServerGL::ClearViewport( uint32_t color )
{
    //if (!m_pDevice) return;
    //m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, color, 1.0, 0 );
}

void JRenderServerGL::ClearDepthStencil( bool bDepth, bool bStencil )
{
    //if (!m_pDevice) return;
    //uint32_t flags = 0;
    //if (bDepth)   flags |= D3DCLEAR_ZBUFFER;
    //if (bStencil && m_bHasStencil) flags |= D3DCLEAR_STENCIL;
    //m_pDevice->Clear( 0, NULL, flags, 0, 1.0, 0 );
}

void JRenderServerGL::SetViewport( const Frame& rc )
{
    //if (!m_pDevice) return;

    ////  clip viewport to the render target size
    //Frame vp = rc;
    //Frame rcRT( Frame::null );
    //if (m_RTID == -1)
    //{
    //    rcRT.w = float( m_PresParam.BackBufferWidth  );
    //    rcRT.h = float( m_PresParam.BackBufferHeight );
    //}
    //else
    //{
    //    rcRT.w = float( m_Textures[m_RTID].m_Prop.m_Width  );
    //    rcRT.h = float( m_Textures[m_RTID].m_Prop.m_Height );
    //}

    //vp.Clip( rcRT );
    //if (vp.w < 1.0f || vp.h < 1.0f)
    //{
    //    return;
    //}

    //m_Viewport = vp;
    //D3DVIEWPORT9 dvp;
    //dvp.X     = m_Viewport.x;
    //dvp.Y     = m_Viewport.y;
    //dvp.Width = m_Viewport.w;
    //dvp.Height    = m_Viewport.h;
    //dvp.MinZ  = 0.0f;
    //dvp.MaxZ  = 1.0f;
    //m_pDevice->SetViewport( (D3DVIEWPORT9*)&dvp );
}

bool JRenderServerGL::Shut()
{
    if (!m_bInited)
    {
        return false;
    }

    rlog.msg( "Releasing d3d9 render device..." );

    /*SAFE_RELEASE( m_pBackBuffer );
    SAFE_RELEASE( m_pDepthStencil );

    for (int i = 0; i < m_Textures.size(); i++)
    {
        SAFE_RELEASE( m_Textures[i].m_pTexture );
        SAFE_RELEASE( m_Textures[i].m_pDepthStencil );
    }
    for (int i = 0; i < m_Shaders.size(); i++)
    {
        SAFE_RELEASE( m_Shaders[i].m_pEffect );
    }
    for (int i = 0; i < m_VBuffers.size(); i++)
    {
        SAFE_RELEASE( m_VBuffers[i].m_pBuffer );
    }
    for (int i = 0; i < m_IBuffers.size(); i++)
    {
        SAFE_RELEASE( m_IBuffers[i].m_pBuffer );
    }
    for (int i = 0; i < m_VertexDeclarations.size(); i++)
    {
        SAFE_RELEASE( m_VertexDeclarations[i].m_pVDecl );
    }

    m_Textures.clear();
    m_Shaders.clear();
    m_VBuffers.clear();
    m_IBuffers.clear();
    m_VertexDeclarations.clear();

    int nRef = 0;
    if (m_pDevice) nRef = m_pDevice->Release();
    m_pDevice = NULL;

    if (nRef > 0)
    {
        rlog.err( "Render device was not released properly." );
    }

    SAFE_RELEASE( m_pD3D );*/
    m_bInited = false;
    return true;
}

void JRenderServerGL::SetViewTM( const Mat4& tm )
{
    m_ViewTM = tm;
    //m_pDevice->SetTransform( D3DTS_VIEW, (D3DMATRIX*)&tm );
}

void JRenderServerGL::SetWorldTM( const Mat4& tm )
{
    m_WorldTM = tm;
    //m_pDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&tm );
}

void JRenderServerGL::SetProjTM( const Mat4& tm )
{
    m_ProjTM = tm;
    //m_pDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&tm );
}

void JRenderServerGL::SetTextureTM( const Mat4& tm )
{
    m_TextureTM = tm;
    //m_pDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX*)&tm );
}

int JRenderServerGL::GetTextureID( const char* texName )
{
    /*if (!texName || !m_pDevice) return -1;
    for (int i = 0; i < m_Textures.size(); i++)
    {
        if (!stricmp( m_Textures[i].m_Name.c_str(), texName )) return i;
    }

    FInStream is;
    g_pFileServer->OpenMedia( texName, "", is );
    if (!is) return -1;

    int nBytes = is.GetTotalSize();
    uint8_t* buf = new uint8_t[nBytes];
    is.Read( buf, nBytes );

    D3DXIMAGE_INFO info;
    HRESULT hRes = D3DXGetImageInfoFromFileInMemory( buf, nBytes, &info );
    DXBaseTexture* pTex = NULL;


    TextureFile tex;
    tex.m_pDepthStencil = NULL;
    tex.m_Name          = texName;

    TextureProperties& tp = tex.m_Prop;

    if (info.ResourceType == D3DRTYPE_CUBETEXTURE)
    {
        IDirect3DCubeTexture9* pCubeTexture = NULL;
        hRes = D3DXCreateCubeTextureFromFileInMemoryEx( m_pDevice,
            buf, nBytes,
            D3DX_DEFAULT, 0,
            0, D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
            0x00000000, NULL, NULL, &pCubeTexture );
        pTex = pCubeTexture;

        D3DSURFACE_DESC desc;
        pCubeTexture->GetLevelDesc( 0, &desc );
        tp.m_Format         = ConvertColorFormat( desc.Format );
        tp.m_Height         = desc.Height;
        tp.m_Width          = desc.Width;
        tp.m_PoolType       = ConvertPoolType( desc.Pool );
    }
    else
    {
        DXTexture* pTexture2D = NULL;
        hRes = D3DXCreateTextureFromFileInMemoryEx( m_pDevice,
            buf, nBytes,
            D3DX_DEFAULT, D3DX_DEFAULT, 0,
            0, D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED , D3DX_DEFAULT, D3DX_DEFAULT,
            0x00000000, NULL, NULL, &pTexture2D );
        pTex = pTexture2D;

        D3DSURFACE_DESC desc;
        pTexture2D->GetLevelDesc( 0, &desc );
        tp.m_Format         = ConvertColorFormat( desc.Format );
        tp.m_Height         = desc.Height;
        tp.m_Width          = desc.Width;
        tp.m_PoolType       = ConvertPoolType( desc.Pool );
    }

    delete []buf;
    if (!pTex || hRes != S_OK) return -1;

    tp.m_bAutoGenMips   = false;
    tp.m_NMips          = pTex->GetLevelCount();
    strcpy( tp.m_Name, texName );

    tex.m_pTexture      = pTex;
    m_Textures.push_back( tex );
    return m_Textures.size() - 1;*/
    return -1;
}

void JRenderServerGL::SetTexture( int texID, int stage )
{
   /* if (texID < 0 || texID >= (int)m_Textures.size()) return;
    m_pDevice->SetTexture( stage, m_Textures[texID].m_pTexture );
    m_TexturesPerFrame++;*/
}

void JRenderServerGL::DeleteTexture( int texID )
{
    //if (texID < 0 || texID >= (int)m_Textures.size()) return;
    //m_Textures[texID].m_pTexture->Release();
    //m_Textures[texID].m_pTexture = NULL;
}

void JRenderServerGL::ShowCursor( bool bShow )
{
    /*if (bShow == m_bCursorVisible) return;
    if (bShow)
    {
        m_pDevice->ShowCursor( TRUE );
        ::ShowCursor( TRUE );
    }
    else
    {
        m_pDevice->ShowCursor( FALSE );
        ::ShowCursor( FALSE );
    }
    m_bCursorVisible = bShow;*/
}

int JRenderServerGL::CreateDepthStencil( const char* texName, int w, int h, DepthFormat fmt )
{
    /*DXSurface* pSurface = NULL;
    HRESULT hRes = m_pDevice->CreateDepthStencilSurface( w, h, ConvertDepthFormat( fmt ),
        D3DMULTISAMPLE_NONE, 0, TRUE, &pSurface, NULL );
    if (hRes != S_OK || !pSurface)
    {
        rlog.err( "Could not create depth stencil surface %dx%d. %s", GetDXError( hRes ) );
        return -1;
    }
    TextureFile tex;

    D3DSURFACE_DESC desc;
    pSurface->GetDesc( &desc );

    TextureProperties& tp = tex.m_Prop;
    tp.m_bAutoGenMips   = false;
    tp.m_Format         = ColorFormat_Unknown;
    tp.m_DSFormat       = fmt;
    tp.m_Height         = desc.Height;
    tp.m_Width          = desc.Width;
    tp.m_PoolType       = ConvertPoolType( desc.Pool );
    tp.m_NMips          = 0;
    strcpy( tp.m_Name, texName );

    tex.m_pTexture      = NULL;
    tex.m_pDepthStencil = pSurface;
    tex.m_Name          = texName;
    m_Textures.push_back( tex );
    return m_Textures.size() - 1;*/
    return 0;
}

int JRenderServerGL::CreateRenderTarget( const char* texName, int w, int h, ColorFormat fmt )
{
    /*uint32_t usage = D3DUSAGE_RENDERTARGET;
    DXTexture* pTex = NULL;
    HRESULT hRes = D3DXCreateTexture( m_pDevice, w, h, 1, usage, ConvertColorFormat( fmt ), D3DPOOL_DEFAULT, &pTex );
    if (hRes != S_OK || !pTex)
    {
        rlog.err( "Could not create render target texture %dx%d. %s", GetDXError( hRes ) );
        return -1;
    }
    TextureFile tex;

    D3DSURFACE_DESC desc;
    pTex->GetLevelDesc( 0, &desc );

    TextureProperties& tp = tex.m_Prop;
    tp.m_bAutoGenMips   = false;
    tp.m_Format         = ConvertColorFormat( desc.Format );
    tp.m_Height         = desc.Height;
    tp.m_Width          = desc.Width;
    tp.m_PoolType       = ConvertPoolType( desc.Pool );
    tp.m_bRenderTarget  = true;
    tp.m_NMips          = pTex->GetLevelCount();
    strcpy( tp.m_Name, texName );

    tex.m_pTexture      = pTex;
    tex.m_pDepthStencil = NULL;
    tex.m_Name          = texName;
    m_Textures.push_back( tex );
    return m_Textures.size() - 1;*/
    return 0;
}

bool JRenderServerGL::SetRenderTarget( int texID, int dsID )
{
    //if (texID < 0 || texID >= (int)m_Textures.size())
    //{
    //    //  restore backbuffer render target
    //    HRESULT res = m_pDevice->SetRenderTarget( 0, m_pBackBuffer );
    //    res = m_pDevice->SetDepthStencilSurface( m_pDepthStencil );
    //    m_RTID = -1;
    //    m_DSID = -1;
    //    return (res == S_OK);
    //}
    //DXTexture* pTex = (DXTexture*)m_Textures[texID].m_pTexture;
    //if (!pTex) return false;
    //DXSurface* pSurf = NULL;
    //pTex->GetSurfaceLevel( 0, &pSurf );
    //HRESULT res = m_pDevice->SetRenderTarget( 0, pSurf );
    //pSurf->Release();

    //DXSurface* pDepthStencil = m_pDepthStencil;
    //if (dsID >= 0 && dsID < (int)m_Textures.size())
    //{
    //    pDepthStencil = m_Textures[dsID].m_pDepthStencil;
    //}

    ////  check for depth-stencil buffer being the proper size
    //if (pDepthStencil && pSurf)
    //{
    //    D3DSURFACE_DESC dsDesc, rtDesc;
    //    res &= pDepthStencil->GetDesc( &dsDesc );
    //    res &= pSurf->GetDesc( &rtDesc );
    //    if (dsDesc.Width < rtDesc.Width || dsDesc.Height < rtDesc.Height)
    //    {
    //        rlog.warn( "DepthStensil surface has less size (%dx%d) than RenderTarget (%dx%d)."
    //            " Defaulting to NULL.", dsDesc.Width, dsDesc.Height, rtDesc.Width, rtDesc.Height );
    //        pDepthStencil = NULL;
    //    }
    //}

    //res &= m_pDevice->SetDepthStencilSurface( pDepthStencil );

    //m_RTID = texID;
    //m_DSID = dsID;

    //m_RTsPerFrame++;

    //return (res == S_OK);
    return false;
}

bool JRenderServerGL::SaveTexture( int texID, const char* fname )
{
    /*if (texID < 0 || texID >= (int)m_Textures.size()) return false;
    DXTexture* pTex = (DXTexture*)m_Textures[texID].m_pTexture;
    if (!pTex) return false;

    char drive[_MAX_PATH];
    char dir  [_MAX_PATH];
    char file [_MAX_PATH];
    char ext  [_MAX_PATH];

    _splitpath( fname, drive, dir, file, ext );

    D3DXIMAGE_FILEFORMAT type = D3DXIFF_DDS;

    if (!strcmp( ext, ".bmp" )) type = D3DXIFF_BMP;
    if (!strcmp( ext, ".tga" )) type = D3DXIFF_TGA;
    if (!strcmp( ext, ".dds" )) type = D3DXIFF_DDS;

    HRESULT hRes = D3DXSaveTextureToFile( fname, type, pTex, 0 );
    return (hRes == S_OK);*/
    return false;
}

void JRenderServerGL::SetAmbient( uint32_t color )
{
    m_Ambience = color;
}

int JRenderServerGL::AddPointLight( const Vec3& pos, float radius, float falloff,
                          uint32_t diffuse, uint32_t specular, bool bPerPixel )
{
    return 0;
}

int JRenderServerGL::AddDirLight( const Vec3& dir, uint32_t diffuse, uint32_t specular, bool bPerPixel )
{
   /* D3DLIGHT9 light;
    light.Type          = D3DLIGHT_DIRECTIONAL ;
    light.Diffuse       = ConvertColor( diffuse  );
    light.Specular      = ConvertColor( specular );
    light.Ambient       = ConvertColor( 0xFF000000 );

    Vec3 ldir( dir.x, dir.y, dir.z );
    ldir.normalize();
    light.Direction.x   = ldir.x;
    light.Direction.y   = ldir.y;
    light.Direction.z   = ldir.z;

    light.Range         = 100.0f;
    light.Position.x    = 0.0f;
    light.Position.y    = 0.0f;
    light.Position.z    = 0.0f;

    light.Attenuation0  = 0.0f;
    light.Attenuation1  = 0.0f;
    light.Attenuation2  = 0.0f;

    light.Falloff       = 0.0f;
    light.Theta         = c_PI;
    light.Phi           = c_PI;

    m_pDevice->LightEnable( 0, TRUE );
    m_pDevice->SetLight   ( 0, &light );*/

    return 0;
}

int JRenderServerGL::GetConstantID( int shaderID, const char* name )
{
    //if (shaderID < 0 || shaderID >= m_Shaders.size()) return -1;
    //ShaderInstance& sh = m_Shaders[shaderID];
    //int res = (int)sh.m_pEffect->GetParameterByName( NULL, name );
    //return res;
    return 0;
}

bool JRenderServerGL::SetConstant( int shaderID, int constID, const Mat4& val )
{
    //if (shaderID < 0 || shaderID >= m_Shaders.size() || constID == NULL) return false;
    //const ShaderInstance& shader = m_Shaders[shaderID];
    //HRESULT hRes = shader.m_pEffect->SetMatrix( reinterpret_cast<D3DXHANDLE>( constID ), (D3DXMATRIX*)&val );
    //return (hRes == S_OK);
    return false;
}

bool JRenderServerGL::SetConstant( int shaderID, int constID, float x, float y, float z, float w )
{
    //if (shaderID < 0 || shaderID >= m_Shaders.size() || constID == NULL) return false;
    //const ShaderInstance& shader = m_Shaders[shaderID];
    //D3DXVECTOR4 vec( x, y, z, w );
    //HRESULT hRes = shader.m_pEffect->SetVector( reinterpret_cast<D3DXHANDLE>( constID ), &vec );
    //return (hRes == S_OK);
    return false;
}

bool JRenderServerGL::SetConstant( int shaderID, int constID, float val )
{
    /*if (shaderID < 0 || shaderID >= m_Shaders.size() || constID == NULL) return false;
    const ShaderInstance& shader = m_Shaders[shaderID];
    HRESULT hRes = shader.m_pEffect->SetFloat( reinterpret_cast<D3DXHANDLE>( constID ), val );
    return (hRes == S_OK);
    */
    return false;
}

int JRenderServerGL::GetShaderID( const char* name, bool bReload )
{
    int shID = -1;
//    for (int i = 0; i < (int)m_Shaders.size(); i++)
//    {
//        if (!stricmp( m_Shaders[i].GetName(), name ))
//        {
//            shID = i;
//            break;
//        }
//    }
//
//    if (!bReload && shID != -1) return shID;
//
//    if (!name || name[0] == 0) return -1;
//
//    ID3DXEffect* pEffect = NULL;
//    ID3DXBuffer* pErrors = NULL;
//
//    uint32_t flags = D3DXFX_NOT_CLONEABLE;
//#ifdef DEBUG_VS
//    flags |= D3DXSHADER_SKIPOPTIMIZATION | D3DXSHADER_DEBUG |D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
//#endif
//#ifdef DEBUG_PS
//    flags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
//#endif
//
//    JString path;
//    if (!g_pFileServer->FindMedia( name, "fx", &path ))
//    {
//        rlog.err( "Could not find shader media file: %s", name );
//        return -1;
//    }
//
//    //  this is to be as low-end as possible
//    flags |= D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;
//
//    HRESULT hRes = D3DXCreateEffectFromFile( m_pDevice, path, NULL, NULL, flags, m_pEffectPool, &pEffect, &pErrors );
//
//    if (hRes != S_OK || !pEffect)
//    {
//      if (pErrors)
//      {
//        rlog.err( "Could not compile shader: %s. %s", name, (char*)pErrors->GetBufferPointer() );
//        pErrors->Release();
//      }
//      else
//      {
//        rlog.err( "Could not compile shader: %s", name );
//      }
//      return -1;
//    }
//
//    if (!bReload)
//    {
//        ShaderInstance sh;
//        sh.SetName( name );
//        sh.m_pEffect = pEffect;
//        m_Shaders.push_back( sh );
//        shID = m_Shaders.size() - 1;
//    }
//    else
//    {
//        m_Shaders[shID].m_pEffect->Release();
//        m_Shaders[shID].m_pEffect = pEffect;
//    }
    return shID;
}

void JRenderServerGL::SetShader( int shID )
{
    /*if (shID >= (int)m_Shaders.size()) return;
    if (shID == m_CurShader) return;

    if (m_CurShader >= 0)
    {
      ShaderInstance& shCurrent = m_Shaders[m_CurShader];
      if (shCurrent.m_pEffect)
      {
        shCurrent.m_pEffect->EndPass();
        shCurrent.m_pEffect->End();
      }
    }
    m_CurShader = shID;

    if (m_CurShader < 0) return;

    ShaderInstance& sh = m_Shaders[m_CurShader];
    if (sh.m_pEffect)
    {
      UINT nPasses = 0;
      uint32_t flags = D3DXFX_DONOTSAVESTATE | D3DXFX_DONOTSAVESAMPLERSTATE | D3DXFX_DONOTSAVESHADERSTATE;
      m_pDevice->SetVertexShader( NULL );
      m_pDevice->SetPixelShader( NULL );
      sh.m_pEffect->Begin( &nPasses, flags );
      sh.m_pEffect->BeginPass( 0 );

      sh.m_pEffect->CommitChanges();
    }

    if (m_bWireFrame)
    {
        m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
    }*/

    m_ShadersPerFrame++;
}

void JRenderServerGL::SetWireFrame( bool bWireFrame )
{
    m_bWireFrame = bWireFrame;
    //m_pDevice->SetRenderState( D3DRS_FILLMODE, m_bWireFrame ? D3DFILL_WIREFRAME : D3DFILL_SOLID );
}

int JRenderServerGL::CreateTexture( const TextureProperties& texProp )
{
    /*if (m_pDevice == NULL)
    {
        return -1;
    }
    int texID = GetTextureID( texProp.m_Name );
    if (texID != -1)
    {
        if (m_Textures[texID].m_pTexture)
        {
            return texID;
        }
    }
    else
    {
        m_Textures.push_back( TextureFile() );
        texID = m_Textures.size() - 1;
    }

    uint32_t       usage   = 0;
    D3DFORMAT   format  = ConvertColorFormat( texProp.m_Format );
    D3DPOOL     pool    = ConvertPoolType( texProp.m_PoolType );
    IDirect3DTexture9* pTexture = NULL;
    if (texProp.m_bRenderTarget)
    {
        usage = D3DUSAGE_RENDERTARGET;
    }
    HRESULT hRes = m_pDevice->CreateTexture( texProp.m_Width, texProp.m_Height,
        texProp.m_NMips, usage, format, pool, &pTexture, NULL );
    if (hRes != S_OK)
    {
        rlog.err( "Could not create texture %s (width=%d, height=%d).",
            texProp.m_Name, texProp.m_Width, texProp.m_Height );
        return -1;
    }

    TextureFile& tex = m_Textures[texID];
    tex.m_Prop          = texProp;
    tex.m_pTexture      = pTexture;
    tex.m_pDepthStencil = NULL;
    tex.m_Name          = texProp.m_Name;
    return texID;*/
    return 0;
}

uint8_t* JRenderServerGL::LockTexture( int texID, const Frame& rect, int level, uint32_t* pStride )
{
    /*if (texID < 0 || texID >= (int)m_Textures.size()) return NULL;
    D3DLOCKED_RECT dRect;
    RECT wrct;
    wrct.left   = rect.x;
    wrct.top    = rect.y;
    wrct.right  = rect.r();
    wrct.bottom = rect.b();
    uint32_t flags = 0;

    DXTexture* pTex = (DXTexture*)m_Textures[texID].m_pTexture;
    HRESULT hRes = pTex->LockRect( level, &dRect, &wrct, flags );
    if (hRes != S_OK) return NULL;
    if (pStride) *pStride = dRect.Pitch;
    return (uint8_t*)dRect.pBits;*/
    return NULL;
}

bool JRenderServerGL::UnlockTexture( int texID, int level )
{
    //if (texID < 0 || texID >= (int)m_Textures.size()) return false;
    //DXTexture* pTex = (DXTexture*)m_Textures[texID].m_pTexture;
    //HRESULT hRes = pTex->UnlockRect( level );
    //return (hRes == S_OK);
    return false;
}

void JRenderServerGL::SetColorTint( uint32_t color )
{
    //m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, color );
}

int JRenderServerGL::GetVDeclID( const VertexDeclaration& vdecl )
{
    /*if (!m_bInited)
    {
        return -1;
    }
    for (int i = 0; i < m_VertexDeclarations.size(); i++)
    {
        const VDeclInstance& vd = m_VertexDeclarations[i];
        if (vd.m_VDecl == vdecl) return i;
    }
    VDeclInstance vd;
    vd.m_VDecl = vdecl;
    vd.m_FVF = CreateFVF( vdecl );
    vd.m_pVDecl = CreateVDecl( m_pDevice, vdecl );
    m_VertexDeclarations.push_back( vd );
    return m_VertexDeclarations.size() - 1;*/
    return -1;
}

int JRenderServerGL::CreateVB( const char* name, int numBytes, bool bStatic )
{
    //int vbID = GetVBID( name );
    //if (vbID != -1)
    //{
    //    if( m_VBuffers[vbID].m_pBuffer)
    //    {
    //        return vbID;
    //    }
    //}
    //else
    //{
    //    m_VBuffers.push_back( VBInstance() );
    //    vbID = m_VBuffers.size() - 1;
    //}
    //VBInstance& vb = m_VBuffers[vbID];

    //uint32_t usage = D3DUSAGE_WRITEONLY;
    //if (!bStatic)
    //{
    //    usage |= D3DUSAGE_DYNAMIC;
    //}

    //D3DPOOL pool = D3DPOOL_DEFAULT;
    //uint32_t fvf = 0;//CreateFVF( GetStockVDecl( VertexType_XYZWDSUV ) );
    //IDirect3DVertexBuffer9* pBuffer = NULL;
    //HRESULT hRes = m_pDevice->CreateVertexBuffer( numBytes, usage, fvf, pool, &pBuffer, NULL );
    //if (hRes != S_OK) return -1;
    //vb.m_Name           = name;
    //vb.m_pBuffer        = pBuffer;
    //vb.m_bDynamic       = !bStatic;
    //vb.m_CurIteration++;
    //vb.m_CurLastByte    = 0;
    //vb.m_Size           = numBytes;
    //return vbID;
    return 0;
}

int JRenderServerGL::GetVBID( const char* name )
{
    //for (int i = 0; i < m_VBuffers.size(); i++)
    //{
    //    if (!stricmp( name, m_VBuffers[i].m_Name.c_str() )) return i;
    //}
    return -1;
}

bool JRenderServerGL::UnlockVB( int vbID )
{
    //if (vbID < 0 || vbID >= m_VBuffers.size()) return false;
    //IDirect3DVertexBuffer9* pBuffer = m_VBuffers[vbID].m_pBuffer;
    //if (!pBuffer) return NULL;
    //HRESULT hRes = pBuffer->Unlock();
    //return (hRes == S_OK);
    return false;
}

bool JRenderServerGL::SetVB( int vbID, int vdeclID, int stream )
{
    //if (vbID < 0 || vbID >= m_VBuffers.size() ||
    //    vdeclID < 0 || vdeclID >= m_VertexDeclarations.size())
    //{
    //    return false;
    //}
    //IDirect3DVertexBuffer9* pBuffer = m_VBuffers[vbID].m_pBuffer;
    //if (!pBuffer)
    //{
    //    return false;
    //}
    //const VDeclInstance& vdecl = m_VertexDeclarations[vdeclID];
    //HRESULT hRes = S_OK;
    //if (vdecl.m_FVF != m_CurFVF)
    //{
    //    //hRes = m_pDevice->SetVertexDeclaration( vdecl.m_pVDecl );
    //    m_pDevice->SetFVF( vdecl.m_FVF );
    //    m_CurFVF = vdecl.m_FVF;
    //    if (hRes != S_OK) return false;
    //}
    //hRes = m_pDevice->SetStreamSource( stream, pBuffer, 0, vdecl.m_VDecl.m_VertexSize  );
    //return (hRes == S_OK);
    return false;
}

bool JRenderServerGL::DeleteVB( int vbID )
{
    //if (vbID < 0 || vbID >= m_VBuffers.size()) return false;
    //m_VBuffers[vbID].m_pBuffer->Release();
    //m_VBuffers[vbID].m_pBuffer = NULL;
    return true;
}

int JRenderServerGL::CreateIB( const char* name, int numBytes, bool bStatic )
{
    /*int ibID = GetIBID( name );
    if (ibID != -1)
    {
        if( m_IBuffers[ibID].m_pBuffer)
        {
            return ibID;
        }
    }
    else
    {
        m_IBuffers.push_back( IBInstance() );
        ibID = m_IBuffers.size() - 1;
    }
    IBInstance& ib = m_IBuffers[ibID];

    uint32_t usage = D3DUSAGE_WRITEONLY;
    if (!bStatic)
    {
        usage |= D3DUSAGE_DYNAMIC;
    }

    D3DPOOL pool = D3DPOOL_DEFAULT;
    IDirect3DIndexBuffer9* pBuffer = NULL;
    HRESULT hRes = m_pDevice->CreateIndexBuffer( numBytes, usage, D3DFMT_INDEX16, pool, &pBuffer, NULL );
    if (hRes != S_OK) return -1;
    ib.m_Name           = name;
    ib.m_pBuffer        = pBuffer;
    ib.m_bDynamic       = !bStatic;
    ib.m_CurIteration++;
    ib.m_CurLastByte    = 0;
    ib.m_Size           = numBytes;
    return ibID;*/
    return 0;
}

int JRenderServerGL::GetIBID( const char* name )
{
    //for (int i = 0; i < m_IBuffers.size(); i++)
    //{
    //    if (!stricmp( name, m_IBuffers[i].m_Name.c_str() )) return i;
    //}
    return -1;
}

bool JRenderServerGL::UnlockIB( int ibID )
{
    //if (ibID < 0 || ibID >= m_IBuffers.size()) return false;
    //IDirect3DIndexBuffer9* pBuffer = m_IBuffers[ibID].m_pBuffer;
    //if (!pBuffer) return NULL;
    //HRESULT hRes = pBuffer->Unlock();
    //return (hRes == S_OK);
    return false;
}

bool JRenderServerGL::SetIB( int ibID, int baseIndex )
{
    //if (ibID < 0 || ibID >= m_IBuffers.size()) return false;
    //IDirect3DIndexBuffer9* pBuffer = m_IBuffers[ibID].m_pBuffer;
    //if (!pBuffer) return false;
    //HRESULT hRes = m_pDevice->SetIndices( pBuffer );
    //m_BaseIndex = baseIndex;
    //return (hRes == S_OK);
    return false;
}

bool JRenderServerGL::DeleteIB( int ibID )
{
    /*if (ibID < 0 || ibID >= m_IBuffers.size()) return false;
    m_IBuffers[ibID].m_pBuffer->Release();
    m_IBuffers[ibID].m_pBuffer = NULL;
    return true;*/
    return false;
}

bool JRenderServerGL::Draw( int firstIdx, int numIdx, int firstVert, int numVert, PrimitiveType primType )
{
    //if (numVert == 0) return false;
    //
    ////  FIXME: for each draw call?..
    //SetGlobalConstants();

    //bool bIndexed   = true;
    //int numPri      = 0;
    //if (numIdx == 0)
    //{
    //    numPri = GetNumPrimitives( primType, numVert );
    //    bIndexed = false;
    //}
    //else
    //{
    //    numPri = GetNumPrimitives( primType, numIdx );
    //}

    //if (primType == PrimitiveType_QuadList)
    //{
    //    numPri      = numVert/2;
    //    bIndexed    = true;
    //    firstIdx    = 0;
    //    SetIB( m_QuadIB, 0 );
    //}

    //HRESULT hRes = S_OK;
    //hRes = m_pDevice->BeginScene();
    //if (bIndexed)
    //{
    //    hRes = m_pDevice->DrawIndexedPrimitive( ConvertPrimitiveType( primType ), firstVert, 0, numVert, firstIdx, numPri );
    //}
    //else
    //{
    //    hRes = m_pDevice->DrawPrimitive( ConvertPrimitiveType( primType ), firstVert, numPri );
    //}
    //hRes = m_pDevice->EndScene();
    //
    //m_DIPsPerFrame++;
    //m_PolysPerFrame += numPri;
    //
    //return (hRes == S_OK);
    return false;
}

int JRenderServerGL::ClearVB( int vbID )
{
    /*if (vbID < 0 || vbID >= m_VBuffers.size()) return false;
    IDirect3DVertexBuffer9* pBuffer = m_VBuffers[vbID].m_pBuffer;
    if (!pBuffer) return false;
    void* pData = NULL;
    HRESULT hRes = pBuffer->Lock( 0, m_VBuffers[vbID].m_Size, &pData, D3DLOCK_DISCARD );
    if (hRes != S_OK) return -1;
    pBuffer->Unlock();
    m_VBuffers[vbID].m_CurIteration++;
    return m_VBuffers[vbID].m_CurIteration;*/
    return -1;
}

int JRenderServerGL::ClearIB( int ibID )
{
    //if (ibID < 0 || ibID >= m_IBuffers.size()) return -1;
    //IDirect3DIndexBuffer9* pBuffer = m_IBuffers[ibID].m_pBuffer;
    //if (!pBuffer) return false;
    //void* pData = NULL;
    //HRESULT hRes = pBuffer->Lock( 0, m_IBuffers[ibID].m_Size, &pData, D3DLOCK_DISCARD );
    //if (hRes != S_OK) return -1;
    //pBuffer->Unlock();
    //m_IBuffers[ibID].m_CurIteration++;
    //return m_IBuffers[ibID].m_CurIteration;
    return 0;
}

int JRenderServerGL::GetVBIteration( int vbID )
{
    //if (vbID < 0 || vbID >= m_VBuffers.size()) return -1;
    //return m_VBuffers[vbID].m_CurIteration;
    return -1;
}

int JRenderServerGL::GetIBIteration( int ibID )
{
    //if (ibID < 0 || ibID >= m_IBuffers.size()) return -1;
    //return m_IBuffers[ibID].m_CurIteration;
    return -1;
}

uint8_t* JRenderServerGL::LockIB( int ibID, int firstByte, int numBytes )
{
    /*if (ibID < 0 || ibID >= m_IBuffers.size()) return NULL;
    uint32_t flags = 0;
    if (m_IBuffers[ibID].m_bDynamic) flags |= D3DLOCK_NOOVERWRITE;
    void* pData = NULL;
    IDirect3DIndexBuffer9* pBuffer = m_IBuffers[ibID].m_pBuffer;
    if (!pBuffer) return NULL;
    HRESULT hRes = pBuffer->Lock( firstByte, numBytes, &pData, flags );
    if (hRes != S_OK) return NULL;
    m_IBuffers[ibID].m_CurLastByte = firstByte + numBytes;
    return (uint8_t*)pData;*/
    return NULL;
}

bool JRenderServerGL::CacheIB( int ibID, uint8_t* pData, int size, int& iteration, int& firstByte )
{
//    if (ibID < 0 || ibID >= m_IBuffers.size()) return -1;
//    IBInstance& ib = m_IBuffers[ibID];
//    if (ib.m_CurIteration == iteration) return true;
//
//    //  data is not in buffer - try to add it there
//    if (ib.m_Size < ib.m_CurLastByte + size)
//    {
//        // no place in buffer - discard it
//        ClearIB( ibID );
//        ib.m_CurLastByte = 0;
//        ib.m_CurIteration++;
//    }
//
//    if (ib.m_Size < size)
//    {
//        rlog.err( "Could not cache %d uint8_ts in index buffer %s", size, ib.m_Name.c_str() );
//        return false;
//    }
//
//    firstByte = ib.m_CurLastByte;
//    iteration = ib.m_CurIteration;
//
//    uint8_t* pBuf = LockIB( ibID, firstByte, size );
//    memcpy( pBuf, pData, size );
//    UnlockIB( ibID );

    return true;
}

uint8_t* JRenderServerGL::LockVB( int vbID, int firstByte, int numBytes )
{/*
    if (vbID < 0 || vbID >= m_VBuffers.size()) return NULL;
    uint32_t flags = 0;
    if (m_VBuffers[vbID].m_bDynamic) flags |= D3DLOCK_NOOVERWRITE;
    void* pData = NULL;
    IDirect3DVertexBuffer9* pBuffer = m_VBuffers[vbID].m_pBuffer;
    if (!pBuffer) return NULL;
    HRESULT hRes = pBuffer->Lock( firstByte, numBytes, &pData, flags );
    if (hRes != S_OK) return NULL;
    m_VBuffers[vbID].m_CurLastByte = firstByte + numBytes;
    return (uint8_t*)pData;*/
    return NULL;
}

bool JRenderServerGL::CacheVB( int vbID, uint8_t* pData, int size, int stride, int& iteration, int& firstByte )
{
//    if (vbID < 0 || vbID >= m_VBuffers.size()) return -1;
//    VBInstance& vb = m_VBuffers[vbID];
//    if (vb.m_CurIteration == iteration) return true;
//
//    //  data is not in buffer - try to add it there
//    if (vb.m_Size < vb.m_CurLastByte + size + stride)
//    {
//        // no place in buffer - discard it
//        ClearVB( vbID );
//        vb.m_CurLastByte = 0;
//        vb.m_CurIteration++;
//    }
//
//    if (vb.m_Size < size)
//    {
//        rlog.err( "Could not cache %d uint8_ts in vertex buffer %s", size, vb.m_Name.c_str() );
//        return false;
//    }
//
//    firstByte = vb.m_CurLastByte;
//    //  align firstByte to stride
//    if (firstByte%stride != 0)
//    {
//        firstByte += stride - firstByte%stride;
//    }
//
//    iteration = vb.m_CurIteration;
//
//    uint8_t* pBuf = LockVB( vbID, firstByte, size );
//    if (!pBuf)
//    {
//        return false;
//    }
//    memcpy( pBuf, pData, size );
//    UnlockVB( vbID );

    return false;
}

bool JRenderServerGL::CopyTexture( int destID, int srcID, const Frame* rct, int nRect )
{
//    if (destID < 0 || destID >= m_Textures.size()) return false;
//    if (srcID < 0 || srcID >= m_Textures.size()) return false;
//
//    IDirect3DTexture9* pSrc = (IDirect3DTexture9*)m_Textures[srcID].m_pTexture;
//    IDirect3DTexture9* pDst = (IDirect3DTexture9*)m_Textures[destID].m_pTexture;
//    if (!pSrc || !pDst) return false;
//
//    if (!rct)
//    {
//        pDst->AddDirtyRect( NULL );
//    }
//
//    for (int i = 0; i < nRect; i++)
//    {
//        RECT wrct;
//        wrct.left    = rct[i].x;
//        wrct.top     = rct[i].y;
//        wrct.right   = rct[i].r();
//        wrct.bottom  = rct[i].b();
//        pDst->AddDirtyRect( &wrct );
//    }
//
//    //  if source is default pool render target
//    if (m_Textures[srcID].m_Prop.m_bRenderTarget)
//    {
//        HRESULT hRes = S_OK;
//        DXSurface* pSrcSurf = NULL;
//        DXSurface* pDstSurf = NULL;
//        hRes &= pSrc->GetSurfaceLevel( 0, &pSrcSurf );
//        hRes &= pDst->GetSurfaceLevel( 0, &pDstSurf );
//        hRes &= m_pDevice->GetRenderTargetData( pSrcSurf, pDstSurf );
//        if (pSrcSurf) pSrcSurf->Release();
//        if (pDstSurf) pDstSurf->Release();
//        return (hRes == S_OK);
//    }
//
//    HRESULT hRes = m_pDevice->UpdateTexture( pSrc, pDst );
//    if (hRes != S_OK)
//    {
//        hRes = S_OK;
//        DXSurface* pSrcSurf = NULL;
//        DXSurface* pDstSurf = NULL;
//        hRes &= pSrc->GetSurfaceLevel( 0, &pSrcSurf );
//        hRes &= pDst->GetSurfaceLevel( 0, &pDstSurf );
//        hRes &= D3DXLoadSurfaceFromSurface( pDstSurf, NULL, NULL, pSrcSurf, NULL, NULL, D3DX_FILTER_NONE, NULL );
//    }
    return false;
}

bool JRenderServerGL::GetTextureProp( int texID, TextureProperties& texProp )
{
    /*if (texID < 0 || texID >= m_Textures.size()) return false;
    texProp = m_Textures[texID].m_Prop;*/
    return false;
}

void JRenderServerGL::Register( IRenderServerClient* pClient )
{
    for (int i = 0; i < m_Clients.size(); i++)
    {
        if (pClient == m_Clients[i]) return;
    }
    m_Clients.push_back( pClient );
}

void JRenderServerGL::Unregister( IRenderServerClient* pClient )
{
    for (int i = 0; i < m_Clients.size(); i++)
    {
        if (pClient == m_Clients[i])
        {
            m_Clients.erase( m_Clients.begin() + i );
            return;
        }
    }
}

void JRenderServerGL::ReleaseDefaultRes()
{/*
    if (false)
    {
        rlog.msg( "BackBuffer: %d", GetRefCount( m_pBackBuffer ) );
        rlog.msg( "DepthStencil: %d", GetRefCount( m_pDepthStencil ) );

        for (int i = 0; i < m_Textures.size(); i++)
        {
            TextureFile& tex = m_Textures[i];
            if (tex.m_Prop.m_PoolType == PoolType_Video)
            {
                rlog.msg( "Texture %i (%s): %d", i, tex.m_Name.c_str(), GetRefCount( tex.m_pTexture ) );
                rlog.msg( "Texture %i (%s) depth stencil: %d", i,
                    tex.m_Name.c_str(), GetRefCount( tex.m_pDepthStencil ) );
            }
        }

        for (int i = 0; i < m_VBuffers.size(); i++)
        {
            rlog.msg( "VBuffer %i (%s): %d", i, m_VBuffers[i].m_Name.c_str(), GetRefCount( m_VBuffers[i].m_pBuffer ) );
        }

        for (int i = 0; i < m_IBuffers.size(); i++)
        {
            rlog.msg( "IBuffer %i (%s): %d", i, m_IBuffers[i].m_Name.c_str(), GetRefCount( m_IBuffers[i].m_pBuffer ) );
        }
    }

    for (int i = 0; i < 8; i++)
    {
        m_pDevice->SetTexture( i, NULL );
    }
    m_CurFVF = 0;

    SAFE_RELEASE( m_pBackBuffer );
    SAFE_RELEASE( m_pDepthStencil );

    for (int i = 0; i < m_Textures.size(); i++)
    {
        TextureFile& tex = m_Textures[i];
        if (tex.m_Prop.m_PoolType == PoolType_Video)
        {
            SAFE_RELEASE( tex.m_pTexture );
            SAFE_RELEASE( tex.m_pDepthStencil );
        }
    }

    for (int i = 0; i < m_VBuffers.size(); i++)
    {
        SAFE_RELEASE( m_VBuffers[i].m_pBuffer );
    }

    for (int i = 0; i < m_IBuffers.size(); i++)
    {
        SAFE_RELEASE( m_IBuffers[i].m_pBuffer );
    }

    for (int i = 0; i < m_Shaders.size(); i++)
    {
        m_Shaders[i].m_pEffect->OnLostDevice();
    }*/
}

void JRenderServerGL::RecreateDefaultRes()
{/*
    m_pDevice->EvictManagedResources();
    m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
    m_pDevice->GetDepthStencilSurface( &m_pDepthStencil );

    for (int i = 0; i < m_VBuffers.size(); i++)
    {
        VBInstance& vb = m_VBuffers[i];
        CreateVB( vb.m_Name.c_str(), vb.m_Size, !vb.m_bDynamic );
    }

    for (int i = 0; i < m_IBuffers.size(); i++)
    {
        IBInstance& ib = m_IBuffers[i];
        CreateIB( ib.m_Name.c_str(), ib.m_Size, !ib.m_bDynamic );
    }

    for (int i = 0; i < m_Textures.size(); i++)
    {
        TextureFile& tex = m_Textures[i];
        if (tex.m_Prop.m_PoolType == PoolType_Video)
        {
            CreateTexture( tex.m_Prop );
        }
    }

    for (int i = 0; i < m_Shaders.size(); i++)
    {
        m_Shaders[i].m_pEffect->OnResetDevice();
    }

    CreateQuadIB();*/

}

void JRenderServerGL::CreateQuadIB()
{
    //  create quad index buffer
    m_QuadIB = CreateIB( "QuadIndexBuffer", c_QuadIBSize, true );
    uint16_t* pQuadIdx = (uint16_t*)LockIB( m_QuadIB, 0, c_QuadIBSize );
    if (!pQuadIdx)
    {
        rlog.err( "JRenderServerGL: Could not create internal quad index buffer." );
        return;
    }
    int curV = 0;
    for (int i = 0; i < c_QuadIBSize/2; i += 6)
    {
        pQuadIdx[i    ] = curV;
        pQuadIdx[i + 1] = curV + 1;
        pQuadIdx[i + 2] = curV + 2;
        pQuadIdx[i + 3] = curV + 2;
        pQuadIdx[i + 4] = curV + 1;
        pQuadIdx[i + 5] = curV + 3;
        curV += 4;
    }
    UnlockIB( m_QuadIB );
}



