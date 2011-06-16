//****************************************************************************/
//  File:   JMouseEvent.h
//  Date:   26.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JMOUSEEVENT_H__
#define __JMOUSEEVENT_H__
#include "jevent.h"
#include "jinputaction.h"

//****************************************************************************/
//  Enum:  JMouseKey
//  Desc:
//****************************************************************************/
enum JMouseKey
{
    mkNone   = 0x0,
    mkLeft   = 0x1,
    mkMiddle = 0x2,
    mkRight  = 0x4
}; // enum JMouseKey

//****************************************************************************/
//  Enum:  JMouseEventType
//  Desc:
//****************************************************************************/
enum JMouseEventType
{
    meNone   = 0,
    meLeft   = 1,
    meMiddle = 2,
    meRight  = 3
}; // enum JMouseEventType

//****************************************************************************/
//  Class:  JMouseEvent
//  Desc:
//****************************************************************************/
class JMouseEvent : public JEventID<JMouseEvent>
{
    JMouseKey        m_MouseKey;
    JInputAction     m_InputAction;
    int              m_MouseX, m_MouseY;

    friend class     JWindowServer;
public:

    JMouseKey        MouseKey   () const { return m_MouseKey; }
    JInputAction     Action     () const { return m_InputAction; }
    int              MouseX     () const { return m_MouseX; }
    int              MouseY     () const { return m_MouseY; }

}; // class JMouseEvent

#endif // __JMOUSEEVENT_H__


