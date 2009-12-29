#include "stdafx.h"
#include "Path.h"

TEST( Path )
{
    Path path; 
    CHECK_EQUAL( strlen( path.GetFullPath() ), 0 );
    
    path.SetDrive( "c" );
    CHECK( !strcmp( path.GetDrive(), "c" ) );
    CHECK( !strncmp( path.GetFullPath(), "c:", 2 ) );

    path.SetDirectory( "My Documents/My Pictures\\photos\\" );
    CHECK_EQUAL( strlen( path.GetFile() ), 0 );
    CHECK( !strcmp( path.GetDirectory(), "\\My Documents\\My Pictures\\photos" ) );
    CHECK( !strcmp( path.GetFullPath(), "c:\\My Documents\\My Pictures\\photos\\" ) );
    
    path.SetFileExt( "IMG12323.jpg" );
    CHECK( !strcmp( path.GetFile(), "IMG12323" ) );
    CHECK( !strcmp( path.GetExt(), "jpg" ) );
    CHECK( !strcmp( path.GetFileExt(), "IMG12323.jpg" ) );
    CHECK( !strcmp( path.GetFullPath(), "c:\\My Documents\\My Pictures\\photos\\IMG12323.jpg" ) );

    path.SetExt( "bmp" );
    CHECK( !strcmp( path.GetExt(), "bmp" ) );
    CHECK( !strcmp( path.GetFullPath(), "c:\\My Documents\\My Pictures\\photos\\IMG12323.bmp" ) );

    path.SetDirectory( "Temporary Internet Files/some_directory/images\\pictures\\photos\\\\" );
    CHECK( !strcmp( path.GetDirectory(), "\\Temporary Internet Files\\some_directory\\images\\pictures\\photos" ) );
    CHECK( !strcmp( path, "c:\\Temporary Internet Files\\some_directory\\images\\pictures\\photos\\IMG12323.bmp" ) );
    CHECK_EQUAL( 5, path.GetDirDepth() );

    path.Clear();
    CHECK_EQUAL( strlen( path.GetFullPath() ),  0 );
    CHECK_EQUAL( strlen( path.GetDrive() ),     0 );
    CHECK_EQUAL( strlen( path.GetDirectory() ), 0 );
    CHECK_EQUAL( strlen( path.GetFile() ),      0 );
    CHECK_EQUAL( strlen( path.GetExt() ),       0 );
    CHECK_EQUAL( strlen( path.GetFileExt() ),   0 );

    path = "e:\\books\\articles\\sh lighting\\work\\SphericalCubemap\\Light Probes\\cubemap.dds";
    CHECK( !strcmp( path.GetDrive(), "e" ) );
    CHECK( !strcmp( path.GetDirectory(), "\\books\\articles\\sh lighting\\work\\SphericalCubemap\\Light Probes" ) );
    CHECK( !strcmp( path.GetFile(), "cubemap" ) );
    CHECK( !strcmp( path.GetExt(), "dds" ) );
    CHECK( !strcmp( path.GetFileExt(), "cubemap.dds" ) );

    path.SetFileExt( NULL );
    CHECK_EQUAL( 0, strlen( path.GetFile() ) );
    CHECK_EQUAL( 0, strlen( path.GetExt() ) );

    path.SetFileExt( "texture.jpg" );
    CHECK( !strcmp( path.GetFile(), "texture" ) );
    CHECK( !strcmp( path.GetExt(), "jpg" ) );

    path.SetDrive( NULL );
    CHECK_EQUAL( 0, strlen( path.GetDrive() ) );
    path.SetDirectory( NULL );
    CHECK_EQUAL( 0, strlen( path.GetDirectory() ) );
    path.SetFile( NULL );
    CHECK_EQUAL( 0, strlen( path.GetFile() ) );
    path.SetExt( NULL );
    CHECK_EQUAL( 0, strlen( path.GetExt() ) );
    CHECK_EQUAL( 0, strlen( path.GetFileExt() ) );
}

TEST( PathDirOperations )
{
    Path path( "d:\\home\\rush\\bin\\rboot.exe" );

    bool bRes = path.DirUp();
    CHECK_EQUAL( true, bRes );
    CHECK( !stricmp( path.GetFullPath(), "d:\\home\\rush\\rboot.exe" ) );

    path.DirAppend( "projects" );
    CHECK( !stricmp( path.GetFullPath(), "d:\\home\\rush\\projects\\rboot.exe" ) );

    bRes = path.DirUp();
    CHECK_EQUAL( true, bRes );
    CHECK( !stricmp( path.GetFullPath(), "d:\\home\\rush\\rboot.exe" ) );

    path.DirAppend( "projects\\mermaid2" );
    CHECK( !stricmp( path.GetFullPath(), "d:\\home\\rush\\projects\\mermaid2\\rboot.exe" ) );

    path.SetFileExt( "" );

    bRes = path.DirUp();
    CHECK_EQUAL( true, bRes );
    CHECK( !stricmp( path.GetFullPath(), "d:\\home\\rush\\projects\\" ) );

    path.DirAppend( "\\mermaid2" );
    CHECK( !stricmp( path.GetFullPath(), "d:\\home\\rush\\projects\\mermaid2\\" ) );

    path.DirAppend( "/media\\sprites" );
    CHECK( !stricmp( path.GetFullPath(), "d:\\home\\rush\\projects\\mermaid2\\media\\sprites\\" ) );
}

TEST( PathNormalize )
{
    Path path3( "d:\\home\\rush\\projects\\\\..\\..\\\\mermaid2\\media\\sprites\\" );
    path3.Normalize();
    CHECK( !stricmp( path3.GetFullPath(), "d:\\home\\mermaid2\\media\\sprites\\" ) );

    Path path1( "d:\\\\home\\rush\\\\..\\\\bin\\..\\rboot.exe" );
    path1.Normalize();
    CHECK( !stricmp( path1.GetFullPath(), "d:\\home\\rboot.exe" ) );

    Path path2( "d:\\home\\rush\\.\\bin\\..\\rboot.exe" );
    path2.Normalize();
    CHECK( !stricmp( path2.GetFullPath(), "d:\\home\\rush\\rboot.exe" ) );

}