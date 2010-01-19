//****************************************************************************/
//  File:   JScriptEvent.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JSCRIPTEVENT_H__
#define __JSCRIPTEVENT_H__
#include "janimation.h"
#include "jweakref.h"

//****************************************************************************/
//  Class:  JScriptEvent
//  Desc:   Fires named event in running script
//****************************************************************************/
class JScriptEvent : public JAnimation
{
    JString             m_Script;
    JString             m_Event;
    JWeakRef<JScript>   m_pScript;

public:
                    JScriptEvent();
    virtual void    OnPlay      ();
    virtual void    Init        ();

    expose( JScriptEvent )
    {
        parent(JAnimation);
        field( "Script",    m_Script  );
        field( "Event",     m_Event  );
    }
}; // class JScriptEvent

#endif // __JSCRIPTEVENT_H__


