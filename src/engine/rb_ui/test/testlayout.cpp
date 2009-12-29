#include "stdafx.h"
#include "JButton.h"

TEST( Layout )
{
    JObject* pRoot = 
        obj_cast<JObject>( g_pPersistServer->LoadObject( "testlayout" ) );
    CHECK( pRoot != NULL );

    JWidget* pComboB   = g_pObjectServer->FindObject<JWidget>( "comboB", pRoot );
    JWidget* pComboA   = g_pObjectServer->FindObject<JWidget>( "comboA", pRoot );
    JWidget* pButtonAB = g_pObjectServer->FindObject<JWidget>( "buttonAB", pRoot );
    JWidget* pButtonA  = g_pObjectServer->FindObject<JWidget>( "buttonA", pRoot );
    JWidget* pButtonB  = g_pObjectServer->FindObject<JWidget>( "buttonB", pRoot );
    JWidget* pButtonC  = g_pObjectServer->FindObject<JWidget>( "buttonC", pRoot );
    JWidget* pNice     = g_pObjectServer->FindObject<JWidget>( "nice", pRoot );
    JWidget* pWindowA  = g_pObjectServer->FindObject<JWidget>( "black_window", pRoot );
    JWidget* pWindowB  = g_pObjectServer->FindObject<JWidget>( "white_window", pRoot );

    CHECK( pComboB != NULL && pComboA != NULL && pButtonAB != NULL && pButtonA != NULL &&
           pButtonB != NULL && pButtonC != NULL && pNice != NULL && pWindowA != NULL &&
           pWindowB != NULL );

    CHECK_EQUAL( 1.0f, pWindowA->GetExt().x ); 
    CHECK_EQUAL( 2.0f, pWindowA->GetExt().y );
    CHECK_EQUAL( 1.0f, pWindowA->GetLocalExt().x );
    CHECK_EQUAL( 2.0f, pWindowA->GetLocalExt().y );

    CHECK_EQUAL( -99.0f, pButtonA->GetExt().x );
    CHECK_EQUAL( 22.0f, pButtonA->GetExt().y );
    CHECK_EQUAL( -100.0f, pButtonA->GetLocalExt().x );
    CHECK_EQUAL( 20.0f, pButtonA->GetLocalExt().y );
    CHECK_EQUAL( 15.0f, pButtonA->GetExt().w );
    CHECK_EQUAL( 15.0f, pButtonA->GetLocalExt().w );

    CHECK_EQUAL( -89.0f, pButtonC->GetExt().x );
    CHECK_EQUAL( 42.0f, pButtonC->GetExt().y );
    CHECK_EQUAL( 10.0f, pButtonC->GetLocalExt().x );
    CHECK_EQUAL( 20.0f, pButtonC->GetLocalExt().y );
    CHECK_EQUAL( 50.0f, pButtonC->GetExt().w );
    CHECK_EQUAL( 50.0f, pButtonC->GetLocalExt().w );

    pWindowA->SetLocalExt( Frame( 100, 100, 20, 20 ) );

    CHECK_EQUAL( 0.0f, pButtonA->GetExt().x );
    CHECK_EQUAL( 120.0f, pButtonA->GetExt().y );
    CHECK_EQUAL( -100.0f, pButtonA->GetLocalExt().x );
    CHECK_EQUAL( 20.0f, pButtonA->GetLocalExt().y );
    CHECK_EQUAL( 15.0f, pButtonA->GetExt().w );
    CHECK_EQUAL( 15.0f, pButtonA->GetLocalExt().w );

    CHECK_EQUAL( 10.0f, pButtonC->GetExt().x );
    CHECK_EQUAL( 140.0f, pButtonC->GetExt().y );
    CHECK_EQUAL( 10.0f, pButtonC->GetLocalExt().x );
    CHECK_EQUAL( 20.0f, pButtonC->GetLocalExt().y );
    CHECK_EQUAL( 50.0f, pButtonC->GetExt().w );
    CHECK_EQUAL( 50.0f, pButtonC->GetLocalExt().w );
}
