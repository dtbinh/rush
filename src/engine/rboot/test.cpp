#include "precompile.h"

#include "UnitTest++.h"
#include "jcore.h"

//int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
int main()
{
    import( rb_core         );

    JCore* m_pCore = new JCore();
    m_pCore->SetName( "core" );

    if (m_pCore != JCore::s_pInstance)
    {
        return false;
    }

    import( rb_script_lua   );

    //import( rb_rendergl     );
    //import( rb_ui           );
    
    UnitTest::RunAllTests();
    return 0;
}

