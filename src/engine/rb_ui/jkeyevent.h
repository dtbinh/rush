//****************************************************************************/
//  File:   JKeyEvent.h
//  Date:   26.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JKEYEVENT_H__
#define __JKEYEVENT_H__
#include "jevent.h"
#include "jinputaction.h"

//****************************************************************************/
//  Class:  JKeyEvent
//  Desc:   Describes user keyboard event
//****************************************************************************/
class JKeyEvent : public JEventID<JMouseEvent>
{
    JInputAction    m_InputAction;
    uint32_t           m_Char;
    uint32_t           m_Key;

    friend class    JWindowServer;

public:
    uint32_t           GetChar     () const { return m_Char; }
    uint32_t           Key         () const { return m_Key; }
    JInputAction    Action      () const { return m_InputAction; }

}; // class JKeyEvent

#endif // __JKEYEVENT_H__


