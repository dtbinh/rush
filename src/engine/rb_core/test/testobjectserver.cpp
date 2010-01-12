#include "precompile.h"
#include "jcore.h"
#include "jobjectserver.h"

TEST( CreateFromScript )
{

}

/*
TEST( ObjectServer )
{   
    int numObj1 = g_pObjectServer->GetNObjects();
    int numDel1 = g_pObjectServer->GetNDelObjects();
    JLabel* pStandalone = new JLabel();
    pStandalone->SetName( "pic" );
    JLabel* pSLab = new JLabel();
    pSLab->SetName( "lab1" );
    pStandalone->AddChild( pSLab );
    
    pSLab = new JLabel();
    pSLab->SetName( "lab1" );
    pStandalone->AddChild( pSLab );

    pSLab = new JLabel();
    pSLab->SetName( "pic" );
    pStandalone->AddChild( pSLab );

    int numObj2 = g_pObjectServer->GetNObjects();
    int numDel2 = g_pObjectServer->GetNDelObjects();

    CHECK_EQUAL( numDel1, numDel2 );
    CHECK_EQUAL( numObj1 + 4, numObj2 );

    pStandalone->Release();

    int numObj3 = g_pObjectServer->GetNObjects();
    int numDel3 = g_pObjectServer->GetNDelObjects();

    CHECK_EQUAL( numDel1 + 4, numDel3 );
    CHECK_EQUAL( numObj1, numObj3 );



    JObject* pObj = g_pPersistServer->LoadObject( "test_object_server", NULL );
    CHECK( pObj != NULL );
    pObj->InitTree();

    JLabel* pSLabel = pObj->FindObject<JLabel>( "label_with_very_strange_name" );
    CHECK( pSLabel != NULL ); 
    
    pObj->Release();
    pSLabel = pObj->FindObject<JLabel>( "label_with_very_strange_name" );
    CHECK( pSLabel == NULL ); 

    pObj = g_pPersistServer->LoadObject( "test_object_server", NULL );
    CHECK( pObj != NULL );

    JObject* pObj1 = g_pPersistServer->LoadObject( "test_object_server", pObj );
    CHECK_EQUAL( pObj, pObj1 );

    CHECK_EQUAL( true, g_pObjectServer->AreObjectsEqual( pObj, pObj, true ) );
    CHECK_EQUAL( true, g_pObjectServer->AreObjectsEqual( pObj, pObj, false ) );

    JObject* pNewObj = g_pObjectServer->CloneObject( pObj, JCore::s_pInstance );
    CHECK_EQUAL( true, g_pObjectServer->AreObjectsEqual( pObj, pNewObj, false ) );

    JListBox* pListBox = pNewObj->FindObject<JListBox>( "droplist" );
    CHECK( pListBox != NULL );
    pListBox->SetExt( Frame( 1.0f, 2.0f, 3.0f, 4.0f ) );
    CHECK_EQUAL( false, g_pObjectServer->AreObjectsEqual( pObj, pNewObj, true ) );
    CHECK_EQUAL( true, g_pObjectServer->AreObjectsEqual( pObj, pNewObj, false ) );

    int nObj1 = g_pObjectServer->GetNObjects();
    JLabel* pLabel = new JLabel();
    int nObj2 = g_pObjectServer->GetNObjects();
    CHECK_EQUAL( nObj1, nObj2 );
    pLabel->SetName( "label" );
    nObj2 = g_pObjectServer->GetNObjects();
    CHECK_EQUAL( nObj1 + 1, nObj2 );
    pLabel->SetName( "" );
    nObj2 = g_pObjectServer->GetNObjects();
    CHECK_EQUAL( nObj1 + 1, nObj2 );
    pLabel->Release();
    nObj2 = g_pObjectServer->GetNObjects();
    CHECK_EQUAL( nObj1, nObj2 );
    
    pObj->Release();
}
*/