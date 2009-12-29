//****************************************************************************/
//  File:  JRenderServer.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "DeviceDX8.h"
#include "JObjectServer.h"
#include "Timer.h"
#include "FStream.h"
#include "IWindowServer.h"
#include "IPersistServer.h"
#include "IFileServer.h"

//  allright, this bullshit is here because d3dx8 is not included with DirectX SDK 
#include "../../../external/d3dx8/include/d3dx8.h"
#pragma comment(lib,"../../external/d3dx8/lib/d3dx8.lib")

#pragma comment(lib,"d3d8.lib")
#pragma comment(lib,"dxerr8.lib")
#pragma comment(lib,"dxguid.lib")

IDirect3DDevice8* GetDirect3DDevice8()
{
    return JRenderServer::s_pInstance->m_pDevice;
}

//****************************************************************************/
/*  JRenderServer
//****************************************************************************/
decl_class(JRenderServer);
JRenderServer* JRenderServer::s_pInstance = NULL;
JRenderServer::JRenderServer()
{
    m_Viewport          = Frame( 0.0f, 0.0f, 800.0f, 600.0f );
    m_pD3D              = NULL;
    m_pDevice           = NULL;
    m_hWnd              = NULL;
    m_CurFrame          = 0;
    m_bInited           = false;
    m_ViewTM            = Mat4::identity;
    m_ProjTM            = Mat4::identity;
    m_WorldTM           = Mat4::identity;
    m_TextureTM         = Mat4::identity;
    m_bCursorVisible    = true;
    m_pBackBuffer       = NULL;
    m_pDepthStencil     = NULL;
    m_bHasStencil       = false;
    m_CurFVF            = 0;
    m_QuadIB            = -1;
    m_Ambience          = 0xFF333333;
    m_RTID              = -1;
    m_DSID              = -1;
    m_bDeviceLost       = false;

    m_FPS               = 0;
    m_bWireFrame        = false;

    g_pRenderServer = this;
    s_pInstance = this;
} // JRenderServer::JRenderServer

JRenderServer::~JRenderServer()
{
    Shut();
    if (g_pRenderServer == this) g_pRenderServer = NULL;
} // JRenderServer::~JRenderServer

void JRenderServer::Init()
{
    m_hWnd = (HWND)g_pWindowServer->GetRootHandle();

    if (m_bInited) return;
    
    SetName( "render" );

    rlog.msg( "Creating d3d8 render device..." );
    m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
    if (m_pD3D == NULL)
    {
        rlog.err( "Could not initialize d3d8." );
        return;
    }
    
    D3DDISPLAYMODE d3ddm;
    m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
    
    ZeroMemory( &m_PresParam, sizeof( m_PresParam ) );

    JScreenMode mode = g_pWindowServer->GetScreenMode();
    BOOL bWindowed = (mode == smWindow || mode == smDesktop);

    bool bHasStencil = true;

    m_PresParam.Windowed                  = bWindowed;
    m_PresParam.SwapEffect                = bWindowed ? D3DSWAPEFFECT_COPY_VSYNC : D3DSWAPEFFECT_FLIP;
    m_PresParam.EnableAutoDepthStencil    = TRUE;
    m_PresParam.AutoDepthStencilFormat    = D3DFMT_D16;
    m_PresParam.BackBufferFormat		  = d3ddm.Format;
    m_PresParam.BackBufferCount           = 1;
    m_PresParam.hDeviceWindow             = m_hWnd;

    if (bHasStencil) 
    {
        m_PresParam.AutoDepthStencilFormat = D3DFMT_D24S8;
        m_bHasStencil = true;
    }

    RECT cRect;
    GetClientRect( m_hWnd, &cRect );

    m_PresParam.BackBufferWidth        = cRect.right - cRect.left;
    m_PresParam.BackBufferHeight       = cRect.bottom - cRect.top;

    DWORD flags = D3DCREATE_MIXED_VERTEXPROCESSING;
    D3DDEVTYPE devType = D3DDEVTYPE_HAL;

    rlog.msg( "Creating d3d8 render device..." );

    /*
	//  fixme: this takes precious startup time
    D3DADAPTER_IDENTIFIER8 adapterID;
    m_pD3D->GetAdapterIdentifier( 0, 0, &adapterID );
    rlog.msg( "Adapter: %s", adapterID.Description );
    rlog.msg( "Driver: %s, product: %d, ver: %d, subver: %d, build: %d", 
                adapterID.Driver, 
                HIWORD(adapterID.DriverVersion.HighPart),
                LOWORD(adapterID.DriverVersion.HighPart),
                HIWORD(adapterID.DriverVersion.LowPart),
                LOWORD(adapterID.DriverVersion.LowPart) );

    rlog.msg( "Vendor ID: %d, Device ID: %d, Subsystem ID: %d, Revision: %d", 
                adapterID.VendorId, 
                adapterID.DeviceId,
                adapterID.SubSysId,
                adapterID.Revision );
    */

    // Create the D3DDevice
    HRESULT res = m_pD3D->CreateDevice(	0, devType, m_hWnd, flags, &m_PresParam, &m_pDevice ); 
    if (!m_pDevice) 
    {
        rlog.warn( "Could not create render device. Switching to software vertex processing." );
        
        flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        res = m_pD3D->CreateDevice(	0, devType, m_hWnd, flags, &m_PresParam, &m_pDevice );
        
        if (!m_pDevice)
        {
            rlog.err( "Could not create render device. %s", GetDXError( res ) );
            return;
        }
    }

    rlog.msg( "Render device created OK." );
    
    SAFE_RELEASE( m_pBackBuffer );
    SAFE_RELEASE( m_pDepthStencil );
    m_pDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
    m_pDevice->GetDepthStencilSurface( &m_pDepthStencil );

    m_bInited = true;

    //  register stock vertex types
    for (int vtype = VertexType_Unknown; vtype < VertexType_Last; vtype++)
    {
        int typeID = GetVDeclID( GetStockVDecl( (VertexType)vtype ) );
        assert( typeID == vtype );
    }

    CreateQuadIB();
} // JRenderServer::Init

