#include "stdafx.h"
#include "ColorSurface.h"
#include "MemStream.h"
#include "IRenderServer.h"

static ColorSurface surf, surf1;

TEST( ColorSurfaceIsEmptyAfterCreation )
{
    CHECK_EQUAL( 0, surf.GetW() );
    CHECK_EQUAL( 0, surf.GetH() ); 
}

TEST( ColorSurfaceCheckResize )
{
    surf.SetExt( 10, 5 );
    CHECK_EQUAL( 50, surf.GetNPixels() ); 
}

TEST( FillColorSurface )
{
    surf.Clear( ColorF::blue ); 
    ColorF clr = surf( 7, 3 );
    CHECK_EQUAL( 0.0f, clr.r );
    CHECK_EQUAL( 0.0f, clr.g );
    CHECK_EQUAL( 1.0f, clr.b );
    CHECK_EQUAL( 1.0f, clr.a );
}

TEST( CopyColorSurface )
{
    surf1 = surf;
    CHECK_EQUAL( 10, surf1.GetW() );
    CHECK_EQUAL( 5, surf1.GetH() ); 

    ColorF clr = surf1( 7, 3 );
    CHECK_EQUAL( 0.0f, clr.r );
    CHECK_EQUAL( 0.0f, clr.g );
    CHECK_EQUAL( 1.0f, clr.b );
    CHECK_EQUAL( 1.0f, clr.a );
}

/*
TEST( ReadFromDeviceTexture )
{
    int texID = g_pRenderServer->GetTextureID( "checkers.tga" );
    CHECK( texID != -1 );
    bool bRes = surf.ReadFromTexture( texID );
    CHECK_EQUAL( true, bRes );

    CHECK_EQUAL( 32, surf.GetW() );
    CHECK_EQUAL( 16, surf.GetH() );

    ColorF clr = surf( 14, 5 );
    CHECK( clr == ColorF( 0, 1, 1, 0 ) );

    clr = surf( 30, 3 );
    CHECK( clr == ColorF( 1, 0, 1, 0 ) );

    clr = surf( 0, 15 );
    CHECK( clr == ColorF( 1, 0, 0, 1 ) );
}


TEST( ColorSurfaceSerialization )
{
    MemOutStream os( 8192*16 );
    os << surf;
    os.Close();

    MemInStream is( os.GetBuffer() );
    ColorSurface nsurf;
    is >> nsurf;
    CHECK( surf == nsurf );
}

TEST( ColorSurfaceDownSampling )
{
    surf.Downsample();
    CHECK_EQUAL( 16, surf.GetW() );
    CHECK_EQUAL( 8,  surf.GetH() );

    ColorF clr = surf( 7, 2 );
    CHECK( clr == ColorF( 0, 1, 1, 0 ) );

    clr = surf( 15, 1 );
    CHECK( clr == ColorF( 1, 0, 1, 0 ) );

    clr = surf( 0, 7 );
    CHECK( clr == ColorF( 1, 0, 0, 1 ) );
}

TEST( ColorSurfaceToGrayScale )
{

}

TEST( ColorSurfaceCopyRect )
{

}

*/