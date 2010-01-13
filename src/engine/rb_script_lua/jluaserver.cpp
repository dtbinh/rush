//****************************************************************************/
//  File:  JLuaServer.cpp
//****************************************************************************/
#include "precompile.h"
#include "stdlib.h"
#include "float.h"
#include "jstring.h"
#include "fstream.h"
#include "path.h"
#include "jobjectserver.h"
#include "iscriptserver.h"

#include "jluascript.h"
#include "jluathread.h"
#include "jluaserver.h"

//****************************************************************************/
/*  JLuaServer implementation
//****************************************************************************/
decl_class(JLuaServer)
JLuaServer* JLuaServer::s_pInstance = NULL;

JLuaServer::JLuaServer()
{
    m_Threads.reserve_ids( 1 );
    m_AllocMemSize = 0;
    m_pLua = NULL;

    SetName( "lua" );

    s_pInstance = this;
    g_pScriptServer = this;
} // JLuaServer::JLuaServer

JLuaServer::~JLuaServer()
{
    if (s_pInstance == this)
    {
        if (m_pLua)
        {
            lua_close( m_pLua );
        }
        g_pScriptServer = NULL;
        s_pInstance = NULL;
    }
} // JLuaServer::~JLuaServer

void JLuaServer::Init()
{
    //  create lua state instance
    m_pLua = lua_newstate( (lua_Alloc)JLuaServer::MemAlloc, NULL );
    if (m_pLua == NULL)
    {
        rlog.err( "Could not create Lua state instance." );
        return;
    }
    m_pLua->userdata = NULL;
    
    //  lua panic handler
    lua_atpanic( m_pLua, JLuaServer::LuaPanic );

    //  open standard libraries
    luaL_openlibs( m_pLua );
    
    //  register global functions to be accesible from the lua script
    RegGlobalFunc( FnCreateCB,      "create"         );
    RegGlobalFunc( FnDeleteCB,      "delete"         );
    RegGlobalFunc( FnObjCB,         "obj"            );
    RegGlobalFunc( FnCallCB,        "call"           );
    RegGlobalFunc( FnGetCB,         "get"            );
    RegGlobalFunc( FnSetCB,         "set"            );
    RegGlobalFunc( FnChildrenCB,    "children"       );
    RegGlobalFunc( FnParentCB,      "parent"         );
    RegGlobalFunc( FnSetParentCB,   "setparent"      );
    RegGlobalFunc( FnLogCB,         "log"            );
    RegGlobalFunc( FnWaitCB,        "wait"           );
    RegGlobalFunc( FnForkCB,        "fork"           );
    RegGlobalFunc( FnConnectCB,     "connect"        );
    RegGlobalFunc( FnRequireCB,     "require"        );
    RegGlobalFunc( FnDumpStackCB,   "dumpstack"      );

    //  clear stack
    lua_settop( m_pLua, 0 );
} // JLuaServer::Init

int JLuaServer::LuaPanic( lua_State* pLua )
{
    rlog.err( "Lua interpreter raised exception: %s", lua_tostring( pLua, -1 ) );
    lua_settop( pLua, 0 );
    return 0;
} // JLuaServer::LuaPanic

void* JLuaServer::MemAlloc( void* ud, void* ptr, size_t osize, size_t nsize ) 
{
    if (nsize == 0) 
    {
        free( ptr );   
        JLuaServer::s_pInstance->m_AllocMemSize -= osize;
        return NULL;
    }
    else
    {
        JLuaServer::s_pInstance->m_AllocMemSize += (nsize - osize);
        return realloc( ptr, nsize );
    }
} // JLuaServer::Alloc

void JLuaServer::RegGlobalFunc( lua_CFunction func, const char* name )
{
    lua_pushstring          ( m_pLua, name );
    lua_pushlightuserdata   ( m_pLua, this );
    lua_pushcclosure        ( m_pLua, func, 1 );
    lua_settable            ( m_pLua, LUA_GLOBALSINDEX );
    lua_settop              ( m_pLua, 0 );
} // JLuaServer::RegGlobalFunc

