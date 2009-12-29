#include "stdafx.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    UnitTest::RunAllTests();
    return 0;
}