void JRenderServer::Register( IRenderServerClient* pClient )
{
    for (int i = 0; i < m_Clients.size(); i++)
    {
        if (pClient == m_Clients[i]) return;
    }
    m_Clients.push_back( pClient );
} // JRenderServer::Register

void JRenderServer::Unregister( IRenderServerClient* pClient )
{
    for (int i = 0; i < m_Clients.size(); i++)
    {
        if (pClient == m_Clients[i]) 
        {
            m_Clients.erase( m_Clients.begin() + i );
            return;
        }
    }
} // JRenderServer::Unregister

bool JRenderServer::StartFrame()
{
    if (!m_bInited || !m_pDevice) 
    {
        return false;
    }

    if (m_bDeviceLost)
    {
        //  test the cooperative level to see if it's okay to render
        HRESULT hRes = m_pDevice->TestCooperativeLevel();
        if (hRes != S_OK)
        {
            //  if the device was lost, do not render until we get it back
            if (hRes == D3DERR_DEVICELOST) 
            {
                return false;
            }

            //  check if the device needs to be reset
            if (hRes == D3DERR_DEVICENOTRESET)
            {
                ReleaseDefaultRes();
                hRes = m_pDevice->Reset( &m_PresParam );
                if (hRes == S_OK)
                {
                    RecreateDefaultRes();
                    //  invalidate all device resource clients
                    for (int i = 0; i < m_Clients.size(); i++)
                    {
                        m_Clients[i]->OnResetDevice();
                    }
                }
            }
            return false;
        }
        m_bDeviceLost = false;
    }

    return true;
} // JRenderServer::StartFrame

void JRenderServer::EndFrame()
{
    if (!m_pDevice) return;
    m_CurFrame++;
    HRESULT hRes = m_pDevice->Present( NULL, NULL, NULL, NULL );
    if (hRes == D3DERR_DEVICELOST)
    {
        m_bDeviceLost = true;
        return;
    }

    if (GetKeyState( 'Q' ) < 0 && GetKeyState( VK_CONTROL ) < 0)
    {
        for (int i = 0; i < m_Shaders.size(); i++)
        {
            m_Shaders[i] = obj_cast<Shader>( g_pPersistServer->LoadObject( m_Shaders[i]->GetName(), 
                m_Shaders[i] ) );
            if (m_Shaders[i]) m_Shaders[i]->InitTree();
        }
    }

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

} // JRenderServer::EndFrame

bool JRenderServer::IsFullScreen() const
{
    return m_PresParam.Windowed ? false : true;
}

void JRenderServer::SetFullScreen( bool bFullScreen )
{
    bool bCurFullScreen = m_PresParam.Windowed ? false : true;
    
    if (bCurFullScreen == bFullScreen)
    {
        return;
    }

    g_pWindowServer->DestroyWnd();
    g_pWindowServer->CreateWnd( bFullScreen ? smFullscreen : smWindow );
    m_hWnd = (HWND)g_pWindowServer->GetRootHandle();
    m_PresParam.hDeviceWindow = m_hWnd;

    m_PresParam.Windowed   = !bFullScreen;
    m_PresParam.SwapEffect = bFullScreen ? D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_COPY;

    ReleaseDefaultRes();
    HRESULT hRes = m_pDevice->Reset( &m_PresParam );
    if (hRes == S_OK)
    {
        RecreateDefaultRes();
        //  invalidate all device resource clients
        for (int i = 0; i < m_Clients.size(); i++)
        {
            m_Clients[i]->OnResetDevice();
        }
    }
}

