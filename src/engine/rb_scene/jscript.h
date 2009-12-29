//****************************************************************************/
//  File:   JScript.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JSCRIPT_H__
#define __JSCRIPT_H__
#include "JAnimation.h"

//****************************************************************************/
//  Class:  JScript
//  Desc:   Scripted animation
//****************************************************************************/
class JScript : public JAnimation
{
    JString         m_Script;
    int             m_ScriptID;
    int             m_ThreadID;

    JString         m_Result;

    JStringList     m_DispatchTable;

public: 
                    JScript    ();
    virtual         ~JScript   ();
    virtual void    OnPlay     (); 
    virtual void    OnStop     (); 
    virtual void    Init       (); 
    virtual void    Render     (); 

    void            Reload     ();
    void            Dispatch   ( int methodID );
    void            Dispatch   ( const char* event );
    bool            OnSignal   ( JObject* pSrc, const char* pSrcAttr, const char* pDstAttr );

    expose( JScript )
    {
        parent(JAnimation);
        field( "Script",        m_Script        );
        field( "Result",        m_Result        );
        field( "Dispatchtable", m_DispatchTable );
        method( "Reload",       Reload          );
        method( "Dispatch",     Dispatch        );
    }

private:
    void            StopThread  ();
}; // class JScript

#endif // __JSCRIPT_H__


