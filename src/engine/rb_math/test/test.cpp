#include "stdafx.h"
#include "windows.h"

#pragma comment(lib, "d3dx9.lib")

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    UnitTest::RunAllTests();
    return 0;
}

