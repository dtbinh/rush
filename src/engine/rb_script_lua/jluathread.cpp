//****************************************************************************/
//  File:  JLuaThread.cpp
//****************************************************************************/
#include "precompile.h"
#include "JSignalServer.h"
#include "JLuaScript.h"
#include "JLuaServer.h"
#include "JLuaThread.h"

//****************************************************************************/
/*  JLuaThread implementation
//****************************************************************************/
decl_class(JLuaThread);

const char* JLuaThread::GetScriptName() const
{
    if (m_pScript)
    {
        return m_pScript->GetName();
    }
    else if (m_pParent)
    {
        return m_pParent->GetScriptName();
    }
    else
    {
        return "command line";
    }
} // JLuaThread::GetScriptName

//!  starts thread for some chunk (script) pScript
int JLuaThread::Start( lua_State* pLua, JLuaScript* pScript, JObject* pRootObj, JString* pResult )
{
    if (!pScript || pScript->m_ChunkID < 0 || pScript->m_bError)
    {
        return -1;
    }
    m_pRootObj  = pRootObj;
    m_pScript   = pScript;
    m_WaitTime  = 0.0f;
    m_pLua      = lua_newthread( pLua );
    m_pLua->userdata = this;
    
    //  anchor created thread, so it does not get occasionally garbage-collected
    m_AnchorID  = luaL_ref( pLua, LUA_REGISTRYINDEX );
    
    if (!m_pLua)
    {
        rlog.err( "LUA: Could not create thread for script <%s>", GetScriptName() );
        pScript->m_bError = true;
        return -1;
    }

    if (pScript)
    {
        lua_rawgeti( m_pLua, LUA_REGISTRYINDEX, pScript->m_ChunkID );
    }
    int status = lua_resume( m_pLua, 0 );

    if (status != 0 && status != LUA_YIELD) 
    {
        JString res;
        JLuaServer::s_pInstance->GetStack( m_pLua, lua_gettop( m_pLua ), &res );
        rlog.err( "LUA: Could not run script <%s>: %s", GetScriptName(), res.c_str() );
        pScript->m_bError = true;
        return status;
    }

    if (pResult) pResult->clear();
    JLuaServer::s_pInstance->GetStack( m_pLua, 1, pResult );
    return status;
} // JLuaThread::Start

//!  starts thread for some function (funcName), pArgument is optional argument, passed to function
int JLuaThread::Start( lua_State* pLua, const char* funcName, JLuaThread* pParent, 
    JObject* pArgument, JString* pResult )
{
    if (!funcName)
    {
        return -1;
    }
    m_pRootObj          = pParent->m_pRootObj;
    m_pScript           = NULL;
    m_WaitTime          = 0.0f;
    m_pParent           = pParent;
    m_pLua              = lua_newthread( pLua );
    m_pLua->userdata    = this;
    
    //  anchor created thread, so it does not get occasionally garbage-collected
    m_AnchorID  = luaL_ref( pLua, LUA_REGISTRYINDEX );
    
    if (!m_pLua)
    {
        rlog.err( "LUA: Could not create thread for function <%s>", funcName );
        return -1;
    }

    lua_getfield( m_pLua, LUA_GLOBALSINDEX, funcName );
    if (lua_gettop( m_pLua ) == 0)
    {
        rlog.err( "Lua function <%s> does not exist in context of script <%s>", 
            funcName, GetScriptName() );
        return false;
    }
    
    int nArgs = 0;
    if (pArgument)
    {
        lua_pushlightuserdata( m_pLua, pArgument );
        nArgs = 1;
    }

    int status = lua_resume( m_pLua, nArgs );

    if (status != 0 && status != LUA_YIELD) 
    {
        JString res;
        JLuaServer::s_pInstance->GetStack( m_pLua, lua_gettop( m_pLua ), &res );
        rlog.err( "LUA: Could not run function <%s> in script <%s>: %s", 
            funcName, GetScriptName(), res.c_str() );
        return status;
    }

    if (pResult) pResult->clear();
    JLuaServer::s_pInstance->GetStack( m_pLua, 1, pResult );
    return status;
} // JLuaThread::Start

