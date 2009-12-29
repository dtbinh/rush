//****************************************************************************/
//  File:  JLuaThread.h
//****************************************************************************/
#ifndef __JLUATHREAD_H__
#define __JLUATHREAD_H__

#include "JWeakRef.h"

struct JThreadSignal
{
    JWeakRef<JObject>   m_pObj;

    JString             m_Attr;
    JString             m_Func;
    JThreadSignal() 
        : m_pObj( NULL )
    {}
    JThreadSignal( JObject* pObj, const char* attr, const char* pFunc ) 
        : m_pObj( pObj ),
          m_Attr( attr ),
          m_Func( pFunc )
    {}

    bool  IsEqual( JObject* pObj, const char* attr )
    {
        if (pObj != m_pObj) return false;
        return (stricmp( attr, m_Attr.c_str() ) == 0);
    }
}; // struct JThreadSignal

//****************************************************************************/
//  Class:  JLuaThread
//  Desc:    
//****************************************************************************/
class JLuaThread : public JObject 
{
public:
    JLuaThread() 
        : m_pLua        ( NULL  ),
          m_WaitTime    ( 0.0f  ),
          m_pRootObj    ( NULL  ),
          m_AnchorID    ( -1    ),
          m_pScript     ( NULL  ),
          m_pParent     ( NULL  ),
          m_pChild      ( NULL  ),
          m_pSibling    ( NULL  ),
          m_ID          ( -1    )
    {}

    int         Start           ( lua_State* pLua, JLuaScript* pScript, 
                                    JObject* pRootObj = NULL, JString* pResult = NULL );
    int         Start           ( lua_State* pLua, const char* pScript, 
                                    JObject* pRootObj = NULL, JString* pResult = NULL );
    int         Start           ( lua_State* pLua, const char* funcName, JLuaThread* pParent = NULL, 
                                    JObject* pArgument = NULL, JString* pResult = NULL );

    int         Resume          ();
    void        Stop            ( lua_State* pLua );
    bool        Update          ( double dTime );
    bool        Connect         ( JObject* pSrc, const char* pSrcProp, const char* pFunc );
    lua_State*  GetLuaState     () const { return m_pLua; }
    const char* GetScriptName   () const;

    bool        AddChild        ( JLuaThread* pChild );
    bool        RemoveChild     ( JLuaThread* pChild );

    void        Input           ();

    expose( JLuaThread )
    {
        parent(JObject);
        method( "input", Input );
    }

private:
    lua_State*                  m_pLua;
    double                      m_WaitTime;
    JObject*                    m_pRootObj;
    int                         m_AnchorID;
    JLuaScript*                 m_pScript;
    JLuaThread*                 m_pParent;

    JLuaThread*                 m_pChild;   // first child
    JLuaThread*                 m_pSibling; // next sibling

    int                         m_ID;

    std::vector<JThreadSignal>  m_Signals;

    friend class                JLuaScript;
    friend class                JLuaServer;

}; // class JLuaThread

#endif // __JLUATHREAD_H__