void JRenderServer::SetWireFrame( bool bWireFrame )
{
    m_bWireFrame = bWireFrame; 
    m_pDevice->SetRenderState( D3DRS_FILLMODE, m_bWireFrame ? D3DFILL_WIREFRAME : D3DFILL_SOLID );
}

void JRenderServer::ClearViewport( DWORD color )
{
    if (!m_pDevice) return;
    m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, color, 1.0, 0 );
} // JRenderServer::ClearViewport

void JRenderServer::ClearDepthStencil( bool bDepth, bool bStencil )
{
    if (!m_pDevice) return;
    DWORD flags = 0;
    if (bDepth)   flags |= D3DCLEAR_ZBUFFER;
    if (bStencil && m_bHasStencil) flags |= D3DCLEAR_STENCIL;
    m_pDevice->Clear( 0, NULL, flags, 0, 1.0, 0 );
}  // JRenderServer::ClearDepthBuffer

void JRenderServer::SetViewport( const Frame& rc )
{
    if (!m_pDevice) return;

    //  clip viewport to the render target size
    Frame vp = rc;
    Frame rcRT( Frame::null );
    if (m_RTID == -1)
    {
        rcRT.w = float( m_PresParam.BackBufferWidth  );
        rcRT.h = float( m_PresParam.BackBufferHeight );
    }
    else
    {
        rcRT.w = float( m_Textures[m_RTID].m_Prop.m_Width  );
        rcRT.h = float( m_Textures[m_RTID].m_Prop.m_Height );
    }

    vp.Clip( rcRT );
    if (vp.w < 1.0f || vp.h < 1.0f)
    {
        return;
    }

    m_Viewport = vp;
    D3DVIEWPORT8 dvp;
    dvp.X		= m_Viewport.x;
    dvp.Y		= m_Viewport.y;
    dvp.Width	= m_Viewport.w;
    dvp.Height	= m_Viewport.h;
    dvp.MinZ	= 0.0f;
    dvp.MaxZ	= 1.0f;
    m_pDevice->SetViewport( (D3DVIEWPORT8*)&dvp );	
} // JRenderServer::SetViewport

bool JRenderServer::Shut()
{
    if (!m_bInited) return false;

    rlog.msg( "Releasing d3d8 render device..." );
    if (!m_bInited) return false;
    
    SAFE_RELEASE( m_pBackBuffer );
    SAFE_RELEASE( m_pDepthStencil );

    for (int i = 0; i < m_Textures.size(); i++) 
    {
        SAFE_RELEASE( m_Textures[i].m_pTexture );
        SAFE_RELEASE( m_Textures[i].m_pDepthStencil );
    }
    for (int i = 0; i < m_VBuffers.size(); i++) SAFE_RELEASE( m_VBuffers[i].m_pBuffer );
    for (int i = 0; i < m_IBuffers.size(); i++) SAFE_RELEASE( m_IBuffers[i].m_pBuffer );

    int nRef = 0;
    if (m_pDevice) nRef = m_pDevice->Release();
    
    if (nRef > 0) rlog.err( "Render device was not released properly." );

    SAFE_RELEASE( m_pD3D );
    m_bInited = false;
    return true;
} // JRenderServer::Shut

void JRenderServer::SetViewTM( const Mat4& tm )
{
    if (!m_pDevice) return;
    m_ViewTM = tm;
    m_pDevice->SetTransform( D3DTS_VIEW, (D3DMATRIX*)&tm );
} // JRenderServer::SetProjTM

void JRenderServer::SetWorldTM( const Mat4& tm )
{
    if (!m_pDevice) return;
    m_WorldTM = tm;
    m_pDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&tm );
} // JRenderServer::SetProjTM

void JRenderServer::SetProjTM( const Mat4& tm )
{
    if (!m_pDevice) return;
    m_ProjTM = tm;
    m_pDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&tm );
} // JRenderServer::SetProjTM

void JRenderServer::SetTextureTM( const Mat4& tm )
{
    if (!m_pDevice) return;
    m_TextureTM = tm;
    m_pDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX*)&tm );
} // JRenderServer::SetTextureTM

int JRenderServer::GetTextureID( const char* texName )
{
    if (!texName || !m_pDevice) return -1;
    for (int i = 0; i < m_Textures.size(); i++)
    {
        if (!stricmp( m_Textures[i].m_Name.c_str(), texName )) return i;
    }

    FInStream is;
    g_pFileServer->OpenMedia( texName, "", is );
    if (!is) return -1;

    int nBytes = is.GetTotalSize();
    BYTE* buf = new BYTE[nBytes];
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
        IDirect3DCubeTexture8* pCubeTexture = NULL;
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
    return m_Textures.size() - 1;
} // JRenderServer::GetTextureID

