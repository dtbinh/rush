#include "precompile.h"
#include "commandline.h"
#include "rboot.h"

extern RBoot g_Boot;

#ifdef __GNUC__
int main( int argc, char** argv )
{
    std::string cmdLine;
    for (int i = 0; i < argc; i++)
    {
        cmdLine += " ";
        cmdLine += argv[i];
    }
    const char* lpCmdLine = cmdLine.c_str();
#else
#include "windows.h"
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
#endif
  CommandLine cmd( lpCmdLine );

  g_Boot.Init( cmd );

    int res = g_Boot.Run();

  g_Boot.Shut();
  return res;
}



