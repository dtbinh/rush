//****************************************************************************/
//  File:  JScriptEvent.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JScript.h"
#include "JScriptEvent.h"

//****************************************************************************/
/*  JScriptEvent implementation
//****************************************************************************/
decl_class(JScriptEvent);
JScriptEvent::JScriptEvent()
{
    m_pScript = NULL;
} // JScriptEvent::JScriptEvent

void JScriptEvent::Init()
{   
    m_pScript = obj_cast<JScript>( g_pObjectServer->FindObject( m_Script.c_str(), NULL, this ) );

    //  check whether we have connected properly
    static JString path;
    if (!m_pScript)
    {
        GetPath( path );
        rlog.err( "JScriptEvent at path <%s>: Could not find script object <%s>", path.c_str(), m_Script.c_str() );
        return;
    }
} // JScriptEvent::Init

void JScriptEvent::OnPlay()
{
    if (!m_pScript)
    {
        Init();
    }
    if (m_pScript)
    {
        m_pScript->Dispatch( m_Event.c_str() );
    }
    Stop();
} // JScriptEvent::OnPlay