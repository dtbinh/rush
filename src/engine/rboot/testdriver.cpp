/***********************************************************************************/
//  File:   TestDriver.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "logger.h"
#include "fstream.h"
#include "path.h"
#include "ipersistserver.h"
#include "ifileserver.h"
#include "imodelserver.h"
#include "testdriver.h"  

/***********************************************************************************/
/*  TestDriver implementation
/***********************************************************************************/
decl_class(TestDriver);
TestDriver::TestDriver()
{
    SetName( "testdriver" );
}

TestDriver::~TestDriver()
{
}
    
void TestDriver::Render()
{
    SendSignal( "Render" );
} 

void TestDriver::PostRender()
{
    SendSignal( "PostRender" );
    g_pModelServer->DrawShadows();
}  

void TestDriver::SaveSettings()
{
    FOutStream os( m_ConfigPath.c_str() );
    JString iniStr;
    cast( iniStr, m_ActiveTests );
    os << iniStr;
}

void TestDriver::LoadSettings()
{
    JString path;
    if (!g_pFileServer->FindMedia( "testdriver", "ini", &path ))
    {
        return;
    }
    m_ConfigPath = path;
    FInStream is;
    if (!g_pFileServer->OpenMedia( "testdriver", "ini", is ))
    {
        return;
    }
    JString iniStr;
    is >> iniStr;
    cast( m_ActiveTests, iniStr );
}

void TestDriver::GatherTests()
{
    int nRoots = g_pFileServer->GetNMediaPaths();
    if (nRoots == 0)
    {
        return;
    }
    
    JString path;
    if (!g_pFileServer->FindMedia( "tests_dir_root", "", &path ))
    {
        return;
    }

    Path root( path.c_str() );
    root.SetFileExt( "" );
    std::vector<Path> files;
    root.GetDirFiles( files, "rjml" );
    int nFiles = files.size();
    for (int i = 0; i < nFiles; i++)
    {
        m_TestNames.push_back( files[i].GetFile() );
    }
}  

void TestDriver::Init()
{
    GatherTests();
    LoadSettings();
}  

void TestDriver::Exit()
{
    exit( 0 );
}  



