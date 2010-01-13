#include "precompile.h"
#include "IRenderServer.h"
#include "IDrawServer.h"
#include "FStream.h"
/*
#include "ColorSurface.h"
#include "IModelServer.h"
#include "IFileServer.h"

static ColorSurface s_Surf;
static ColorSurface s_BPSurf;
static int          s_TestMdlID   = -1;
static int          s_RTID        = -1;
static int          s_HelperTexID = -1;
static int          s_DSID        = -1;

//  set to true to create "testmesh.rbf" and "testmesh.tga" blueprint files
static bool         s_bCreateBlueprint = false;

TEST( ModelIsLoaded )
{
    s_TestMdlID = g_pModelServer->GetModelID( "testmesh" ); 
    CHECK( s_TestMdlID != -1 ); 
}

const int c_TestRTSide = 32;
TEST( OffscreenTargetIsCreated )
{
   s_RTID = g_pRenderServer->CreateRenderTarget( "TestMeshesRT", c_TestRTSide, c_TestRTSide );
   CHECK( s_RTID != -1 ); 

   s_DSID = g_pRenderServer->CreateDepthStencil( "TestMeshesDS", c_TestRTSide, c_TestRTSide );
   CHECK( s_DSID != -1 );
}

TEST( HelperTextureIsCreated )
{
    TextureProperties texProp;
    texProp.m_Format    = ColorFormat_RGB565;
    texProp.m_Width     = c_TestRTSide;
    texProp.m_Height    = c_TestRTSide;
    texProp.m_PoolType  = PoolType_System;
    texProp.m_NMips     = 1;
    strcpy( texProp.m_Name, "temp" );
    s_HelperTexID = g_pRenderServer->CreateTexture( texProp );
    CHECK( s_HelperTexID != -1 );
}

TEST( RenderTargetIsSet )
{
    bool bRes = g_pRenderServer->SetRenderTarget( s_RTID, s_DSID );
    CHECK_EQUAL( bRes, true );

    g_pRenderServer->ClearViewport( 0xFFFFFF00 );
    g_pRenderServer->ClearDepthStencil( true, false );
}

TEST( CanCopyFromRTToSysmemTexture )
{
    //  extract pixels to floating-point surface
    bool bRes = g_pRenderServer->CopyTexture( s_HelperTexID, s_RTID );
    CHECK_EQUAL( bRes, true );
}

TEST( RTTextureIsRead )
{
    bool bRes = s_Surf.ReadFromTexture( s_HelperTexID );
    CHECK_EQUAL( true, bRes );

    CHECK_EQUAL( c_TestRTSide, s_Surf.GetW() );
    CHECK_EQUAL( c_TestRTSide, s_Surf.GetH() );

    ColorF clr = s_Surf( 2, 3 );
    CHECK( clr == ColorF( 1, 1, 1, 0 ) );
}

TEST( ModelIsRendered )
{
    Mat4 viewTM( 1,  0,  0, 0, 
                 0,  0, -1, 0,
                 0, 1,   0, 0,
                 0,  0,  0, 1 );
    Mat4 projTM;
    OrthoProjTM( projTM, 150.0f, -5000.0f, 5000.0f, 1.0f );

    g_pRenderServer->SetViewTM( viewTM );
    g_pRenderServer->SetProjTM( projTM );

    Mat4 tm = Mat4::identity;
    tm.st( 1.5f, Vec3( 30.0f, 0.0f, -50.0f ) );
    g_pModelServer->DrawModel( s_TestMdlID, tm );

    bool bRes = g_pRenderServer->CopyTexture( s_HelperTexID, s_RTID );
    CHECK_EQUAL( bRes, true );

    bRes = s_Surf.ReadFromTexture( s_HelperTexID );
    CHECK_EQUAL( true, bRes );
    
    ColorF clr = s_Surf( 20, 18 );
    CHECK( clr.isEqual( ColorF( 1, 0, 0, 1 ), 0.06f ) );

    if (s_bCreateBlueprint) 
    {
        FOutStream os( "media\\testmesh\\testmesh.rbf" );
        os << s_Surf;
        g_pRenderServer->SaveTexture( s_HelperTexID, "media\\testmesh\\testmesh.tga" );
    }
}

TEST( BluePrintIsPresent )
{
    FInStream is;
    bool bRes = g_pFileServer->OpenMedia( "testmesh", "rbf", is );
    CHECK_EQUAL( bRes, true );
    CHECK( (bool)is );
    is >> s_BPSurf;
    CHECK_EQUAL( c_TestRTSide, s_Surf.GetW() );
    CHECK_EQUAL( c_TestRTSide, s_Surf.GetH() );

    ColorF clr = s_Surf( 20, 18 ); 
    CHECK( clr.isEqual( ColorF( 1, 0, 0, 1 ), 0.06f ) );
}

TEST( RenderedImageIsTheSameAsBlueprint )
{
    CHECK( s_Surf == s_BPSurf ); 
}
*/
