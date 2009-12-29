#include "stdafx.h"
#include "JCore.h"
#include "CommandLine.h"
#include "IFileServer.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    JCore* pCore = new JCore(); 
    if (pCore != JCore::s_pInstance)
    {
        return 1;
    }
    
    CommandLine cmd( lpCmdLine );

    import( rb_ui           );
    import( rb_render9      );
    import( rb_core         );
    import( rb_logic        );
    import( rb_scene        ); 
    import( rb_particle     );
    import( rb_physics      );
    import( rb_script_lua   );
    import( rb_extui        );
    import( rb_quest        );
    import( rb_draw         );
    import( rb_nature       );

    pCore->Init();

    const char* mediaDir = cmd.GetValue( "media" );
    g_pFileServer->AddMediaPath( mediaDir );

    pCore->AddServer( "luaserver" );
    pCore->AddServer( "windowserver" );
    pCore->AddServer( "renderserver" );
    pCore->AddServer( "drawserver" );
    pCore->AddServer( "modelserver" );
    
    pCore->InitTree();

    UnitTest::RunAllTests();
    pCore->Release();
    return 0;
}