//!  starts thread to execute command line script
int JLuaThread::Start( lua_State* pLua, const char* pScript, 
                        JObject* pRootObj, JString* pResult )
{
    m_pRootObj  = pRootObj;
    m_WaitTime  = 0.0f;
    m_pScript   = NULL;
    m_pLua      = lua_newthread( pLua );
    m_pLua->userdata = this;
    
    if (!m_pLua)
    {
        rlog.err( "LUA: Could not create thread for script [%s]", pScript );
        return -1;
    }

    int status  = luaL_loadbuffer( m_pLua, pScript, strlen( pScript ), pScript );
    if (status != 0)  
    {
        JString res;
        JLuaServer::s_pInstance->GetStack( m_pLua, lua_gettop( m_pLua ) - 1, &res );
        rlog.err( "LUA: Could not compile command-line script [%s]: %s", pScript, res.c_str() );
        return -1;
    }

    status = lua_resume( m_pLua, 0 );

    if (status != 0 && status != LUA_YIELD) 
    {
        JString res;
        JLuaServer::s_pInstance->GetStack( m_pLua, lua_gettop( m_pLua ), &res );
        rlog.err( "LUA: Could not run script [%s]: %s", pScript, res.c_str() );
        return status;
    }

    if (pResult) pResult->clear();
    JLuaServer::s_pInstance->GetStack( m_pLua, 1, pResult );
    return status;
} // JLuaThread::Start

int JLuaThread::Resume()
{
    int status = lua_resume( m_pLua, 0 );

    if (status != 0 && status != LUA_YIELD) 
    {
        if (m_pScript)
        {
            if (m_pScript->m_bError) return status;
            m_pScript->m_bError = true;
        }
        JString res;
        JLuaServer::s_pInstance->GetStack( m_pLua, lua_gettop( m_pLua ), &res );
        rlog.err( "LUA: Error resuming script thread: %s", res.c_str() );
        return status;
    }
    return status;
} // JLuaThread::Resume

bool JLuaThread::Update( double dTime )
{
    if (dTime < m_WaitTime)
    {
        //  still waiting
        m_WaitTime -= dTime;
        return true;
    }
    //  resume thread needed amount of times
    do
    {
        dTime -= m_WaitTime;
        m_WaitTime = 0.0f;
        int status = Resume();
        if (status != LUA_YIELD)
        {
            //  thread has finished
            return false;
        }
    }while (dTime > m_WaitTime);
    m_WaitTime -= dTime;
    return true;
} // JLuaThread::Update

void JLuaThread::Stop( lua_State* pLua )
{
    if (m_AnchorID >= 0 && m_pLua)
    {
        luaL_unref( pLua, LUA_REGISTRYINDEX, m_AnchorID );
        m_AnchorID = -1;
    }
} // JLuaThread::Stop

bool JLuaThread::Connect( JObject* pSrc, const char* pSrcProp, const char* pFunc )
{
    bool bRes = JSignalServer::s_pInstance->Connect( pSrc, pSrcProp, this, "input" );
    m_Signals.push_back( JThreadSignal( pSrc, pSrcProp, pFunc ) );

    //JSignalServer::s_pInstance->DumpSignals();
    return bRes;
} // JLuaThread::Connect

void JLuaThread::Input()
{
    JObject* pObj = JSignalServer::s_pInstance->GetSourceObject();
    const char* pAttr = JSignalServer::s_pInstance->GetSourceAttr();
    if (!pObj || !pAttr) return;
    int nSig = m_Signals.size();
    for (int i = 0; i < nSig; i++)
    {
        JThreadSignal& sig = m_Signals[i];
        if (sig.m_pObj == NULL) continue;
        if (!sig.IsEqual( pObj, pAttr )) continue;
        
        JLuaThread* pChildThread = 
            JLuaServer::s_pInstance->SpawnChildThread( this, sig.m_Func.c_str(), pObj );
        if (!pChildThread)
        {
            return;
        }
    }
} // JLuaThread::Input

bool JLuaThread::AddChild( JLuaThread* pChild )
{
    if (m_pChild)
    {
        pChild->m_pSibling = m_pChild;
    }

    m_pChild = pChild;
    return true;
}

bool JLuaThread::RemoveChild( JLuaThread* pChild )
{
    if (m_pChild == pChild)
    {
        m_pChild = m_pChild->m_pSibling;
        pChild->m_pSibling = NULL;
        return true;
    }
    JLuaThread* pCurChild = m_pChild;
    while (pCurChild->m_pSibling)
    {
        if (pCurChild->m_pSibling == pChild)
        {
            pCurChild->m_pSibling = pChild->m_pSibling;
            pChild->m_pSibling = NULL;
            return true;
        }
        pCurChild = pCurChild->m_pSibling;
    }
    return false;
}



