#include "stdafx.h"
#include "JReflect.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    import( rb_core );

    UnitTest::RunAllTests();
    return 0;
}

