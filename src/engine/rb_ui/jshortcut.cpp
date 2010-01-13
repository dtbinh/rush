/***********************************************************************************/
//  File:   JShortcut.cpp
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JShortcut.h"

/***********************************************************************************/
/*  JShortcut implementation
/***********************************************************************************/
decl_class(JShortcut);
JShortcut::JShortcut()
{
    m_Key       = 0;
    m_bControl  = false;
    m_bAlt      = false;
    m_bShift    = false;

    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
} // JShortcut::JShortcut

void JShortcut::OnKey( JKeyEvent& e )
{

    if (e.Key() == m_Key && e.Action() == aKeyUp)
    {
        if (m_bControl  && GetKeyState( VK_CONTROL ) >= 0) return;
        if (m_bAlt      && GetKeyState( VK_MENU ) >= 0) return;
        if (m_bShift    && GetKeyState( VK_SHIFT ) >= 0) return;
        
        Action();
        if (IsConsumeEvents())
        {
            e.Consume();
        }
    }
} // JShortcut::OnKey

void JShortcut::Action()
{  
    SendSignal( "Action" );
} // JShortcut::Action