void JRenderServer::SetTexture( int texID, int stage )
{
    if (texID < 0 || texID >= (int)m_Textures.size()) 
    {
        m_pDevice->SetTexture( stage, NULL );
        return;
    }
    m_pDevice->SetTexture( stage, m_Textures[texID].m_pTexture );
} // JRenderServer::SetTexture

void JRenderServer::DeleteTexture( int texID )
{
    if (texID < 0 || texID >= (int)m_Textures.size()) return;
    DXBaseTexture* pTexture = m_Textures[texID].m_pTexture;
    if (!pTexture) return;
    pTexture->Release();
    m_Textures[texID].m_pTexture = NULL;
    m_Textures[texID].m_Prop.m_Width = 0;
    m_Textures[texID].m_Prop.m_Height = 0;
} // JRenderServer::DeleteTexture

void JRenderServer::ShowCursor( bool bShow )
{
    if (bShow == m_bCursorVisible) return;
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
    m_bCursorVisible = bShow;
} // JRenderServer::ShowCursor

int JRenderServer::CreateRenderTarget( const char* texName, int w, int h, ColorFormat fmt )
{                     
    int texID = GetTextureID( texName );
    if (texID != -1)
    {
        return texID;
    }

    DWORD usage = D3DUSAGE_RENDERTARGET;
    DXTexture* pTex = NULL;
    HRESULT hRes = m_pDevice->CreateTexture( w, h, 1, usage, ConvertColorFormat( fmt ), D3DPOOL_DEFAULT, &pTex );
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
    tp.m_bRenderTarget  = true;
    tp.m_PoolType       = ConvertPoolType( desc.Pool );
    tp.m_NMips          = pTex->GetLevelCount();
    strcpy( tp.m_Name, texName );


    tex.m_pTexture      = pTex;
    tex.m_pDepthStencil = NULL;
    tex.m_Name          = texName;
    m_Textures.push_back( tex );
    return m_Textures.size() - 1;
} // JRenderServer::CreateRenderTarget

int JRenderServer::CreateDepthStencil( const char* texName, int w, int h, DepthFormat fmt )
{
    DXSurface* pSurface = NULL;
    HRESULT hRes = m_pDevice->CreateDepthStencilSurface( w, h, ConvertDepthFormat( fmt ), 
        D3DMULTISAMPLE_NONE, &pSurface );
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
    return m_Textures.size() - 1;
} // JRenderServer::CreateDepthStencil


bool JRenderServer::SetRenderTarget( int texID, int dsID )
{
    if (texID < 0 || texID >= (int)m_Textures.size()) 
    {
        //  restore backbuffer render target
        HRESULT res = m_pDevice->SetRenderTarget( m_pBackBuffer, m_pDepthStencil );
        m_RTID = -1;
        m_DSID = -1;
        return (res == S_OK);
    }
    DXTexture* pTex = (DXTexture*)m_Textures[texID].m_pTexture;
    if (!pTex) return false;
    DXSurface* pSurf = NULL;
    pTex->GetSurfaceLevel( 0, &pSurf );
    pSurf->Release();

    DXSurface* pDepth = NULL;
    if (dsID >= 0 && dsID < (int)m_Textures.size()) 
    {
        pDepth = (DXSurface*)m_Textures[dsID].m_pDepthStencil;
    }

    m_RTID = texID;
    m_DSID = dsID;
    
    HRESULT res = m_pDevice->SetRenderTarget( pSurf, pDepth );
    return (res == S_OK);
} // JRenderServer::SetRenderTarget

bool JRenderServer::SaveTexture( int texID, const char* fname )
{
    if (texID < 0 || texID >= (int)m_Textures.size()) return false;
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
    return (hRes == S_OK);
} // JRenderServer::SaveTexture

void JRenderServer::SetAmbient( DWORD color )
{
    m_Ambience = color;
    IDirect3DDevice8* pDevice = GetDirect3DDevice8();
    pDevice->SetRenderState( D3DRS_AMBIENT, color );
} // JRenderServer::SetAmbient

int JRenderServer::AddPointLight( const Vec3& pos, float radius, float falloff, 
                               DWORD diffuse, DWORD specular, bool bPerPixel )
{
    return -1;
} // JRenderServer::AddPointLight

