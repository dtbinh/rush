#include "stdafx.h"
#include "JCore.h"
#include "JWindowServer.h"
#include "JDialog.h"
#include "ISoundServer.h"
#include "direct.h"
#include "JDialog.h"
#include "path.h"
#include "fstream.h" 
#include "ifileserver.h"
#include "IPersistServer.h"

void AddModuleMediaPath()
{
    //  set current working directory to the same where we are located
    char path[_MAX_PATH];
    GetModuleFileName( GetModuleHandle( NULL ), path, _MAX_PATH );
    Path mediaPath( path );
    mediaPath.SetFileExt( "" );
    mediaPath.DirAppend( "media" );
    if (mediaPath.Exists())
    {
        g_pFileServer->AddMediaPath( mediaPath.GetFullPath() );
    }
    chdir( path );  
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    import( rb_core         );

    JCore* pCore = new JCore();
    pCore->SetName( "core" );

    if (pCore != JCore::s_pInstance)
    {
        return 1;
    }

    import( rb_ui    );
    import( rb_scene );
    import( rb_logic );

	link_class(Autorun);
    link_class(DrawServerGDI);

    pCore->Init();

    AddModuleMediaPath();
    
    pCore->AddServer( "windowserver"    );
    pCore->AddServer( "drawservergdi"   );
    
    pCore->InitTree();

    JObject* pRoot = g_pPersistServer->LoadObject( "rboot" );
    if (!pRoot)
    {
        return 1;
    }

    JString objPath;
    pRoot->GetPath( objPath );
    pCore->SetRootObject( objPath.c_str() );

    JWindowServer::s_pInstance->AddChild( pRoot, 0 );
    pRoot->InitTree();

    JDialog* pRootDlg = obj_cast<JDialog>( pRoot );
    if (pRootDlg)
    {
        pRootDlg->Show();
    }

	int res = JWindowServer::s_pInstance->RunApplicationLoop();

    pCore->Release();
    return res;
}
