#include "stdafx.h"
#include "JPersistServer.h"
#include "JGrid.h"

TEST( Grid )
{
    JGrid* pTestGrid = new JGrid();
    pTestGrid->SetExt( Frame( 100, 100, 500, 500 ) );

    const int c_NWidgets = 8;
    JWidget* pWidget[c_NWidgets];
    for (int i = 0; i < c_NWidgets; i++)
    {
        pWidget[i] = new JWidget();
        pWidget[i]->SetExt( Frame( 1000, -100, i*10, 50 ) );
        pTestGrid->AddChild( pWidget[i] );
    }

    //  horizontal
    pTestGrid->SetRowsCols( 1, 8 );
    CHECK_EQUAL( 100, pWidget[0]->GetExt().x );
    CHECK_EQUAL( 110, pWidget[1]->GetExt().x );
    CHECK_EQUAL( 130, pWidget[2]->GetExt().x );
    CHECK_EQUAL( 160, pWidget[3]->GetExt().x );
    CHECK_EQUAL( 200, pWidget[0]->GetExt().x );
    CHECK_EQUAL( 250, pWidget[1]->GetExt().x );
    CHECK_EQUAL( 310, pWidget[2]->GetExt().x );
    CHECK_EQUAL( 380, pWidget[3]->GetExt().x );

    CHECK_EQUAL( 100, pWidget[0]->GetExt().y );
    CHECK_EQUAL( 100, pWidget[1]->GetExt().y );
    CHECK_EQUAL( 100, pWidget[2]->GetExt().y );
    CHECK_EQUAL( 100, pWidget[3]->GetExt().y );
    CHECK_EQUAL( 100, pWidget[0]->GetExt().y );
    CHECK_EQUAL( 100, pWidget[1]->GetExt().y );
    CHECK_EQUAL( 100, pWidget[2]->GetExt().y );
    CHECK_EQUAL( 100, pWidget[3]->GetExt().y );

    //  vertical
    pTestGrid->SetRowsCols( 8, 1 );
    CHECK_EQUAL( 100, pWidget[0]->GetExt().y );
    CHECK_EQUAL( 110, pWidget[1]->GetExt().y );
    CHECK_EQUAL( 130, pWidget[2]->GetExt().y );
    CHECK_EQUAL( 160, pWidget[3]->GetExt().y );
    CHECK_EQUAL( 200, pWidget[0]->GetExt().y );
    CHECK_EQUAL( 250, pWidget[1]->GetExt().y );
    CHECK_EQUAL( 310, pWidget[2]->GetExt().y );
    CHECK_EQUAL( 380, pWidget[3]->GetExt().y );

    CHECK_EQUAL( 100, pWidget[0]->GetExt().x );
    CHECK_EQUAL( 100, pWidget[1]->GetExt().x );
    CHECK_EQUAL( 100, pWidget[2]->GetExt().x );
    CHECK_EQUAL( 100, pWidget[3]->GetExt().x );
    CHECK_EQUAL( 100, pWidget[0]->GetExt().x );
    CHECK_EQUAL( 100, pWidget[1]->GetExt().x );
    CHECK_EQUAL( 100, pWidget[2]->GetExt().x );
    CHECK_EQUAL( 100, pWidget[3]->GetExt().x );

    pTestGrid->Release();
}
