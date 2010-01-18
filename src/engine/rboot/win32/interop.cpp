#include "stdafx.h"
#include "rboot.h"
#include "IRenderServer.h"
#include "IWindowServer.h"
#include "commandline.h"

extern RBoot g_Boot;

extern "C" __declspec(dllexport) VOID WINAPI RushInit( const char* cmdLine )
{
    CommandLine cmd( cmdLine );
    g_Boot.Init( cmd );
    HWND hWnd = (HWND)g_pWindowServer->GetRootHandle();
    ShowWindow( hWnd, SW_HIDE );
}

extern "C" __declspec(dllexport) VOID WINAPI RushUpdate()
{
    g_Boot.Update();
}

extern "C" __declspec(dllexport) VOID WINAPI RushResize( int w, int h )
{
    g_pRenderServer->ResizeBackBuffer( w, h );
}

extern "C" __declspec(dllexport) VOID WINAPI RushShut()
{
    g_Boot.Shut();
}

extern "C" __declspec(dllexport) LPVOID WINAPI RushGetBackBuffer()
{
    if (!g_pRenderServer)
    {
        return NULL;
    }
    return g_pRenderServer->GetRenderSurface();
}