int JLuaServer::GetScriptID( const char* name )
{
    //  search in existing scripts
    JScriptReg::iterator it = m_Scripts.begin();
    while (it != m_Scripts.end())
    {
        JLuaScript* pScript = *it;
        if (!stricmp( name, pScript->m_Name.c_str() ))
        {
            return it.id();
        }
        ++it;
    }
    
    int id = m_Scripts.alloc();
    JLuaScript& script = *(m_Scripts[id]);
    script.m_Name = name;
    script.Load();

    return id;
} // JLuaServer::GetScriptID

bool JLuaServer::DeleteScript( int scriptID )
{
    return m_Scripts.remove( scriptID );
} // JLuaServer::DeleteScript

bool JLuaServer::ReloadScript( int scriptID )
{
    JLuaScript* pScript = m_Scripts[scriptID];
    if (!pScript) return false;
    return pScript->Load();
} // JLuaServer::ReloadScript

void JLuaServer::ReloadScripts()
{
    JScriptReg::iterator it = m_Scripts.begin();
    while (it != m_Scripts.end())
    {
        (*it)->Load();
        ++it;
    }
} // JLuaServer::ReloadScripts

bool JLuaServer::UpdateThread( int threadID, float dTime )
{
    if (dTime < 0.0f) return false;
    JLuaThread* pThread = m_Threads[threadID];
    if (!pThread) return false;

    //  update all children threads
    JLuaThread* pChild = pThread->m_pChild;
    while (pChild)
    {
        UpdateThread( pChild->m_ID, dTime );
        pChild = pChild->m_pSibling;
    }

    double dt = (double)dTime;
    bool bRes = pThread->Update( dt );
    if (!bRes)
    {
        //  thread has finished
        DestroyThread( threadID );
    }
    return bRes;
} // JLuaServer::UpdateThread

int JLuaServer::GetNThreads( int threadID )
{
    JLuaThread* pThread = m_Threads[threadID];
    if (!pThread) return 0;
    int nCh = 0;
    JLuaThread* pChild = pThread->m_pChild;
    while (pChild)
    {
        nCh++;
        pChild = pChild->m_pSibling;
    }
    return nCh;
} // JLuaServer::GetNThreads

bool JLuaServer::StopThread( int threadID )
{
    JLuaThread* pThread = m_Threads[threadID];
    if (!pThread) return false;
    pThread->Stop( m_pLua );
    DestroyThread( threadID );
    return true;
} // JLuaServer::StopThread

void JLuaServer::ClearThreads()
{
    JThreadReg::iterator it = m_Threads.begin();
    while (it != m_Threads.end())
    {
        (*it)->Stop( m_pLua );
        ++it;
    }
    m_Threads.clear();
} // JLuaServer::ClearThreads

void JLuaServer::ClearScripts()
{
    JScriptReg::iterator it = m_Scripts.begin();
    while (it != m_Scripts.end())
    {
        (*it)->Release();
        ++it;
    }
    m_Scripts.clear();
} // JLuaServer::ClearScripts

void JLuaServer::DestroyThread( int threadID )
{
    JLuaThread* pThread = m_Threads[threadID];
    if (!pThread) return;
    
    //  stop all children threads
    while (pThread->m_pChild)
    {
        StopThread( pThread->m_pChild->m_ID );
    }

    if (pThread->m_pParent)
    {
        pThread->m_pParent->RemoveChild( pThread );
    }

    m_Threads.remove( threadID );
    //  calling explicitly Destroy here, because thread object constructor is not called, due 
    //  to being allocated from the pool
    pThread->Destroy();
} // JLuaServer::DestroyThread

bool JLuaServer::RunFunction( int threadID, const char* funcName, JObject* pArgument )
{
    JLuaThread* pThread = m_Threads[threadID];
    if (!pThread)
    {
        return false;
    }

    JLuaThread* pChildThread = SpawnChildThread( pThread, funcName, pArgument );
    if (!pChildThread)
    {
        return false;
    }
    return true;
} // JLuaServer::RunFunction