int JRenderServer::AddDirLight( const Vec3& dir, DWORD diffuse, DWORD specular, bool bPerPixel )
{
    D3DLIGHT8 light;
    light.Type		    = D3DLIGHT_DIRECTIONAL ;            
    light.Diffuse	    = ConvertColor( diffuse  );
    light.Specular	    = ConvertColor( specular );        
    light.Ambient	    = ConvertColor( 0xFF000000 );   

    Vec3 ldir( dir.x, dir.y, dir.z );
    ldir.normalize();
    light.Direction.x	= ldir.x; 
    light.Direction.y	= ldir.y;
    light.Direction.z	= ldir.z;

    light.Range			= 100.0f;
    light.Position.x	= 0.0f; 
    light.Position.y	= 0.0f;
    light.Position.z	= 0.0f;

    light.Attenuation0	= 0.0f;
    light.Attenuation1	= 0.0f;
    light.Attenuation2	= 0.0f;

    light.Falloff		= 0.0f;
    light.Theta			= c_PI;
    light.Phi			= c_PI;

    IDirect3DDevice8* pDevice = GetDirect3DDevice8();
    pDevice->LightEnable( 0, TRUE );
    pDevice->SetLight   ( 0, &light );
    
    return 0;
} // JRenderServer::AddDirLight

int JRenderServer::GetShaderID( const char* name, bool bReload )
{
    for (int i = 0; i < (int)m_Shaders.size(); i++)
    {
        if (m_Shaders[i] && !stricmp( m_Shaders[i]->GetName(), name )) return i; 
    }
    Shader* pShader = obj_cast<Shader>( g_pPersistServer->LoadObject( name, NULL ) );
    if (!pShader) return -1;
    pShader->SetName( name );
    pShader->InitTree();
    m_Shaders.push_back( pShader );
    return m_Shaders.size() - 1;
} // JRenderServer::GetShaderID

void JRenderServer::SetShader( int shID )
{
    if (shID < 0 || shID >= (int)m_Shaders.size()) return;
    m_Shaders[shID]->Render();

    if (m_bWireFrame)
    {
        m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
    }
} // JRenderServer::SetShader

int JRenderServer::CreateTexture( const TextureProperties& texProp )
{
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

    DWORD usage = 0;
    D3DFORMAT format = ConvertColorFormat( texProp.m_Format );
    D3DPOOL pool = ConvertPoolType( texProp.m_PoolType );
    IDirect3DTexture8* pTexture = NULL;

    if (texProp.m_bRenderTarget) 
    {
        usage = D3DUSAGE_RENDERTARGET;
    }

    if (pool == D3DPOOL_DEFAULT)
    {
        m_pDevice->ResourceManagerDiscardBytes( 0 );
    }

    HRESULT hRes = m_pDevice->CreateTexture( texProp.m_Width, texProp.m_Height, 
                              texProp.m_NMips, usage, format, pool, &pTexture );
    if (hRes != S_OK)
    {
        rlog.err( "Could not reate texture %s (width=%d, height=%d).", 
            texProp.m_Name, texProp.m_Width, texProp.m_Height );
        return -1;
    }
    
    TextureFile& tex = m_Textures[texID];

    tex.m_Prop          = texProp;
    tex.m_pTexture      = pTexture;
    tex.m_pDepthStencil = NULL;
    tex.m_Name          = texProp.m_Name;
    return texID;
} // JRenderServer::CreateTexture

BYTE* JRenderServer::LockTexture( int texID, const Frame& rect, int level, DWORD* pStride )
{
    if (texID < 0 || texID >= (int)m_Textures.size()) return NULL;
    D3DLOCKED_RECT dRect;
    RECT wrct;
    wrct.left   = rect.x;
    wrct.top    = rect.y;
    wrct.right  = rect.r();
    wrct.bottom = rect.b();
    DWORD flags = 0;

    DXTexture* pTexture = (DXTexture*)m_Textures[texID].m_pTexture;
    if (!pTexture) return NULL;

    HRESULT hRes = pTexture->LockRect( level, &dRect, &wrct, flags );
    if (hRes != S_OK) return NULL;
    if (pStride) *pStride = dRect.Pitch;
    return (BYTE*)dRect.pBits;
} // JRenderServer::LockTexture

bool JRenderServer::UnlockTexture( int texID, int level )
{
    if (texID < 0 || texID >= (int)m_Textures.size()) return false;
    DXTexture* pTexture = (DXTexture*)m_Textures[texID].m_pTexture;
    if (!pTexture) return NULL;
    HRESULT hRes = pTexture->UnlockRect( level );
    return (hRes == S_OK);
} // JRenderServer::UnlockTexture

int JRenderServer::GetVDeclID( const VertexDeclaration& vdecl )
{
    for (int i = 0; i < m_VertexDeclarations.size(); i++)
    {
        const VDeclInstance& vd = m_VertexDeclarations[i];
        if (vd.m_VDecl == vdecl) return i;
    }
    VDeclInstance vd;
    vd.m_VDecl = vdecl;
    vd.m_FVF = CreateFVF( vdecl );
    m_VertexDeclarations.push_back( vd );
    return m_VertexDeclarations.size() - 1;
} // JRenderServer::GetVDeclID

