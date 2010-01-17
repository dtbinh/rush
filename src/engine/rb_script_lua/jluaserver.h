//****************************************************************************/
//  File:  JLuaServer.h
//****************************************************************************/
#ifndef __JLUASERVER_H__
#define __JLUASERVER_H__

#include "jluathread.h"
#include "jluascript.h"

typedef RegPool<JLuaScript>  JScriptReg;
typedef RegPool<JLuaThread>  JThreadReg;
//****************************************************************************/
//  Class:  JLuaServer
//  Desc:
//****************************************************************************/
class JLuaServer : public JObject, public IScriptServer
{
    lua_State*      m_pLua;
    JScriptReg      m_Scripts;
    JThreadReg      m_Threads;
    int             m_AllocMemSize;

    friend class    JLuaScript;
    friend class    JLuaThread;

public:
                    JLuaServer      ();
                    ~JLuaServer     ();

    virtual void    Init            ();

    virtual int     GetScriptID     ( const char* name );
    virtual bool    DeleteScript    ( int scriptID );
    virtual int     RunScript       ( int scriptID, JObject* pRoot = NULL, JString* pResult = NULL );
    virtual bool    RunFunction     ( int threadID, const char* funcName, JObject* pArgument = NULL );
    virtual void    ReloadScripts   ();
    virtual bool    ReloadScript    ( int scriptID );

    virtual int     GetNScripts     () const { return m_Scripts.size(); }
    virtual int     GetNThreads     () const { return m_Threads.size(); }
    virtual int     GetNThreads     ( int threadID );
    virtual bool    UpdateThread    ( int threadID, float dTime );
    virtual bool    StopThread      ( int threadID );
    virtual void    ClearThreads    ();
    virtual void    ClearScripts    ();
    virtual int     RunString       ( const char* str, JObject* pRoot = NULL, JString* pResult = NULL );
    virtual int     GetMemUsage     () const { return m_AllocMemSize; }

    bool            GetStack        ( lua_State* pLua, int stackStart, JString* pRes );

    lua_State*      GetLuaState     () const { return m_pLua; }

    static JLuaServer*  s_pInstance;

    expose(JLuaServer)
    {
        parent(JObject);
        rprop( "AllocBytes", GetMemUsage );
        rprop( "NumScripts", GetNScripts );
        rprop( "NumThreads", GetNThreads );
        method( "ReloadScripts", ReloadScripts );
    }

protected:
    void            RegGlobalFunc   ( lua_CFunction func, const char* name );
    void            DestroyThread   ( int threadID );
    JLuaThread*     SpawnChildThread( JLuaThread* pParent, const char* funcName, JObject* pArgument = NULL );


    static void*    MemAlloc        ( void* ud, void* ptr, size_t osize, size_t nsize );
    static int      LuaPanic        ( lua_State* pLua );

    //  static "global" function callbacks to serve as lua invokers
    static int      FnCreateCB      ( lua_State* pLua ) { return s_pInstance->FnCreate( pLua ); }
    static int      FnDeleteCB      ( lua_State* pLua ) { return s_pInstance->FnDelete( pLua ); }
    static int      FnObjCB         ( lua_State* pLua ) { return s_pInstance->FnObj( pLua ); }
    static int      FnGetCB         ( lua_State* pLua ) { return s_pInstance->FnGet( pLua ); }
    static int      FnSetCB         ( lua_State* pLua ) { return s_pInstance->FnSet( pLua ); }
    static int      FnCallCB        ( lua_State* pLua ) { return s_pInstance->FnCall( pLua ); }
    static int      FnChildrenCB    ( lua_State* pLua ) { return s_pInstance->FnChildren( pLua ); }
    static int      FnParentCB      ( lua_State* pLua ) { return s_pInstance->FnParent( pLua ); }
    static int      FnSetParentCB   ( lua_State* pLua ) { return s_pInstance->FnSetParent( pLua ); }
    static int      FnLogCB         ( lua_State* pLua ) { return s_pInstance->FnLog( pLua ); }
    static int      FnWaitCB        ( lua_State* pLua ) { return s_pInstance->FnWait( pLua ); }
    static int      FnForkCB        ( lua_State* pLua ) { return s_pInstance->FnFork( pLua ); }
    static int      FnConnectCB     ( lua_State* pLua ) { return s_pInstance->FnConnect( pLua ); }
    static int      FnRequireCB     ( lua_State* pLua ) { return s_pInstance->FnRequire( pLua ); }
    static int      FnDumpStackCB   ( lua_State* pLua ) { return s_pInstance->FnDumpStack( pLua ); }

    //  implementations of the "global" functions
    int             FnCreate        ( lua_State* pLua );
    int             FnDelete        ( lua_State* pLua );
    int             FnObj           ( lua_State* pLua );
    int             FnGet           ( lua_State* pLua );
    int             FnSet           ( lua_State* pLua );
    int             FnCall          ( lua_State* pLua );
    int             FnChildren      ( lua_State* pLua );
    int             FnParent        ( lua_State* pLua );
    int             FnSetParent     ( lua_State* pLua );
    int             FnLog           ( lua_State* pLua );
    int             FnWait          ( lua_State* pLua );
    int             FnFork          ( lua_State* pLua );
    int             FnConnect       ( lua_State* pLua );
    int             FnRequire       ( lua_State* pLua );
    int             FnDumpStack     ( lua_State* pLua );


}; // class JLuaServer

#endif // __JLUASERVER_H__


