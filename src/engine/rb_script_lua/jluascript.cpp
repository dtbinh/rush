//****************************************************************************/
//  File:  JLuaScript.cpp
//****************************************************************************/
#include "precompile.h"
#include "jluascript.h"
#include "jluaserver.h"
#include "ifileserver.h"

//****************************************************************************/
/*  JLuaScript implementation
//****************************************************************************/
decl_class(JLuaScript);
bool JLuaScript::Load()
{
    m_bError = true;
    lua_State* pLua = JLuaServer::s_pInstance->GetLuaState();
    
    if (!pLua)
    {
        return false;
    }

    if (m_ChunkID != -1)
    {
        Release();
    }

    //  open script
    FInStream is;
    if (!g_pFileServer->OpenMedia( m_Name.c_str(), "lua", is )) 
    {
        return false;
    }
    
    int nBytes = is.GetTotalSize();
    if (nBytes == 0)
    {
        return false;
    }

    m_Code.resize( nBytes + 1 );
    is.Read( &(m_Code[0]), nBytes );
    m_Code[nBytes] = 0;

    //  compile script
    const char* pScriptText = m_Code.c_str();
    int status  = luaL_loadbuffer( pLua, pScriptText, strlen( pScriptText ), m_Name.c_str() );
    if (status != 0)  
    {
        JString res;
        JLuaServer::s_pInstance->GetStack( pLua, lua_gettop( pLua ), &res );
        rlog.err( "LUA: Could not compile script <%s>: %s", m_Name.c_str(), res.c_str() );
        return false;
    }

    m_ChunkID = luaL_ref( pLua, LUA_REGISTRYINDEX );
    m_bError = false;
    return true;
} // JLuaScript::Load

void JLuaScript::Release()
{
    if (JLuaServer::s_pInstance == NULL) return;
    lua_State* pLua = JLuaServer::s_pInstance->GetLuaState();
    if (!pLua) return;
    luaL_unref( pLua, LUA_REGISTRYINDEX, m_ChunkID );
    m_ChunkID = -1;
} // JLuaScript::Release