int JRenderServer::CreateVB( const char* name, int numBytes, bool bStatic )
{
    int vbID = GetVBID( name );
    if (vbID != -1)
    {
        if( m_VBuffers[vbID].m_pBuffer)
        {
            return vbID;
        }
    }
    else
    {
        m_VBuffers.push_back( VBInstance() );
        vbID = m_VBuffers.size() - 1;
    }

    VBInstance& vb = m_VBuffers[vbID];

    DWORD usage = D3DUSAGE_WRITEONLY;
    if (!bStatic) 
    {
        usage |= D3DUSAGE_DYNAMIC;
    }

    D3DPOOL pool = D3DPOOL_DEFAULT;
    DWORD fvf = CreateFVF( GetStockVDecl( VertexType_XYZWDSUV ) );
    IDirect3DVertexBuffer8* pBuffer = NULL;
    HRESULT hRes = m_pDevice->CreateVertexBuffer( numBytes, usage, 0, pool, &pBuffer );
    if (hRes != S_OK) return -1;
    vb.m_Name           = name;
    vb.m_pBuffer        = pBuffer;
    vb.m_bDynamic       = !bStatic;
    vb.m_CurIteration++;
    vb.m_CurLastByte    = 0;
    vb.m_Size           = numBytes;
    return vbID;
} // JRenderServer::CreateVB

int JRenderServer::GetVBID( const char* name )
{
    for (int i = 0; i < m_VBuffers.size(); i++)
    {
        if (!stricmp( name, m_VBuffers[i].m_Name.c_str() )) return i;
    }
    return -1;
} // JRenderServer::GetVBID

bool JRenderServer::UnlockVB( int vbID )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return false;
    IDirect3DVertexBuffer8* pBuffer = m_VBuffers[vbID].m_pBuffer;
    if (!pBuffer) return NULL;
    HRESULT hRes = pBuffer->Unlock();
    return (hRes == S_OK);
} // JRenderServer::UnlockVB

bool JRenderServer::SetVB( int vbID, int vdeclID, int stream )
{
    if (vbID < 0 || vbID >= m_VBuffers.size() || 
        vdeclID < 0 || vdeclID >= m_VertexDeclarations.size()) return false;
    IDirect3DVertexBuffer8* pBuffer = m_VBuffers[vbID].m_pBuffer;
    if (!pBuffer) return false;
    const VDeclInstance& vdecl = m_VertexDeclarations[vdeclID];
    HRESULT hRes = S_OK;
    if (vdecl.m_FVF != m_CurFVF) 
    {
        m_pDevice->SetVertexShader( vdecl.m_FVF );
        m_CurFVF = vdecl.m_FVF;
        if (hRes != S_OK) return false;
    }
    hRes = m_pDevice->SetStreamSource( stream, pBuffer, vdecl.m_VDecl.m_VertexSize );
    return (hRes == S_OK);
} // JRenderServer::SetVB

bool JRenderServer::DeleteVB( int vbID )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return false;
    m_VBuffers[vbID].m_pBuffer->Release();
    m_VBuffers[vbID].m_pBuffer = NULL;
    return true;
} // JRenderServer::DeleteVB

int JRenderServer::CreateIB( const char* name, int numBytes, bool bStatic )
{
    int ibID = GetIBID( name );
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

    DWORD usage = D3DUSAGE_WRITEONLY;
    if (!bStatic) 
    {
        usage |= D3DUSAGE_DYNAMIC;
    }

    D3DPOOL pool = D3DPOOL_DEFAULT;
    IDirect3DIndexBuffer8* pBuffer = NULL;
    HRESULT hRes = m_pDevice->CreateIndexBuffer( numBytes, usage, D3DFMT_INDEX16, pool, &pBuffer );
    if (hRes != S_OK) return -1;
    ib.m_Name           = name;
    ib.m_pBuffer        = pBuffer;
    ib.m_bDynamic       = !bStatic;
    ib.m_CurIteration++;
    ib.m_CurLastByte    = 0;
    ib.m_Size           = numBytes;
    return ibID;
} // JRenderServer::CreateIB

int JRenderServer::GetIBID( const char* name )
{
    for (int i = 0; i < m_IBuffers.size(); i++)
    {
        if (!stricmp( name, m_IBuffers[i].m_Name.c_str() )) return i;
    }
    return -1;
} // JRenderServer::GetIBID

bool JRenderServer::UnlockIB( int ibID )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return false;
    IDirect3DIndexBuffer8* pBuffer = m_IBuffers[ibID].m_pBuffer;
    if (!pBuffer) return NULL;
    HRESULT hRes = pBuffer->Unlock();
    return (hRes == S_OK);
} // JRenderServer::UnlockIB

bool JRenderServer::SetIB( int ibID, int baseIndex )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return false;
    IDirect3DIndexBuffer8* pBuffer = m_IBuffers[ibID].m_pBuffer;
    if (!pBuffer) return false;
    HRESULT hRes = m_pDevice->SetIndices( pBuffer, baseIndex );
    return (hRes == S_OK);
} // JRenderServer::SetIB

