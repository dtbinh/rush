//****************************************************************************/
//  File:   JDialog.cpp
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "jcore.h"
#include "jdialog.h"

//****************************************************************************/
/*	JDialog implementation
//****************************************************************************/
decl_class(JDialog);
JDialog::JDialog()
{
    m_bModal        = false;
    m_bFloating     = false;
    m_Result        = drUnknown;
    SetClipToExt( true );
} // JDialog::JDialog

void JDialog::OnOK()
{
    m_Result = drOK;
}

void JDialog::OnCancel()
{
    m_Result = drCancel;
}

void JDialog::Render()
{
} // JDialog::Render

void JDialog::OnFocus( bool bEnter )
{
    JWidget* pFocus = g_pObjectServer->FindObject<JWidget>( m_DefaultFocus.c_str(), this );
    if (!pFocus || !bEnter) return;
    pFocus->SetFocus( bEnter );
} // JDialog::OnFocus

void JDialog::Show()
{
    SetVisible( true );
    SetEnabled( true );

    //  set this dialog as "root" object for editors
    JString path;
    GetPath( path );
    JCore::s_pInstance->SetRootObject( path.c_str() );

    OnShow();

    SetVisible ( true );
    SetEnabled ( true );
    SetFocus   ( true );
    SendSignal ( "OnShow" );
} // JDialog::Show

void JDialog::Close()
{
    SetVisible( false );
    OnClose();
    SendSignal( "OnClose" );
} // JDialog::Close




