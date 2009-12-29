#include "stdafx.h"
#include "shellapi.h"
#include "path.h"
#include "logger.h"
#include "autorun.h"

/***********************************************************************************/
/*  Autorun implementation
/***********************************************************************************/
decl_class(Autorun);

Autorun::Autorun()
{
    m_bInstalled = false;
    m_UninstFile = "uninstall";
    m_ReadmeFile = "readme.txt";
}

void Autorun::Init()
{
    UpdateStatus();
}

void Autorun::UpdateStatus()
{
    char autorunPath[_MAX_PATH];
    GetModuleFileName( NULL, autorunPath, _MAX_PATH );
    m_AutorunPath = autorunPath;

    HKEY hKey = NULL;
    RegOpenKeyEx( HKEY_LOCAL_MACHINE, m_AppKey.c_str(), 0, KEY_READ, &hKey );
    m_bInstalled = (hKey != NULL);

    if (!m_bInstalled)
    {
        return;
    }

    char installDir[_MAX_PATH];
    char appFile[_MAX_PATH];

    DWORD sz = _MAX_PATH;
    DWORD type = REG_SZ;
    
    LONG hRes = RegQueryValueEx( hKey, "InstallDir", 0, &type, (BYTE*)installDir, &sz ); 
    if (hRes != ERROR_SUCCESS)
    {
        rlog.err( "Could not find registry key value <InstallDir>" );
        return;
    }
    
    sz = _MAX_PATH;
    hRes = RegQueryValueEx( hKey, "AppFile", 0, &type, (BYTE*)appFile, &sz ); 
    if (hRes != ERROR_SUCCESS)
    {
        rlog.err( "Could not find registry key value <AppFile>" );
        return;
    }
    

    m_GameExecutable = appFile;
    m_GamePath       = installDir;    
}

bool Autorun::OpenFile( const char* path )
{
    HINSTANCE hInst = ShellExecute( NULL, "open", path, "", NULL, SW_SHOWNORMAL );
    if (hInst == 0)
    {
        rlog.err( "Could not open external file: %s", path );
        return false;
    }
    return true;
}

void Autorun::Render()
{
    UpdateStatus();
    SendSignal( "GameInstalled" );
    SendSignal( "GameNotInstalled" );
}

void Autorun::Install()
{
    Path path( m_AutorunPath.c_str() );
    path.SetFile( m_SetupFile.c_str() );
    path.SetExt( "exe" );
    OpenFile( path.GetFullPath() );
}

void Autorun::Uninstall()
{
    Path path( m_GameExecutable.c_str() );
    path.SetFile( m_UninstFile.c_str() );
    path.SetExt( "exe" );
    OpenFile( path.GetFullPath() );
}

void Autorun::Readme()
{
    Path path( m_AutorunPath.c_str() );
    path.SetFileExt( m_ReadmeFile.c_str() );
    OpenFile( path.GetFullPath() );
}

void Autorun::Exit()
{
    exit( 0 );
}

void Autorun::Play()
{
    if (OpenFile( m_GameExecutable.c_str() ))
    {
        Exit();
    }
}

void Autorun::DirectX()
{

}

void Autorun::Codecs()
{

}