//****************************************************************************/
//  File:  MockRenderServer.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "mockrenderserver.h"

//****************************************************************************/
/*  MockRenderServer
//****************************************************************************/
MockRenderServer::MockRenderServer()
{
    m_Viewport      = Frame( 100.0f, 100.0f, 400.0f, 500.0f );
    m_ViewTM        = IdentityMatrix();
    m_ProjTM        = IdentityMatrix();
    m_WorldTM       = IdentityMatrix();
    m_TextureTM     = IdentityMatrix();
    m_CurFrame      = 0;
}

MockRenderServer::~MockRenderServer()
{
} // MockRenderServer::~MockRenderServer

void MockRenderServer::Init()
{
} // MockRenderServer::Init

void MockRenderServer::ResetTime()
{
} // MockRenderServer::ResetTime

bool MockRenderServer::StartFrame()
{
    return true;
} // MockRenderServer::StartFrame

void MockRenderServer::EndFrame()
{
} // MockRenderServer::EndFrame

void MockRenderServer::ReloadResources()
{
} // MockRenderServer::ReloadResources

void MockRenderServer::ClearViewport( uint32_t color )
{
} // MockRenderServer::ClearViewport

void MockRenderServer::ClearDepthStencil( bool bDepth, bool bStencil )
{
}  // MockRenderServer::ClearDepthBuffer

void MockRenderServer::SetViewport( const Frame& rc )
{
    m_Viewport = rc;
} // MockRenderServer::SetViewport

bool MockRenderServer::Shut()
{
    return true;
} // MockRenderServer::Shut

void MockRenderServer::SetViewTM( const Mat4& tm )
{
    m_ViewTM = tm;
} // MockRenderServer::SetProjTM

void MockRenderServer::SetWorldTM( const Mat4& tm )
{
    m_WorldTM = tm;
} // MockRenderServer::SetProjTM

void MockRenderServer::SetProjTM( const Mat4& tm )
{
    m_ProjTM = tm;
} // MockRenderServer::SetProjTM

void MockRenderServer::SetTextureTM( const Mat4& tm )
{
    m_TextureTM = tm;
} // MockRenderServer::SetTextureTM

int MockRenderServer::GetTextureID( const char* texName )
{
    return 0;
} // MockRenderServer::GetTextureID

void MockRenderServer::SetTexture( int texID, int stage )
{
} // MockRenderServer::SetTexture

void MockRenderServer::DeleteTexture( int texID )
{
} // MockRenderServer::DeleteTexture

void MockRenderServer::ShowCursor( bool bShow )
{
} // MockRenderServer::ShowCursor

int MockRenderServer::CreateDepthStencil( const char* texName, int w, int h, DepthFormat fmt )
{
    return 0;
} // MockRenderServer::CreateDepthStencil

int MockRenderServer::CreateRenderTarget( const char* texName, int w, int h, ColorFormat fmt )
{
    return 0;
} // MockRenderServer::CreateRenderTarget

bool MockRenderServer::SetRenderTarget( int texID, int dsID )
{
    return true;
} // MockRenderServer::SetRenderTarget

bool MockRenderServer::SaveTexture( int texID, const char* fname )
{
    return true;
} // MockRenderServer::SaveTexture

void MockRenderServer::SetAmbient( uint32_t color )
{
} // MockRenderServer::SetAmbient

int MockRenderServer::AddPointLight( const Vec3& pos, float radius, float falloff,
                          uint32_t diffuse, uint32_t specular, bool bPerPixel )
{
    return 0;
} // MockRenderServer::AddPointLight

int MockRenderServer::AddDirLight( const Vec3& dir, uint32_t diffuse, uint32_t specular, bool bPerPixel )
{
    return 0;
} // MockRenderServer::AddDirLight

int MockRenderServer::GetConstantID( int shaderID, const char* name )
{
    return 0;
} // MockRenderServer::GetConstantID

bool MockRenderServer::SetConstant( int shaderID, int constID, const Mat4& val )
{
    return true;
} // MockRenderServer::SetConstant

bool MockRenderServer::SetConstant( int shaderID, int constID, float x, float y, float z, float w )
{
    return true;
} // MockRenderServer::SetConstant

