#include "stdafx.h"
#include "commandline.h"
#include "rboot.h"

extern RBoot g_Boot;

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    CommandLine cmd( lpCmdLine );
    g_Boot.Init( cmd );

	int res = g_Boot.Run();

    g_Boot.Shut();
    return res;
}



