#include "precompile.h"
#include "IPersistServer.h"
#include "JObject.h"

TEST( FindObject )
{
    JObject* pRoot1 = g_pPersistServer->LoadObject( "test_findobject1" );
    JObject* pRoot2 = g_pPersistServer->LoadObject( "test_findobject2" );
    CHECK( pRoot1 != NULL );
    CHECK( pRoot2 != NULL );

    JObject* pDrop = g_pObjectServer->FindObject( "droplist" );
    CHECK( pDrop != NULL );
    CHECK( !stricmp( pDrop->ClassName(), "group" ) );

    JObject* pDrop1 = g_pObjectServer->FindObject( "droplist", pRoot1 );
    JObject* pDrop2 = g_pObjectServer->FindObject( "droplist", pRoot2 ); 
    CHECK( pDrop1 != NULL );
    CHECK( pDrop2 != NULL );
    CHECK( pDrop1 != pDrop2 );
    CHECK_EQUAL(  4, pDrop1->GetTag() );
    CHECK_EQUAL( 14, pDrop2->GetTag() );

    JObject* pThumb1 = g_pObjectServer->FindObject( "thumb", pDrop1 );
    JObject* pThumb2 = g_pObjectServer->FindObject( "thumb", pDrop2 );
    CHECK( pThumb1 != NULL );
    CHECK( pThumb2 != NULL );
    CHECK_EQUAL(  31, pThumb1->GetTag() );
    CHECK_EQUAL( -31, pThumb2->GetTag() );

    //  compound paths
    JObject* pThumb = NULL; 
    pThumb = g_pObjectServer->FindObject( "droplist.pic.thumb", pRoot2 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL(  303, pThumb->GetTag() );

    pThumb = g_pObjectServer->FindObject( "object_tree.thumb", pRoot2 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL(  188, pThumb->GetTag() );  

    pThumb = g_pObjectServer->FindObject( "anotherpic.thumb", pRoot2 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL(  108, pThumb->GetTag() );

    pThumb = g_pObjectServer->FindObject( "pic.thumb", pRoot2 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL(  303, pThumb->GetTag() );

    pThumb = g_pObjectServer->FindObject( "droplist.thumb", pRoot2 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL(  -31, pThumb->GetTag() );

    pThumb = g_pObjectServer->FindObject( "object_tree.anotherpic.thumb", pRoot2 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL(  108, pThumb->GetTag() );

    //  typed paths
    pThumb = g_pObjectServer->FindObject( "group:thumb", pRoot1 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL( 11, pThumb->GetTag() );

    pThumb = g_pObjectServer->FindObject( "object:*", pRoot1 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL( 40, pThumb->GetTag() );

    pThumb = g_pObjectServer->FindObject( "object:thumb", pRoot1 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL( 88, pThumb->GetTag() ); 

    pThumb = g_pObjectServer->FindObject( "group:*", pRoot1 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL(  40, pThumb->GetTag() );

    pThumb = g_pObjectServer->FindObject( "nonexistant_class:*", pRoot1 );
    CHECK( pThumb == NULL );

    pThumb = g_pObjectServer->FindObject( "group:nonexistant_obj", pRoot1 );
    CHECK( pThumb == NULL );

    pThumb = g_pObjectServer->FindObject( "*:nonexistant_obj", pRoot1 );
    CHECK( pThumb == NULL );

    pThumb = g_pObjectServer->FindObject( "*:*", pRoot1 );
    CHECK( pThumb != NULL );
    CHECK_EQUAL( 40, pThumb->GetTag() );

    pRoot1->Release();
    pRoot2->Release();
}