bool MockRenderServer::SetConstant( int shaderID, int constID, float val )
{
    return true;
} // MockRenderServer::SetConstant

int MockRenderServer::GetShaderID( const char* name, bool bReload )
{
    return 0;
} // MockRenderServer::GetShaderID

void MockRenderServer::SetShader( int shID )
{
} // MockRenderServer::SetShader

int MockRenderServer::CreateTexture( const TextureProperties& texProp )
{
    return 0;
} // MockRenderServer::CreateTexture

uint8_t* MockRenderServer::LockTexture( int texID, const Frame& rect, int level, uint32_t* pStride )
{
    return NULL;
} // MockRenderServer::LockTexture

bool MockRenderServer::UnlockTexture( int texID, int level )
{
    return false;
} // MockRenderServer::UnlockTexture

void MockRenderServer::SetColorTint( uint32_t color )
{
} // MockRenderServer::SetColorTint

int MockRenderServer::GetVDeclID( const VertexDeclaration& vdecl )
{
    return 0;
} // MockRenderServer::GetVDeclID

int MockRenderServer::CreateVB( const char* name, int numBytes, bool bStatic )
{
    return 0;
} // MockRenderServer::CreateVB

int MockRenderServer::GetVBID( const char* name )
{
    return -1;
} // MockRenderServer::GetVBID

bool MockRenderServer::UnlockVB( int vbID )
{
    return false;
} // MockRenderServer::UnlockVB

bool MockRenderServer::SetVB( int vbID, int vdeclID, int stream )
{
    return false;
} // MockRenderServer::SetVB

bool MockRenderServer::DeleteVB( int vbID )
{
    return true;
} // MockRenderServer::DeleteVB

int MockRenderServer::CreateIB( const char* name, int numBytes, bool bStatic )
{
    return 0;
} // MockRenderServer::CreateIB

int MockRenderServer::GetIBID( const char* name )
{
    return -1;
} // MockRenderServer::GetIBID

bool MockRenderServer::UnlockIB( int ibID )
{
    return true;
} // MockRenderServer::UnlockIB

bool MockRenderServer::SetIB( int ibID, int baseIndex )
{
    return true;
} // MockRenderServer::SetIB

bool MockRenderServer::DeleteIB( int ibID )
{
    return false;
} // MockRenderServer::DeleteIB

bool MockRenderServer::Draw( int firstIdx, int numIdx, int firstVert, int numVert, PrimitiveType primType )
{
    return true;
} // MockRenderServer::Draw

int MockRenderServer::ClearVB( int vbID )
{
    return -1;
} // MockRenderServer::ClearVB

int MockRenderServer::ClearIB( int ibID )
{
    return -1;
} // MockRenderServer::ClearIB

int MockRenderServer::GetVBIteration( int vbID )
{
    return -1;
} // MockRenderServer::GetVBIteration

int MockRenderServer::GetIBIteration( int ibID )
{
    return -1;
} // MockRenderServer::GetIBIteration

uint8_t* MockRenderServer::LockIB( int ibID, int firstByte, int numBytes )
{
    return NULL;
} // MockRenderServer::LockIB

bool MockRenderServer::CacheIB( int ibID, uint8_t* pData, int size, int& iteration, int& firstByte )
{
    return true;
} // MockRenderServer::CacheIB

uint8_t* MockRenderServer::LockVB( int vbID, int firstByte, int numBytes )
{
    return NULL;
} // MockRenderServer::LockVB

bool MockRenderServer::CacheVB( int vbID, uint8_t* pData, int size, int stride, int& iteration, int& firstByte )
{
    return true;
} // MockRenderServer::CacheVB

bool MockRenderServer::CopyTexture( int destID, int srcID, const Frame* rct, int nRect )
{
    return false;
} //MockRenderServer::CopyTexture

bool MockRenderServer::GetTextureProp( int texID, TextureProperties& texProp )
{
    return true;
} // MockRenderServer::GetTextureProp

void MockRenderServer::Register( IRenderServerClient* pClient )
{
} // MockRenderServer::Register

void MockRenderServer::Unregister( IRenderServerClient* pClient )
{
} // MockRenderServer::Unregister


