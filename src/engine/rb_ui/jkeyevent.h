//****************************************************************************/
//  File:   JKeyEvent.h
//  Date:   26.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JKEYEVENT_H__
#define __JKEYEVENT_H__
#include "JEvent.h"
#include "JInputAction.h"

//****************************************************************************/
//  Class:  JKeyEvent
//  Desc:   Describes user keyboard event
//****************************************************************************/
class JKeyEvent : public JEventID<JMouseEvent>
{
    JInputAction    m_InputAction;
    DWORD           m_Char;
    DWORD           m_Key;

    friend class    JWindowServer;

public:
    DWORD           GetChar     () const { return m_Char; }
    DWORD           Key         () const { return m_Key; }
    JInputAction    Action      () const { return m_InputAction; }

}; // class JKeyEvent

#endif // __JKEYEVENT_H__


