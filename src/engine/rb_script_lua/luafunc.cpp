//****************************************************************************/
//  File:  LuaFunc.cpp
//****************************************************************************/
#include "precompile.h"
#include "ipersistserver.h"
#include "jluascript.h"
#include "jluathread.h"
#include "jluaserver.h"

//****************************************************************************/
/*  JLuaServer implementation
//****************************************************************************/
int JLuaServer::FnDumpStack( lua_State* pLua )
{
    rlog.err( "LUA: Error in lua script: %s", lua_tostring( pLua, -1 ) );
    return 0;
} // JLuaServer::FnDumpStack

int JLuaServer::FnObj( lua_State* pLua )
{
    if (lua_gettop( pLua ) > 2 || lua_gettop( pLua ) < 1)
    {
        rlog.err( "LUA: Incorrect command usage: <obj>. "
            "Must be: obj( <objectName>[, <contextObjectRef> | <contextObjectName>] )" );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    if (!lua_isstring( pLua, 1 ))
    {
        rlog.err( "LUA: Incorrect command usage: <obj>. "
            "Must be: obj( <objectName>[, <contextObjectRef> | <contextObjectName>] )" );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }

    const char* pPath = lua_tostring( pLua, 1 );

    //  extract anchor object
    JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
    assert( pThread->m_pLua == pLua );
    JObject* pAnchorObj = pThread->m_pRootObj;
    if (lua_gettop( pLua ) == 2)
    {
        const char* pAnchorName = lua_tostring( pLua, 2 );
        if (!pAnchorName)
        {
            pAnchorObj = (JObject*)lua_touserdata( pLua, 2 );
        }
        else
        {
            pAnchorObj = g_pObjectServer->FindObject( pAnchorName, NULL, pAnchorObj );
        }
    }

    JObject* pObj = g_pObjectServer->FindObject( pPath, NULL, pAnchorObj );
    if (pObj == NULL)
    {
        rlog.warn( "LUA: Could not find object by name: %s", pPath );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    lua_pushlightuserdata( pLua, pObj );
    return 1;
} // JLuaServer::FnObj


int JLuaServer::FnGet( lua_State* pLua )
{
    if (lua_gettop( pLua ) < 2 || lua_gettop( pLua ) > 3)
    {
        rlog.err( "LUA: Incorrect command usage: <get>. "
            "Function takes 2 or 3 arguments (<objectRef>|<objectName>, <propName>, [tag]), but %d is provided", 
            lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    JObject* pObj = NULL;
    const char* pPath = lua_tostring( pLua, 1 );
    if (pPath)
    {
        JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
        assert( pThread->m_pLua == pLua );
        JObject* pRootObj = pThread->m_pRootObj;
        pObj = g_pObjectServer->FindObject( pPath, NULL, pRootObj );
    }
    else
    {
        pObj = reinterpret_cast<JObject*>( lua_touserdata( pLua, 1 ) );
    }
    const char* pPropName = lua_tostring( pLua, 2 );
    int tag = -1;
    if (lua_isnumber( pLua, 3 ))
    {
        tag = (int)lua_tonumber( pLua, 3 );
    }

    if (!pObj)
    {
        rlog.warn( "LUA: Trying to call <get> for the nil object. Nil is returned." );
        lua_pushnil( pLua );
        return 1;
    }

    static JString val;
    bool bRes = pObj->GetProperty( pPropName, val, tag );
    if (!bRes)
    {
        rlog.err( "LUA: Could not get property '%s' for object '%s' of type '%s'.", 
            pPropName, pObj->GetName(), pObj->ClassName() );
    }

    lua_pushstring( pLua, val.c_str() );
    return 1;
} // JLuaServer::FnGet

int JLuaServer::FnSet( lua_State* pLua )
{
    if (lua_gettop( pLua ) < 3 || lua_gettop( pLua ) > 4)
    {
        rlog.err( "LUA: Incorrect command usage: <set>. "
            "Function takes 3 or 4 arguments (<objectRef>|<objectName>, <propName>, <val>, [tag]), but %d is provided", 
            lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    JObject* pObj = NULL;
    const char* pPath = lua_tostring( pLua, 1 );
    if (pPath)
    {
        JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
        assert( pThread->m_pLua == pLua );
        JObject* pRootObj = pThread->m_pRootObj;
        pObj = g_pObjectServer->FindObject( pPath, NULL, pRootObj );
    }
    else
    {
        pObj = reinterpret_cast<JObject*>( lua_touserdata( pLua, 1 ) );
    }
    const char* pPropName = lua_tostring( pLua, 2 );
    const char* pPropVal  = lua_tostring( pLua, 3 );

    if (!pPropVal && lua_isboolean( pLua, 3 ))
    {
        bool bVal = lua_toboolean( pLua, 3 ) == 0 ? false : true;
        pPropVal = bVal ? "true" : "false";
    }

    int tag = -1;
    if (lua_isnumber( pLua, 4 ))
    {
        tag = (int)lua_tonumber( pLua, 4 );
    }

    if (!pObj)
    {
        rlog.warn( "LUA: Trying to call <set> for the nil object." );
        return 0;
    }

    bool bRes = pObj->SetProperty( pPropName, pPropVal, tag );
    if (!bRes)
    {
        rlog.err( "LUA: Could not set property '%s' for object '%s' of type '%s'.", 
            pPropName, pObj->GetName(), pObj->ClassName() );
    }

    return 0;
} // JLuaServer::FnSet

int JLuaServer::FnCall( lua_State* pLua )
{
    if (lua_gettop( pLua ) < 2 || lua_gettop( pLua ) > 3)
    {
        rlog.err( "LUA: Incorrect command usage: <call>. "
            "Function takes 2 to 3 arguments (object|objectName, funcName, [tag]), but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        return 0;
    }

    JObject* pObj = NULL;
    const char* pPath = lua_tostring( pLua, 1 );
    if (pPath)
    {
        JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
        assert( pThread->m_pLua == pLua );
        JObject* pRootObj = pThread->m_pRootObj;
        pObj = g_pObjectServer->FindObject( pPath, NULL, pRootObj );
        if (!pObj)
        {
            rlog.warn( "LUA: Method <call> refers to non-existing object: %s", pPath );
            return NULL;
        }
    }
    else
    {
        pObj = reinterpret_cast<JObject*>( lua_touserdata( pLua, 1 ) );
        if (!pObj)
        {
            rlog.warn( "LUA: Trying to call <call> for the nil object" );
            return NULL;
        }
    }
    
    const char* pMethodName = lua_tostring( pLua, 2 );

    int tag = -1;
    if (lua_isnumber( pLua, 3 ))
    {
        tag = (int)lua_tonumber( pLua, 3 );
    }

    bool bRes = pObj->CallMethod( pMethodName, tag );
    if (!bRes)
    {
        rlog.err( "LUA: Could not call method '%s' for object '%s' of type '%s'.", 
            pMethodName, pObj->GetName(), pObj->ClassName() );
    }
    return 0;
} // JLuaServer::FnCall

int JLuaServer::FnCreate( lua_State* pLua )
{
    int nParam = lua_gettop( pLua );
    if (nParam < 1 || nParam > 3)
    {
        rlog.err( "LUA: Incorrect command usage: <create>. "
            "Must be: create( <className or fileName> | <templateObject>, [<newName>, <parentObject>] )" );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    
    //  first parameter
    const char* pClassName = lua_tostring( pLua, 1 );
    JObject* pTemplateObj = NULL;
    if (!pClassName)
    {
        pTemplateObj = (JObject*)lua_touserdata( pLua, 1 );
    }

    //  second parameter
    const char* pObjName   = lua_tostring( pLua, 2 );

    //  third parameter
    JObject* pParentObj = NULL;
    if (nParam > 2)
    {
        pParentObj = (JObject*)lua_touserdata( pLua, 3 );
    }
    else
    {
        JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
        assert( pThread->m_pLua == pLua );
        pParentObj = pThread->m_pRootObj;
    }

    JObject* pObj = NULL;
    if (pClassName)
    {
        pObj = g_pObjectServer->Create( pClassName );
        //  try to load from script
        if (pObj == NULL)
        {
            pObj = g_pPersistServer->LoadObject( pClassName );
        }
        if (pObj == NULL)
        {
            rlog.err( "LUA: Cannot create an object because of unknown class (or filename): %s", pClassName );
            lua_settop( pLua, 0 );
            lua_pushnil( pLua );
            return 1;
        }
        if (pObjName) 
        {
            pObj->SetName( pObjName );
        }
    }
    else if (pTemplateObj)
    {
        pObj = pTemplateObj->Clone( pParentObj, pObjName );
    }

    if (pParentObj)
    {
        pParentObj->AddChild( pObj );
    }

    lua_settop( pLua, 0 );
    lua_pushlightuserdata( pLua, pObj );
    return 1;
} // JLuaServer::FnCreate

int JLuaServer::FnDelete( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 1)
    {
        rlog.err( "LUA: Incorrect command usage: <delete>. "
            "Function takes 1 object ref argument, but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    
    //  TODO: make this casting safe
    JObject* pObj = reinterpret_cast<JObject*>( lua_touserdata( pLua, -1 ) );
    if (!pObj)
    {
        return 0;
    }
    
    //  delete object
    pObj->Release();

    lua_settop( pLua, 0 );
    return 0;
} // JLuaServer::FnDelete

int JLuaServer::FnChildren( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 1)
    {
        rlog.err( "LUA: Incorrect command usage: <children>. "
            "Function takes 1 argument (<objectRef>|<objectName>), but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }

    JObject* pObj = NULL;
    const char* pPath = lua_tostring( pLua, -1 );
    if (pPath)
    {
        JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
        assert( pThread->m_pLua == pLua );
        JObject* pRootObj = pThread->m_pRootObj;
        pObj = g_pObjectServer->FindObject( pPath, NULL, pRootObj );
    }
    else
    {
        pObj = reinterpret_cast<JObject*>( lua_touserdata( pLua, -1 ) );
    }

    if (!pObj)
    {
        lua_pushnil( pLua );
        return 1;
    }
    
    //  return table with children
    int nCh = pObj->GetNChildren();
    lua_createtable( pLua, nCh, 0 );
    lua_newtable( pLua );
    for (int i = 0; i < nCh; i++)
    {   
        //  key
        lua_pushnumber( pLua, i + 1 ); 
        //  value (child pointer)
        lua_pushlightuserdata( pLua, pObj->GetChild( i ) );
        lua_settable( pLua, -3 );
    }
    return 1;
} // JLuaServer::FnChildren

int JLuaServer::FnParent( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 1)
    {
        rlog.err( "LUA: Incorrect command usage: <parent>. "
            "Function takes 1 argument, but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }

    JObject* pObj = NULL;
    const char* pPath = lua_tostring( pLua, -1 );
    if (pPath)
    {
        JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
        assert( pThread->m_pLua == pLua );
        JObject* pRootObj = pThread->m_pRootObj;
        pObj = g_pObjectServer->FindObject( pPath, NULL, pRootObj );
    }
    else
    {
        pObj = reinterpret_cast<JObject*>( lua_touserdata( pLua, -1 ) );
    }
    
    if (!pObj)
    {
        lua_pushnil( pLua );
        return 1;
    }
    lua_pushlightuserdata( pLua, pObj->GetParent() );
    return 1;
} // JLuaServer::FnParent

int JLuaServer::FnSetParent( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 2)
    {
        rlog.err( "LUA: Incorrect command usage: <setparent>. "
            "Function takes 2 arguments, but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    JObject* pParent = reinterpret_cast<JObject*>( lua_touserdata( pLua, 2 ) );
    JObject* pObj    = reinterpret_cast<JObject*>( lua_touserdata( pLua, 1 ) );
    if (!pObj)
    {
        lua_pushnil( pLua );
        return 0;
    }
    //  set new parent for the object
    JObject* pOldParent = pObj->GetParent();
    if (pParent)
    {
        pParent->AddChild( pObj );
    }
    if (pOldParent)
    {
        pOldParent->RemoveChild( pObj );
    }
    pObj->SetParent( pParent );
    
    return 0;
} // JLuaServer::FnSetParent

int JLuaServer::FnWait( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 1)
    {
        rlog.err( "LUA: Incorrect command usage: <wait>. "
            "Function takes 1 number argument, but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return lua_yield( pLua, 0 );
    }

    JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
    assert( pThread->m_pLua == pLua );
    pThread->m_WaitTime += (double)lua_tonumber( pLua, -1 );
    return lua_yield( pLua, 0 );
} // JLuaServer::FnWait

int JLuaServer::FnFork( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 1)
    {
        rlog.err( "LUA: Incorrect command usage: <fork>. "
            "Function takes 1 argument, but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        return 0;
    }
    const char* funcName = lua_tostring( pLua, -1 );
    if (!funcName)
    {
         rlog.err( "LUA: Incorrect command usage: <fork>. "
            "Function expects 1 string argument, for example: fork('MyFunction')" );
        lua_settop( pLua, 0 );
        return 0;
    }
    
    JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
    assert( pThread && pThread->m_pLua == pLua );
    JLuaServer::s_pInstance->SpawnChildThread( pThread, funcName );
    return 0;
} // JLuaServer::FnFork

int JLuaServer::FnRequire( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 1)
    {
        rlog.err( "LUA: Incorrect command usage: <require>. "
            "Function takes 1 string argument, but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    if (!lua_isstring( pLua, -1 ))
    {
        rlog.err( "LUA: Incorrect command usage: <require>. "
            "Function expects 1 string argument, for example: require('myfile.lua')" );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    const char* pScriptName = lua_tostring( pLua, -1 );

    Path path( pScriptName );
    int scriptID = GetScriptID( path.GetFile() );
    JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
    assert( pThread->m_pLua == pLua );
    JObject* pRootObj = pThread->m_pRootObj;
    JString res;
    int threadID = RunScript( scriptID, pRootObj, &res );
    return 0;
} // JLuaServer::FnRequire

int JLuaServer::FnConnect( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 3)
    {
        rlog.err( "LUA: Incorrect command usage: <connect>. "
            "Function takes 3 arguments (<objectRef>, <propName>, <luaFuncName>), but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }

    JLuaThread* pThread = reinterpret_cast<JLuaThread*>( pLua->userdata );
    assert( pThread->m_pLua == pLua );

    JObject* pObj = NULL;
    const char* pPath = lua_tostring( pLua, -3 );
    if (pPath)
    {

        JObject* pRootObj = pThread->m_pRootObj;
        pObj = g_pObjectServer->FindObject( pPath, NULL, pRootObj );
    }
    else
    {
        pObj = reinterpret_cast<JObject*>( lua_touserdata( pLua, -3 ) );
    }
    const char* pProp = lua_tostring( pLua, -2 );
    const char* pFunc = lua_tostring( pLua, -1 );

    if (!pObj)
    {
        rlog.warn( "LUA: Trying to call <connect> for the nil source object." );
        return 0;
    }

    //  connecting to the root thread
    while (pThread->m_pParent) 
    {
        pThread = pThread->m_pParent;
    }
    
    pThread->Connect( pObj, pProp, pFunc );

    return 0;
} // JLuaServer::FnConnect

int JLuaServer::FnLog( lua_State* pLua )
{
    if (lua_gettop( pLua ) != 1)
    {
        rlog.err( "LUA: Incorrect command usage: <log>. "
            "Function takes 1 string argument, but %d is provided", lua_gettop( pLua ) );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    if (!lua_isstring( pLua, -1 ))
    {
        rlog.err( "LUA: Incorrect command usage: <log>. "
            "Function expects 1 string argument, for example: log('hello, world!')" );
        lua_settop( pLua, 0 );
        lua_pushnil( pLua );
        return 1;
    }
    const char* pLog = lua_tostring( pLua, -1 );
    rlog.msg( "LUA: %s", pLog );
    return 0;
} // JLuaServer::FnLog
    