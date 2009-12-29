//****************************************************************************/
//  File:  JPanel.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JPanel.h"

//****************************************************************************/
/*  JPanel implementation
//****************************************************************************/
decl_class(JPanel);
JPanel::JPanel()
{
    SetBgColor( 0 );
    SetFgColor( 0 );
}

void JPanel::Render()
{   
    Frame    ext = GetExt();
    if ((GetBgColor()&0xFF000000) != 0)
    {
        g_pDrawServer->Flush();
        g_pDrawServer->DrawRect( ext, GetBgColor() ); 
        g_pDrawServer->Flush();
    }
} // JPanel::Render

void JPanel::OnMouse( JMouseEvent& e )
{ 
    if (e.Action() == aKeyDown && e.MouseKey() == mkLeft)
    {
        if (IsFocusable() && !HasFocus())
        {
            SetFocus();
            e.Consume();
        }
    }

    if (e.Action() == aDblClick && e.MouseKey() == mkLeft)
    {
        SendSignal( "OnLBDbl" );
        OnLBDbl();
    }

    if (IsConsumeEvents())
    {
        e.Consume();
    }
} // JPanel::OnMouse

void JPanel::OnKey( JKeyEvent& e )
{
    /*if (IsConsumeEvents())
    {
        e.Consume();
    }*/
} // JPanel::OnKey

void JPanel::PostRender()
{
    g_pDrawServer->Flush();
} // JPanel::PostRender
    



