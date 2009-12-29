#include "stdafx.h"
#include "JCore.h"
#include "JReflect.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    import( rb_core );
    import( rb_ui );
    import( rb_extui );

    JCore* pCore = new JCore();
    pCore->Init();

    UnitTest::RunAllTests();
    return 0;
}

