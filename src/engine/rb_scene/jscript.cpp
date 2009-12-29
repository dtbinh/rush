//****************************************************************************/
//  File:  JScript.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "JAnimation.h"
#include "IScriptServer.h"
#include "JScript.h"

//****************************************************************************/
/*  JScript implementation
//****************************************************************************/
decl_class(JScript);
JScript::JScript()
{
    m_ScriptID = -1;
    m_ThreadID = -1;
}

JScript::~JScript()
{
    StopThread();
} // JScript::~JScript

void JScript::OnPlay()
{
    StopThread();

    if (m_ScriptID == -1)
    {
        m_ScriptID = g_pScriptServer->GetScriptID( m_Script.c_str() );
    }

    JString res;
    m_ThreadID = g_pScriptServer->RunScript( m_ScriptID, this, &res );
    if (m_ThreadID == 0)
    {
        m_Result = res.c_str();
        Stop();
        m_ThreadID = -1;
    }
    else
    {
        //  if duration is unknown, assign it to the infinite and wait until script finishes 
        if (GetDuration() == 0.0f)
        {
            const float c_VeryLongTime = 1000000.0f;
            SetDuration( c_VeryLongTime );
        }
    }
} // JScript::OnPlay

void JScript::Render()
{
    JAnimation::Render();
    JAnmContext& ctx = GetContext();
    float dt = GetTempo()*GetContext().m_TimeDelta;
    if (IsPaused()) 
    {
        return;
    }
    bool bRes = g_pScriptServer->UpdateThread( m_ThreadID, dt );
    if (!bRes)
    {
        Stop();
    }
} // JScript::Render

void JScript::OnStop()
{
    StopThread();
} // JScript::OnStop

void JScript::Init()
{
    StopThread();
    m_ScriptID = g_pScriptServer->GetScriptID( m_Script.c_str() );
} // JScript::Init

void JScript::Reload()
{
    g_pScriptServer->ReloadScript( m_ScriptID );

    if (IsPlaying())
    {
        Stop();
        Play();
    }
} // JScript::Reload

void JScript::StopThread()
{
    if (m_ThreadID >= 0 && g_pScriptServer)
    {
        g_pScriptServer->StopThread( m_ThreadID );
        m_ThreadID = -1;
    }
} // JScript::StopThread

void JScript::Dispatch( const char* event )
{
    if (m_ThreadID == -1)
    {
        return;
    }
    g_pScriptServer->RunFunction( m_ThreadID, event );
}

void JScript::Dispatch( int methodID )
{
    if (m_ThreadID == -1 || methodID < 0 || methodID >= m_DispatchTable.size())
    {
        return;
    }

    g_pScriptServer->RunFunction( m_ThreadID, m_DispatchTable[methodID].c_str(), this );
} // JScript::Dispatch

bool JScript::OnSignal( JObject* pSrc, const char* pSrcAttr, const char* pDstAttr )
{
    if (m_ThreadID == -1)
    {
        return false;
    }
    return g_pScriptServer->RunFunction( m_ThreadID, pDstAttr, pSrc );    
}