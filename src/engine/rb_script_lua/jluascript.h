//****************************************************************************/
//  File:  JLuaScript.h
//****************************************************************************/
#ifndef __JLUASCRIPT_H__
#define __JLUASCRIPT_H__

//****************************************************************************/
//  Class:  JLuaScript
//  Desc:    
//****************************************************************************/
class JLuaScript : public JObject
{
    JString         m_Name;
    JString         m_Code;

    int             m_ChunkID;
    bool            m_bError;

    friend class    JLuaThread;
    friend class    JLuaServer;

public:
    JLuaScript() : m_ChunkID( -1 ), m_bError( false ) {}

    bool Load   ();
    void Release();

    expose(JLuaScript)
    {
        parent(JObject);
    }
}; // class JLuaScript

#endif // __JLUASCRIPT_H__


