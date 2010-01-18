#include "precompile.h"
#include "commandline.h"
#include "rboot.h"

extern RBoot g_Boot;

#ifdef __GNUC__
int main()
#else
#include "windows.h"
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
#endif
{
  CommandLine cmd( lpCmdLine );
  
  g_Boot.Init( cmd );

	int res = g_Boot.Run();

  g_Boot.Shut();
  return res;
}



