//****************************************************************************/
//  File:  JScriptEvent.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "jscript.h"
#include "jscriptevent.h"

//****************************************************************************/
/*  JScriptEvent implementation
//****************************************************************************/
decl_class(JScriptEvent);
JScriptEvent::JScriptEvent()
{
    m_pScript = NULL;
}

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
}

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
}
