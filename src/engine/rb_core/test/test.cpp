#include "precompile.h"
#include "windows.h"
#include "JCore.h"
#include "JReflect.h"
#include "CommandLine.h"
#include "IFileServer.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    import( rb_core );

    JCore* pCore = new JCore();
    pCore->Init();

    CommandLine cmd( lpCmdLine );
    const char* mediaDir = cmd.GetValue( "media" );
    g_pFileServer->AddMediaPath( mediaDir );

    UnitTest::RunAllTests();
    return 0;
}