bool JRenderServer::DeleteIB( int ibID )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return false;
    m_IBuffers[ibID].m_pBuffer->Release();
    m_IBuffers[ibID].m_pBuffer = NULL;
    return true;
    return false;
} // JRenderServer::DeleteIB

void JRenderServer::SetColorTint( DWORD color )
{
    m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, color );
} // JRenderServer::SetColorTint

bool JRenderServer::Draw( int firstIdx, int numIdx, int firstVert, int numVert, PrimitiveType primType )
{
    if (numVert == 0) return false;
    bool bIndexed   = true; 
    int numPri      = 0;
    if (numIdx == 0)
    {
        numPri = GetNumPrimitives( primType, numVert );
        bIndexed = false;
    }
    else
    {
        numPri = GetNumPrimitives( primType, numIdx );
    }

    if (primType == PrimitiveType_QuadList)
    {
        numPri      = numVert/2;
        bIndexed    = true;
        firstIdx    = 0;
        SetIB( m_QuadIB, firstVert );
    }

    HRESULT hRes = m_pDevice->BeginScene();
    if (hRes != S_OK) return false;
    if (bIndexed)
    {
        hRes = m_pDevice->DrawIndexedPrimitive( ConvertPrimitiveType( primType ), 0, numVert, firstIdx, numPri );
    }
    else
    {
        hRes = m_pDevice->DrawPrimitive( ConvertPrimitiveType( primType ), firstVert, numPri );
    }
    hRes = m_pDevice->EndScene();

    if (hRes != S_OK) return false;
    return (hRes == S_OK);
} // JRenderServer::Draw

int JRenderServer::ClearVB( int vbID )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return false;
    IDirect3DVertexBuffer8* pBuffer = m_VBuffers[vbID].m_pBuffer;
    if (!pBuffer) return false;
    BYTE* pData = NULL;
    HRESULT hRes = pBuffer->Lock( 0, m_VBuffers[vbID].m_Size, &pData, D3DLOCK_DISCARD );
    if (hRes != S_OK) return -1;
    pBuffer->Unlock();
    m_VBuffers[vbID].m_CurIteration++;
    return m_VBuffers[vbID].m_CurIteration;
    return -1;
} // JRenderServer::ClearVB

int JRenderServer::ClearIB( int ibID )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return -1;
    IDirect3DIndexBuffer8* pBuffer = m_IBuffers[ibID].m_pBuffer;
    if (!pBuffer) return false;
    BYTE* pData = NULL;
    HRESULT hRes = pBuffer->Lock( 0, m_IBuffers[ibID].m_Size, &pData, D3DLOCK_DISCARD );
    if (hRes != S_OK) return -1;
    pBuffer->Unlock();
    m_IBuffers[ibID].m_CurIteration++;
    return m_IBuffers[ibID].m_CurIteration;
} // JRenderServer::ClearIB


int JRenderServer::GetVBIteration( int vbID )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return -1;
    return m_VBuffers[vbID].m_CurIteration;
} // JRenderServer::GetVBIteration

int JRenderServer::GetIBIteration( int ibID )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return -1;
    return m_IBuffers[ibID].m_CurIteration;
} // JRenderServer::GetIBIteration

BYTE* JRenderServer::LockIB( int ibID, int firstByte, int numBytes )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return NULL;
    DWORD flags = 0;
    if (m_IBuffers[ibID].m_bDynamic) flags |= D3DLOCK_NOOVERWRITE;
    BYTE* pData = NULL;
    IDirect3DIndexBuffer8* pBuffer = m_IBuffers[ibID].m_pBuffer;
    if (!pBuffer) return NULL;
    HRESULT hRes = pBuffer->Lock( firstByte, numBytes, &pData, flags );
    if (hRes != S_OK) return NULL;
    m_IBuffers[ibID].m_CurLastByte = firstByte + numBytes;
    return (BYTE*)pData;
} // JRenderServer::LockIB

bool JRenderServer::CacheIB( int ibID, BYTE* pData, int size, int& iteration, int& firstByte )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return -1;
    IBInstance& ib = m_IBuffers[ibID];
    if (ib.m_CurIteration == iteration) return true;

    //  data is not in buffer - try to add it there
    if (ib.m_Size < ib.m_CurLastByte + size)
    {
        // no place in buffer - discard it
        ClearIB( ibID );
        ib.m_CurLastByte = 0;
        ib.m_CurIteration++;
    }

    if (ib.m_Size < size) 
    {
        rlog.err( "Could not cache %d bytes in index buffer %s", size, ib.m_Name.c_str() );
        return false;
    }
    
    firstByte = ib.m_CurLastByte;
    iteration = ib.m_CurIteration;

    BYTE* pBuf = LockIB( ibID, firstByte, size );
    memcpy( pBuf, pData, size );
    UnlockIB( ibID );

    return true;
} // JRenderServer::CacheIB

