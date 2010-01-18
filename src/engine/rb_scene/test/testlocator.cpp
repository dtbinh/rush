#include "precompile.h"
#include "IRenderServer.h"
#include "MockRenderServer.h"
#include "JLocator.h"

static MockRenderServer s_MockServer;

const float c_TestEps = 0.8;
TEST( Locator )
{
    g_pRenderServer = &s_MockServer;
    g_pRenderServer->SetViewport( Frame( 10.0f, 20.0f, 800.0f, 900.0f ) );


    JLocator* pLocator = new JLocator();
    
    pLocator->SetExt( Frame( 10.0f, 20.0f, 300.0f, 300.0f ) );
    
    Vec3 srcPos( 160.0f, 170.0f, 0.0f );
    pLocator->SetScreenSpace( true );
    pLocator->SetPos( srcPos );
    Vec3 pos = pLocator->GetPos();
    CHECK( pos.is_equal( srcPos, c_TestEps ) );
    
    pLocator->SetScreenSpace( false );
    pos = pLocator->GetPos();
    CHECK( !pos.is_equal( srcPos, c_TestEps ) );
    
    pLocator->SetScreenSpace( true );
    pos = pLocator->GetPos();
    CHECK( pos.is_equal( srcPos, c_TestEps ) );

    g_pRenderServer->SetViewTM( 
        Mat4( -0.013592230, 0.99156004, 0.12893367, 0.00000000,	
	            0.99990267, 0.013881840, -0.0013477511, 0.00000000,
	          -0.0031262129, 0.12890281, -0.99165231, 0.00000000,
	           -2.8433974, -6.4867854, 49.586658, 0.99999994 ) );
    g_pRenderServer->SetProjTM( 
        Mat4( 2.4142134, 0.00000000, 0.00000000, 0.00000000,
              0.0000000, 3.2189512,  0.00000000, 0.00000000,
              0.0000000, 0.0000000,  1.0000100,  1.000000,
	          0.0000000, 0.00000000, -0.010000099, 0.0000000 ) );


    Vec3 srcPos1( 150.0f, 20.0f, 0.2f );
    pLocator->SetScreenSpace( true );
    pLocator->SetPos( srcPos1 );
    pos = pLocator->GetPos();
    CHECK( pos.is_equal( srcPos1, c_TestEps ) );
    
    pLocator->SetScreenSpace( false );
    pos = pLocator->GetPos();
    CHECK( !pos.is_equal( srcPos1, c_TestEps ) );
    
    pLocator->SetScreenSpace( true );
    pos = pLocator->GetPos();
    CHECK( pos.is_equal( srcPos1, c_TestEps ) );
}
