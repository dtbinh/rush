#include "precompile.h"
#include "jobject.h"

TEST(Object)
{
    int objSize = sizeof( JObject );
    CHECK( objSize < 128 );
}

TEST(ObjectIndex)
{
    JObject* pRoot = new JObject();

    JObject* pCh1 = new JObject(); pCh1->SetName( "ch1" );
    JObject* pCh2 = new JObject(); pCh2->SetName( "ch2" );
    JObject* pCh3 = new JObject(); pCh3->SetName( "ch3" );
    JObject* pCh4 = new JObject(); pCh4->SetName( "ch4" );
    JObject* pCh5 = new JObject(); pCh5->SetName( "ch5" );

    pRoot->AddChild( pCh1 );
    pRoot->AddChild( pCh2 );
    pRoot->AddChild( pCh3 );
    pRoot->AddChild( pCh4 );
    pRoot->AddChild( pCh5 );

    CHECK_EQUAL( pCh1, pRoot->GetChild( 0 ) );

    // 2 3 4 5 1
    pCh1->SetIndex( 10000 );
    CHECK_EQUAL( pCh2, pRoot->GetChild( 0 ) );
    CHECK_EQUAL( pCh3, pRoot->GetChild( 1 ) );
    CHECK_EQUAL( pCh4, pRoot->GetChild( 2 ) );
    CHECK_EQUAL( pCh5, pRoot->GetChild( 3 ) );
    CHECK_EQUAL( pCh1, pRoot->GetChild( 4 ) );

    // 2 4 5 1 3
    pCh3->SetIndex( 4 );
    CHECK_EQUAL( pCh2, pRoot->GetChild( 0 ) );
    CHECK_EQUAL( pCh4, pRoot->GetChild( 1 ) );
    CHECK_EQUAL( pCh5, pRoot->GetChild( 2 ) );
    CHECK_EQUAL( pCh1, pRoot->GetChild( 3 ) );
    CHECK_EQUAL( pCh3, pRoot->GetChild( 4 ) );

    // 2 4 5 1 3
    pCh1->SetIndex( 3 );
    CHECK_EQUAL( pCh2, pRoot->GetChild( 0 ) );
    CHECK_EQUAL( pCh4, pRoot->GetChild( 1 ) );
    CHECK_EQUAL( pCh5, pRoot->GetChild( 2 ) );
    CHECK_EQUAL( pCh1, pRoot->GetChild( 3 ) );
    CHECK_EQUAL( pCh3, pRoot->GetChild( 4 ) );

    // 5 2 4 1 3
    pCh5->SetIndex( -3 );
    CHECK_EQUAL( pCh5, pRoot->GetChild( 0 ) );
    CHECK_EQUAL( pCh2, pRoot->GetChild( 1 ) );
    CHECK_EQUAL( pCh4, pRoot->GetChild( 2 ) );
    CHECK_EQUAL( pCh1, pRoot->GetChild( 3 ) );
    CHECK_EQUAL( pCh3, pRoot->GetChild( 4 ) );

    // 5 1 2 4 3
    pCh1->SetIndex( 1 );
    CHECK_EQUAL( pCh5, pRoot->GetChild( 0 ) );
    CHECK_EQUAL( pCh1, pRoot->GetChild( 1 ) );
    CHECK_EQUAL( pCh2, pRoot->GetChild( 2 ) );
    CHECK_EQUAL( pCh4, pRoot->GetChild( 3 ) );
    CHECK_EQUAL( pCh3, pRoot->GetChild( 4 ) );

    // 5 2 4 1 3
    pCh1->SetIndex( 3 );
    CHECK_EQUAL( pCh5, pRoot->GetChild( 0 ) );
    CHECK_EQUAL( pCh2, pRoot->GetChild( 1 ) );
    CHECK_EQUAL( pCh4, pRoot->GetChild( 2 ) );
    CHECK_EQUAL( pCh1, pRoot->GetChild( 3 ) );
    CHECK_EQUAL( pCh3, pRoot->GetChild( 4 ) );

    pRoot->Release();
}