#include "precompile.h"
#include "windows.h"
#include "jcore.h"
#include "jreflect.h"
#include "commandline.h"
#include "ifileserver.h"

void RunTest( const char* lpCmdLine )
{
    import( rb_core );

    JCore* pCore = new JCore();
    pCore->Init();

    CommandLine cmd( lpCmdLine );
    const char* mediaDir = cmd.GetValue( "media" );
    g_pFileServer->AddMediaPath( mediaDir );

    UnitTest::RunAllTests();
}

