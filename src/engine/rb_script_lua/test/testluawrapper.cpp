#include "precompile.h"
#include "jcore.h"
#include "jsignalserver.h"
#include "fstream.h"
#include "path.h"
#include "iscriptserver.h"
#include "ifileserver.h"

/*
TEST(LuaBindingBase)
{
    int nScripts = g_pScriptServer->GetNScripts();
    CHECK( g_pScriptServer != NULL );
    int scriptID = g_pScriptServer->GetScriptID( "testlua_base" );
    CHECK( scriptID != -1 );
    int nScripts1 = g_pScriptServer->GetNScripts();
    CHECK_EQUAL( nScripts + 1, nScripts1 );
    JString res;
    int nRes = g_pScriptServer->RunScript( scriptID, NULL, &res );
    CHECK( nRes >= 0 );
    CHECK( !strcmp( res.c_str(), "140" ) );

    int scriptID1 = g_pScriptServer->GetScriptID( "testlua_base" );
    CHECK( scriptID1 != -1 );
    int nScripts2 = g_pScriptServer->GetNScripts();
    CHECK_EQUAL( nScripts2, nScripts1 );
}

TEST(LuaCreateDelete)
{
    JObject* pObj = JCore::s_pInstance->FindObject<JWidget>( "testlua_obj", true );
    CHECK( pObj != NULL && pObj->GetNChildren() == 3 );

    int nObj1 = g_pObjectServer->GetNObjects();
    int scriptID = g_pScriptServer->GetScriptID( "testlua_create" );
    CHECK( scriptID != -1 );
    JString res;
    int nRes = g_pScriptServer->RunScript( scriptID, pObj, &res );
    //  object reference extraction in context
    CHECK_EQUAL( 0, stricmp( res.c_str(), "success" ) );

    int nObj2 = g_pObjectServer->GetNObjects();
    CHECK_EQUAL( nObj1 + 2, nObj2 );
    CHECK( nRes >= 0 );
    CHECK( pObj != NULL );
    CHECK( pObj->GetNChildren() == 4 );
    JLabel* pLbl = pObj->FindObject<JLabel>( "lbl" );
    CHECK( pLbl != NULL );
    CHECK_EQUAL( 1, pLbl->GetNRef() );

    JLabel* pLbl1 = pObj->FindObject<JLabel>( "lbl1" );
    CHECK( pLbl1 != NULL );
    CHECK_EQUAL( 1, pLbl1->GetNRef() );
    CHECK_EQUAL( pLbl, pLbl1->GetParent() );
    CHECK_EQUAL( 15.0f, pLbl1->GetLocalExt().x );

    scriptID = g_pScriptServer->GetScriptID( "testlua_delete" );
    CHECK( scriptID != -1 );
    nRes = g_pScriptServer->RunScript( scriptID, pObj, &res );
    CHECK( nRes >= 0 );
    CHECK( pObj->GetNChildren() == 3 );
    pLbl = pObj->FindObject<JLabel>( "lbl" );
    CHECK( pLbl == NULL );
    int nObj3 = g_pObjectServer->GetNObjects();
    CHECK_EQUAL( nObj1, nObj3 );

    pObj->Release();
}

TEST(LuaCall)
{
    JObject* pObj = JCore::s_pInstance->FindObject<JWidget>( "testlua_obj", true );
    CHECK( pObj != NULL && pObj->GetNChildren() == 3 );
    int scriptID = g_pScriptServer->GetScriptID( "testlua_call" );
    CHECK( scriptID != -1 );
    JPicture* pPic = pObj->FindObject<JPicture>( "pic" );
    CHECK( pPic );
    CHECK_EQUAL( true, pPic->IsVisible() );
    JString res;
    int nRes = g_pScriptServer->RunScript( scriptID, pObj, &res );
    CHECK( nRes >= 0 );
    CHECK_EQUAL( false, pPic->IsVisible() );
    nRes = g_pScriptServer->RunScript( scriptID, pObj, &res );
    CHECK( nRes >= 0 );
    CHECK_EQUAL( true, pPic->IsVisible() );
    pObj->Release();
}

TEST(LuaGetSet)
{
    JObject* pObj = JCore::s_pInstance->FindObject<JWidget>( "testlua_obj", true );
    CHECK( pObj != NULL && pObj->GetNChildren() == 3 );
    JButton*  pBtn = pObj->FindObject<JButton>( "btn" );
    JPicture* pPic = pObj->FindObject<JPicture>( "pic" );
    CHECK( pBtn && pPic );
    CHECK( strcmp( pPic->GetName(), "LUAPicture" ) );

    int scriptID = g_pScriptServer->GetScriptID( "testlua_getset" );
    CHECK( scriptID != -1 );
    JString res;
    int nRes = g_pScriptServer->RunScript( scriptID, pObj, &res );
    CHECK( nRes >= 0 );
    CHECK( pObj->GetNChildren() == 3 );
    CHECK( !strcmp( pPic->GetName(), "LUAPicture" ) );
    CHECK( 0 != pBtn->GetExt().x );
    CHECK_EQUAL( pPic->GetExt().x + pPic->GetExt().y, pBtn->GetExt().x );
    CHECK_EQUAL( pBtn->GetExt().y, pPic->GetExt().x );
    pObj->Release();
}

TEST(LuaChildren)
{
    JObject* pObj = JCore::s_pInstance->FindObject<JWidget>( "testlua_obj", true );
    CHECK( pObj != NULL && pObj->GetNChildren() == 3 );
    int scriptID = g_pScriptServer->GetScriptID( "testlua_children" );
    CHECK( scriptID != -1 );
    JString res;
    int nRes = g_pScriptServer->RunScript( scriptID, pObj, &res );
    CHECK( !stricmp( res.c_str(), "4" ) );
    pObj->Release();
}

TEST(LuaParent)
{
    JObject* pObj = JCore::s_pInstance->FindObject<JWidget>( "testlua_parent", true );
    CHECK( pObj != NULL );
    int scriptID = g_pScriptServer->GetScriptID( "testlua_parent" );
    CHECK( scriptID != -1 );
    JString res;
    int nRes = g_pScriptServer->RunScript( scriptID, pObj, &res );
    CHECK( !stricmp( res.c_str(), "testlua_parent" ) );

    //  test setparent
    JButton*  pBtn = pObj->FindObject<JButton>( "btn" );
    JPicture* pPic = pObj->FindObject<JPicture>( "pic" );
    JPicture* pInPic = pObj->FindObject<JPicture>( "inpic" );
    CHECK( pBtn && pPic && pInPic );

    int nBtnRef = pBtn->GetNRef();
    int nPicRef = pPic->GetNRef();
    int nInPicRef = pInPic->GetNRef();

    int scriptID1 = g_pScriptServer->GetScriptID( "testlua_setparent1" );
    int scriptID2 = g_pScriptServer->GetScriptID( "testlua_setparent2" );
    int scriptID3 = g_pScriptServer->GetScriptID( "testlua_setparent3" );
    int scriptID4 = g_pScriptServer->GetScriptID( "testlua_setparent4" );

    CHECK( scriptID1 != -1 && scriptID2 != -1 && scriptID3 != -1 && scriptID4 != -1 );

    nRes = g_pScriptServer->RunScript( scriptID1, pObj, &res );
    CHECK_EQUAL( nBtnRef, pBtn->GetNRef() );
    CHECK_EQUAL( nPicRef, pPic->GetNRef() );
    CHECK_EQUAL( nInPicRef, pInPic->GetNRef() );
    CHECK_EQUAL( pPic, pInPic->GetParent() );
    CHECK_EQUAL( 1, pPic->GetNChildren() );
    CHECK_EQUAL( 0, pBtn->GetNChildren() );

    nRes = g_pScriptServer->RunScript( scriptID2, pObj, &res );
    CHECK_EQUAL( nBtnRef, pBtn->GetNRef() );
    CHECK_EQUAL( nPicRef, pPic->GetNRef() );
    CHECK_EQUAL( nInPicRef, pInPic->GetNRef() );
    CHECK_EQUAL( pBtn, pInPic->GetParent() );
    CHECK_EQUAL( 0, pPic->GetNChildren() );
    CHECK_EQUAL( 1, pBtn->GetNChildren() );

    pInPic->AddRef();
    nRes = g_pScriptServer->RunScript( scriptID3, pObj, &res );
    CHECK_EQUAL( nBtnRef, pBtn->GetNRef() );
    CHECK_EQUAL( nPicRef, pPic->GetNRef() );
    CHECK_EQUAL( nInPicRef, pInPic->GetNRef() );
    CHECK( NULL == pInPic->GetParent() );
    CHECK_EQUAL( 0, pPic->GetNChildren() );
    CHECK_EQUAL( 0, pBtn->GetNChildren() );

    nRes = g_pScriptServer->RunScript( scriptID4, pObj, &res );
    CHECK_EQUAL( nBtnRef, pBtn->GetNRef() );
    CHECK_EQUAL( nPicRef, pPic->GetNRef() );
    CHECK_EQUAL( nInPicRef + 1, pInPic->GetNRef() );
    CHECK_EQUAL( pBtn, pInPic->GetParent() );
    CHECK_EQUAL( 0, pPic->GetNChildren() );
    CHECK_EQUAL( 1, pBtn->GetNChildren() );
    pInPic->Release();

    pObj->Release();
}

TEST(LuaRequire)
{
    int scriptID = g_pScriptServer->GetScriptID( "testlua_require" );
    CHECK( scriptID != -1 );
    JString res;
    int nRes = g_pScriptServer->RunScript( scriptID, NULL, &res );
    CHECK( nRes >= 0 );
    CHECK( !stricmp( res.c_str(), "123" ) );
}

TEST(LuaReloadScripts)
{
    const char* src = g_pFileServer->FindMedia( "testlua_reload_before", "lua" );
    CHECK( src != NULL );
    Path pathBefore( src );
    Path path( pathBefore.GetFullPath() );
    Path pathAfter( path.GetFullPath() );
    path.SetFile( "testlua_reload" );
    pathAfter.SetFile( "testlua_reload_after" );
    DeleteFile( path.GetFullPath() );
    CopyFile( pathBefore.GetFullPath(), path.GetFullPath(), FALSE );

    int scriptID = g_pScriptServer->GetScriptID( "testlua_reload" );
    CHECK( scriptID != -1 );
    JString res;
    int nRes = g_pScriptServer->RunScript( scriptID, NULL, &res );
    CHECK( !stricmp( res.c_str(), "before_reloading" ) );

    CopyFile( pathAfter.GetFullPath(), path.GetFullPath(), FALSE );
    g_pScriptServer->ReloadScripts();
    nRes = g_pScriptServer->RunScript( scriptID, NULL, &res );
    CHECK( !stricmp( res.c_str(), "after_reloading" ) );
}

TEST(LuaRunString)
{
    JString res;
    int nRes = g_pScriptServer->RunString( "return 512", NULL, &res );
    CHECK_EQUAL( 0, nRes );
    CHECK( !strcmp( res.c_str(), "512" ) );
}

TEST(LuaThread)
{
    JObject* pObj = JCore::s_pInstance->FindObject<JWidget>( "testlua_obj", true );
    JButton* pBtn = pObj->FindObject<JButton>( "btn" );
    JPicture* pPic = pObj->FindObject<JPicture>( "inpic" );
    CHECK( pBtn && pObj );

    int script0 = g_pScriptServer->GetScriptID( "testlua_thread0" );
    int script1 = g_pScriptServer->GetScriptID( "testlua_thread1" );
    int script2 = g_pScriptServer->GetScriptID( "testlua_thread2" );
    int script3 = g_pScriptServer->GetScriptID( "testlua_thread3" );
    int script4 = g_pScriptServer->GetScriptID( "testlua_thread4" );
    int script5 = g_pScriptServer->GetScriptID( "testlua_thread5" );
    CHECK( script0 >= 0 && script1 >= 0 && script2 >= 0 &&
           script3 >= 0 && script4 >= 0 && script5 >= 0 );

    int thread0  = g_pScriptServer->RunScript( script0, pObj );
    CHECK( thread0 > 0 );
    CHECK_EQUAL( 1, g_pScriptServer->GetNThreads() );
    for (int i = 0; i < 10; i++)
    {
        g_pScriptServer->UpdateThread( thread0, 0.1f );
    }
    CHECK_EQUAL( 10.0f, pBtn->GetExt().x );

    int thread1  = g_pScriptServer->RunScript( script1, pObj );
    int thread2  = g_pScriptServer->RunScript( script2, pObj );
    int thread3  = g_pScriptServer->RunScript( script3, pObj );
    int thread4  = g_pScriptServer->RunScript( script4, pObj );
    int thread5  = g_pScriptServer->RunScript( script5, pObj );

    CHECK( thread0 > 0 && thread1 > 0 && thread2 > 0 &&
           thread3 > 0 && thread4 > 0 && thread5 > 0 );

    CHECK_EQUAL( 6, g_pScriptServer->GetNThreads() );
    CHECK( stricmp( pBtn->GetName(), "thread4" ) );

    for (int i = 0; i < 40; i++)
    {
        g_pScriptServer->UpdateThread( thread5, 0.1f );
    }
    CHECK_EQUAL( 6, g_pScriptServer->GetNThreads() );
    CHECK( stricmp( pPic->GetName(), "thread5" ) );

    for (int i = 0; i < 50; i++)
    {
        g_pScriptServer->UpdateThread( thread2, 0.1f );
    }
    CHECK_EQUAL( 25.0f, pPic->GetExt().x );
    CHECK_EQUAL( 6, g_pScriptServer->GetNThreads() );


    for (int i = 0; i < 40; i++)
    {
        g_pScriptServer->UpdateThread( thread0, 0.4f );
        g_pScriptServer->UpdateThread( thread1, 0.1f );
        g_pScriptServer->UpdateThread( thread2, 0.1f );
        g_pScriptServer->UpdateThread( thread3, 0.1f );
        g_pScriptServer->UpdateThread( thread4, 0.1f );
        g_pScriptServer->UpdateThread( thread5, 0.1f );
    }
    CHECK_EQUAL( 170.0f, pBtn->GetExt().x );
    CHECK_EQUAL( 10.0f, pBtn->GetExt().y );
    CHECK_EQUAL( 45.0f, pPic->GetExt().x );
    CHECK_EQUAL( 4.0f, pPic->GetExt().y );

    CHECK_EQUAL( 4, g_pScriptServer->GetNThreads() );
    CHECK( !stricmp( pBtn->GetName(), "thread4" ) );
    CHECK( !stricmp( pPic->GetName(), "thread5" ) );

    for (int i = 0; i < 40; i++)
    {
        g_pScriptServer->UpdateThread( thread0, 0.2f );
        g_pScriptServer->UpdateThread( thread1, 0.1f );
        g_pScriptServer->UpdateThread( thread2, 0.1f );
        g_pScriptServer->UpdateThread( thread3, 0.1f );
        g_pScriptServer->UpdateThread( thread4, 0.1f );
        g_pScriptServer->UpdateThread( thread5, 0.1f );
    }
    CHECK_EQUAL( 4, g_pScriptServer->GetNThreads() );
    CHECK( !stricmp( pBtn->GetName(), "thread4" ) );
    CHECK( !stricmp( pPic->GetName(), "thread5" ) );
    CHECK_EQUAL( 250.0f, pBtn->GetExt().x );
    CHECK_EQUAL( 20.0f, pBtn->GetExt().y );
    CHECK_EQUAL( 65.0f, pPic->GetExt().x );
    CHECK_EQUAL( 8.0f,  pPic->GetExt().y );

    g_pScriptServer->StopThread( thread0 );
    CHECK_EQUAL( 3, g_pScriptServer->GetNThreads() );

    g_pScriptServer->StopThread( thread1 );
    CHECK_EQUAL( 2, g_pScriptServer->GetNThreads() );

    g_pScriptServer->StopThread( thread2 );
    CHECK_EQUAL( 1, g_pScriptServer->GetNThreads() );

    g_pScriptServer->StopThread( thread3 );
    CHECK_EQUAL( 0, g_pScriptServer->GetNThreads() );

    pObj->Release();


}

TEST( Connect )
{
    JWidget*  pObj  = JCore::s_pInstance->FindObject<JWidget>( "testlua_connect", true );

    JButton*  pBtn1 = pObj->FindObject<JButton>( "btn1" );
    JButton*  pBtn2 = pObj->FindObject<JButton>( "btn2" );

    JPicture* pPic  = pObj->FindObject<JPicture>( "pic" );
    CHECK( pObj && pBtn1 && pBtn2 && pPic );

    int scriptID = g_pScriptServer->GetScriptID( "testlua_connect" );
    CHECK( scriptID != -1 );
    JString res;
    int nSig = JSignalServer::s_pInstance->GetNumSignals();
    int nThread = g_pScriptServer->RunScript( scriptID, pObj, &res );
    CHECK( nThread > 0 );

    int nSig1 = JSignalServer::s_pInstance->GetNumSignals();
    CHECK_EQUAL( nSig + 3, nSig1 );

    CHECK( pObj->GetExt().x == 0.0f && pObj->GetExt().y == 0.0f && pObj->GetExt().w == 0.0f );
    pBtn1->SendSignal( "press" );
    pBtn2->SendSignal( "press" );
    CHECK( pObj->GetExt().x == 0.0f && pObj->GetExt().y == 2.0f && pObj->GetExt().w == 0.0f );
    pBtn1->SendSignal( "unpress" );
    CHECK( pObj->GetExt().x == 0.0f && pObj->GetExt().y == 2.0f && pObj->GetExt().w == 0.0f );
    pBtn2->SendSignal( "unpress" );

    CHECK( pObj->GetExt().x == 3.0f && pObj->GetExt().y == 2.0f );
    pObj->SendSignal( "drag" );
    CHECK( pObj->GetExt().x == 3.0f && pObj->GetExt().y == 2.0f );
    pObj->SendSignal( "onhover" );
    CHECK( pObj->GetExt().x == 1.0f && pObj->GetExt().y == 2.0f );

    g_pScriptServer->UpdateThread( nThread, 10.0f );
    nSig1 = JSignalServer::s_pInstance->GetNumSignals();
    CHECK_EQUAL( nSig + 3, nSig1 );

    g_pScriptServer->UpdateThread( nThread, 100.0f );
    nSig1 = JSignalServer::s_pInstance->GetNumSignals();
    CHECK_EQUAL( nSig, nSig1 );

    pObj->Release();
}

TEST( Fork )
{
    JWidget* pObj = JCore::s_pInstance->FindObject<JWidget>( "testlua_fork", true );
    CHECK( pObj != NULL );

    JButton*  pBtn1 = pObj->FindObject<JButton>( "btn1" );
    JButton*  pBtn2 = pObj->FindObject<JButton>( "btn2" );

    JPicture* pPic  = pObj->FindObject<JPicture>( "pic" );
    CHECK( pObj && pBtn1 && pBtn2 && pPic );

    int scriptID = g_pScriptServer->GetScriptID( "testlua_fork" );
    CHECK( scriptID != -1 );

    int nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 0, nThreads );

    JString res;

    //  start root thread
    int nSig = JSignalServer::s_pInstance->GetNumSignals();
    int nThread = g_pScriptServer->RunScript( scriptID, pObj, &res );
    CHECK( nThread > 0 );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 1, nThreads );

    //  run root thread
    g_pScriptServer->UpdateThread( nThread, 1.0f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 1, nThreads );

    //  spawn "connect" subthread
    pPic->SendSignal( "onlbclick" );
    g_pScriptServer->UpdateThread( nThread, 0.5f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 2, nThreads );
    int nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 1, nCh );

    //  try to spawn wrong "dispatch" subthread
    g_pScriptServer->RunFunction( nThread, "somewrongname" );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 2, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 1, nCh );

    //  finishing "connect" subthread
    g_pScriptServer->UpdateThread( nThread, 0.6f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 1, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 0, nCh );

    //  try to spawn wrong "dispatch" subthread
    g_pScriptServer->RunFunction( nThread, "somewrongname1" );
    g_pScriptServer->UpdateThread( nThread, 0.4f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 1, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 0, nCh );


    // spawn correct "dispatch" subthread
    g_pScriptServer->RunFunction( nThread, "DispatchFunction" );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 2, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 1, nCh );

    //  "fork" subthread is being spawned from the script
    g_pScriptServer->UpdateThread( nThread, 0.5f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 3, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 2, nCh );

    //  3 sec
    //  "dispatch" is finishing
    g_pScriptServer->UpdateThread( nThread, 0.5f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 2, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 1, nCh );

    //  "fork" is finishing
    g_pScriptServer->UpdateThread( nThread, 1.0f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 1, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 0, nCh );

    //  4.5 sec
    //  spawn "connect" subthread
    pPic->SendSignal( "onlbclick" );
    g_pScriptServer->UpdateThread( nThread, 0.5f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 2, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 1, nCh );

    //  5 sec
    //  "connect" subthread finishes
    g_pScriptServer->UpdateThread( nThread, 0.9f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 1, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 0, nCh );

    //  6 sec
    //  script spawns another fork
    g_pScriptServer->UpdateThread( nThread, 0.6f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 2, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 1, nCh );

    //  6.5 sec
    //  fork spawns another fork
    g_pScriptServer->UpdateThread( nThread, 1.0f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 3, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 1, nCh );

    //  7.5 sec
    //  forks finish
    g_pScriptServer->UpdateThread( nThread, 1.5f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 1, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 0, nCh );

    //  9 sec
    // root thread finishes
    g_pScriptServer->UpdateThread( nThread, 1.5f );
    nThreads = g_pScriptServer->GetNThreads();
    CHECK_EQUAL( 0, nThreads );
    nCh = g_pScriptServer->GetNThreads( nThread );
    CHECK_EQUAL( 0, nCh );
}
*/
