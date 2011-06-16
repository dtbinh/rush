/*****************************************************************************/
/*  File:  IScriptServer.h
/*****************************************************************************/
#ifndef __ISCRIPTSERVER_H__
#define __ISCRIPTSERVER_H__

class JObject;
/*****************************************************************************/
/*  Class:  IScriptServer, abstract interface
/*  Desc:   
/*****************************************************************************/
class IScriptServer
{
public:
    //  gets/loads script by name, returns its session-unique id
    virtual int     GetScriptID     ( const char* name ) = 0;

    //  deletes script by id
    virtual bool    DeleteScript    ( int scriptID ) = 0;

    //  runs script by id, returns id of the thread created, -1 if no thread
    virtual int     RunScript       ( int scriptID, JObject* pRoot = NULL, JString* pResult = NULL ) = 0;
    
    //  reloads script with given id
    virtual bool    ReloadScript    ( int scriptID ) = 0;

    //  reloads all scripts in the system
    virtual void    ReloadScripts   () = 0;

    //  returns number of the loaded scripts
    virtual int     GetNScripts     () const = 0;

    //  returns number of the currently running threads
    virtual int     GetNThreads     () const = 0;

    //  returns number of the currently running threads, whose parent is the thread with threadID
    virtual int     GetNThreads     ( int threadID ) = 0;

    //  updates thread state
    virtual bool    UpdateThread    ( int threadID, float dTime ) = 0;

    //  forces thread shut down
    virtual bool    StopThread      ( int threadID ) = 0;

    //  stops all currently running threads
    virtual void    ClearThreads    () = 0;

    //  deletes all currently loaded scripts
    virtual void    ClearScripts    () = 0;

    //  runs script provided by command line
    virtual int     RunString       ( const char* str, JObject* pRoot = NULL, JString* pResult = NULL ) = 0;

    //  runs function by name, inside some running thread execution area
    virtual bool    RunFunction     ( int threadID, const char* funcName, JObject* pSource = NULL ) = 0;
    
    //  returns amount of memory allocated by script VM, in bytes
    virtual int     GetMemUsage     () const = 0;

}; // class IScriptServer

extern IScriptServer* g_pScriptServer;

#endif // __ISCRIPTSERVER_H__


