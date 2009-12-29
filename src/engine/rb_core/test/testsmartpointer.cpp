#include "stdafx.h"
#include "JPtr.h"
#include "JGroup.h"

TEST( SmartPtrIsInitiallized )
{
    JPtr<JGroup> ptr;
    CHECK( ptr == NULL );
}

static JGroup* s_pDlg = NULL;
TEST( SmartPtrIncreasesRefCount )
{
    s_pDlg = new JGroup();
    s_pDlg->AddRef();
    CHECK_EQUAL( 1, s_pDlg->GetNRef() );
    
    JPtr<JGroup> pDialog = s_pDlg;

    CHECK_EQUAL( 2, s_pDlg->GetNRef() );
    CHECK( pDialog == s_pDlg );

    JPtr<JGroup> pDialog1 = s_pDlg;
    CHECK( pDialog == s_pDlg );
    CHECK( pDialog1 == s_pDlg );
    CHECK( pDialog1 == pDialog );
    CHECK_EQUAL( 3, s_pDlg->GetNRef() );

    pDialog1 = pDialog;
    CHECK_EQUAL( 3, s_pDlg->GetNRef() );
}

TEST( SmartPtrRefCountDecreasesAfterLeavingScope )
{
    CHECK_EQUAL( 1, s_pDlg->GetNRef() );
}