BYTE* JRenderServer::LockVB( int vbID, int firstByte, int numBytes )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return NULL;
    DWORD flags = 0;
    if (m_VBuffers[vbID].m_bDynamic) flags |= D3DLOCK_NOOVERWRITE;
    BYTE* pData = NULL;
    IDirect3DVertexBuffer8* pBuffer = m_VBuffers[vbID].m_pBuffer;
    if (!pBuffer) return NULL;
    HRESULT hRes = pBuffer->Lock( firstByte, numBytes, &pData, flags );
    if (hRes != S_OK) return NULL;
    m_VBuffers[vbID].m_CurLastByte = firstByte + numBytes;
    return (BYTE*)pData;
} // JRenderServer::LockVB

bool JRenderServer::CacheVB( int vbID, BYTE* pData, int size, int stride, int& iteration, int& firstByte )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return -1;
    VBInstance& vb = m_VBuffers[vbID];
    if (vb.m_CurIteration == iteration) return true;

    //  data is not in buffer - try to add it there
    if (vb.m_Size < vb.m_CurLastByte + size + stride)
    {
        // no place in buffer - discard it
        ClearVB( vbID );
        vb.m_CurLastByte = 0;
        vb.m_CurIteration++;
    }

    if (vb.m_Size < size) 
    {
        rlog.err( "Could not cache %d bytes in vertex buffer %s", size, vb.m_Name.c_str() );
        return false;
    }
    
    firstByte = vb.m_CurLastByte;

    //  align firstByte to stride
    if (firstByte%stride != 0)
    {
        firstByte += stride - firstByte%stride;
    }

    iteration = vb.m_CurIteration;

    BYTE* pBuf = LockVB( vbID, firstByte, size );
    if (!pBuf)
    {
        rlog.err( "Could not lock vertex buffer, id:%d", vbID );
        return false;
    }
    memcpy( pBuf, pData, size );
    UnlockVB( vbID );

    return true;
} // JRenderServer::CacheVB

bool JRenderServer::CopyTexture( int destID, int srcID, const Frame* rct, int nRect ) 
{ 
    if (destID < 0 || destID >= m_Textures.size()) return false;
    if (srcID < 0 || srcID >= m_Textures.size()) return false;
    
    DXTexture* pSrcTex = (DXTexture*)m_Textures[srcID].m_pTexture;
    DXSurface* pSrc = NULL; 
    pSrcTex->GetSurfaceLevel( 0, &pSrc );

    DXTexture* pDstTex = (DXTexture*)m_Textures[destID].m_pTexture;
    DXSurface* pDst = NULL; 
    pDstTex->GetSurfaceLevel( 0, &pDst );

    if (!pSrc || !pDst) return false;

    const int c_MaxRect = 256;
    if (nRect > c_MaxRect) nRect = c_MaxRect;
    RECT wrct[c_MaxRect];
    for (int i = 0; i < nRect; i++)
    {
        wrct[i].left    = rct[i].x;
        wrct[i].top     = rct[i].y;
        wrct[i].right   = rct[i].r();
        wrct[i].bottom  = rct[i].b();
    }

    if (nRect == 0)
    {
        nRect = 1;
        D3DSURFACE_DESC desc;
        pDst->GetDesc( &desc );
        wrct[0].left    = 0;
        wrct[0].top     = 0;
        wrct[0].right   = desc.Width;
        wrct[0].bottom  = desc.Height;
    }

    HRESULT hRes = m_pDevice->CopyRects( pSrc, wrct, nRect, pDst, NULL );

    pSrc->Release();
    pDst->Release();
    return (hRes == S_OK);
} //JRenderServer::CopyTexture

bool JRenderServer::GetTextureProp( int texID, TextureProperties& texProp )
{
    if (texID < 0 || texID >= m_Textures.size()) return false;
    texProp = m_Textures[texID].m_Prop;
    return true;
} // JRenderServer::GetTextureProp

void JRenderServer::ReleaseDefaultRes()
{
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

    m_CurFVF = 0;
} // JRenderServer::ReleaseDefaultRes

void JRenderServer::RecreateDefaultRes()
{
    m_pDevice->ResourceManagerDiscardBytes( 0 );
    m_pDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
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

    CreateQuadIB();
} // JRenderServer::RecreateDefaultRes

void JRenderServer::CreateQuadIB()
{
    //  create quad index buffer
    m_QuadIB = CreateIB( "QuadIndexBuffer", c_QuadIBSize, true );
    WORD* pQuadIdx = (WORD*)LockIB( m_QuadIB, 0, c_QuadIBSize );
    if (!pQuadIdx)
    {
        rlog.err( "JRenderServer: Could not create internal quad index buffer." );
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
} // JRenderServer::CreateQuadIB