JLuaThread* JLuaServer::SpawnChildThread( JLuaThread* pParent, const char* funcName, JObject* pArgument )
{
    if (!m_pLua) return NULL;
    //  create new thread instance and start the thread
    int threadID = m_Threads.alloc();
    JLuaThread* pThread  = m_Threads[threadID];
    pThread->m_ID        = threadID;
    JString res;

    pParent->AddChild( pThread );

    int status = pThread->Start( m_pLua, funcName, pParent, pArgument, &res );

    if (status != 0 && status != LUA_YIELD) 
    {
        DestroyThread( threadID );
        return NULL;
    }

    if (status == 0)
    {
        //  script has finished to execute
        DestroyThread( threadID );
        threadID = 0;
        return NULL;
    }
    return pThread;
} // JLuaServer::SpawnChildThread

int JLuaServer::RunString( const char* str, JObject* pRoot, JString* pResult )
{
    if (!m_pLua) return -1;
    //  create new thread instance and start the thread
    int threadID = m_Threads.alloc();
    JLuaThread* pThread  = m_Threads[threadID];
    pThread->m_ID        = threadID;
    int status = pThread->Start( m_pLua, str, pRoot, pResult );

    if (status != 0 && status != LUA_YIELD) 
    {
        DestroyThread( threadID );
        return -1;
    }

    if (status == 0)
    {
        //  script has finished to execute
        DestroyThread( threadID );
        threadID = 0;
    }
    return threadID;
} // JLuaServer::RunString

int JLuaServer::RunScript( int scriptID, JObject* pRoot, JString* pResult )
{
    if (scriptID < 0 || scriptID >= m_Scripts.size())
    {
        return -1;
    }
    
    JLuaScript* pScript = m_Scripts[scriptID];

    if (!pScript)
    {
        rlog.err( "LUA: Trying to run non-existing script id:%d", scriptID );
        return -1;
    }

    if (pScript->m_ChunkID == -1)
    {
        return -1; 
    }
    
    //  create new thread instance and start the thread
    int threadID = m_Threads.alloc();
    JLuaThread* pThread  = m_Threads[threadID];
    pThread->m_ID        = threadID;
    pThread->SetName( pScript->GetName() );
    int status = pThread->Start( m_pLua, pScript, pRoot, pResult );

    if (status != 0 && status != LUA_YIELD) 
    {
        DestroyThread( threadID );
        return -1;
    }

    if (status == 0)
    {
        //  script has finished to execute
        DestroyThread( threadID );
        threadID = 0;
    }
    return threadID;
} // JLuaServer::RunScript

bool JLuaServer::GetStack( lua_State* pLua, int stackStart, JString* pRes )
{
    if (!pRes || stackStart <= 0) return false;
    JString& res = *pRes;

    while (stackStart <= lua_gettop( pLua ))
    {
        switch (lua_type( pLua, -1 ))
        {
            case LUA_TBOOLEAN:
            {
                if (lua_toboolean( pLua, -1 )) 
                {
                    res += "true";
                }
                else 
                {
                    res += "false";
                }
                break;  
            }
            case LUA_TNUMBER:
            case LUA_TSTRING:
            {
                res += lua_tostring( pLua, -1 );
                break;
            }
            case LUA_TUSERDATA:
            case LUA_TLIGHTUSERDATA:
            {
                res += "<userdata>";
                break;
            }
            case LUA_TNIL:
            {
                res += "<nil>";
                break;
            }
            case LUA_TTABLE:
            {
                res += "{ ";
                lua_pushnil( pLua );
                lua_gettable( pLua, -2 );
                bool firstItem = true;
                while (lua_next( pLua, -2 ) != 0)
                {
                    if (!firstItem)
                    {
                        res += ", ";
                    }
                    else
                    {
                        firstItem = false;
                    }
                    GetStack( pLua, lua_gettop( pLua ) - 1, pRes );
                }
                lua_pop( pLua, 1 );
                res += " }";
                break;
            }
            default:
                break;  
        }
        lua_pop( pLua, -1 );
    }
    return true;
} // JLuaServer::GetStack

    