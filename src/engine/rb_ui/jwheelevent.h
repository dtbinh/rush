//****************************************************************************/
//  File:   JWheelEvent.h
//  Date:   26.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JWHEELEVENT_H__
#define __JWHEELEVENT_H__
#include "jevent.h"

//****************************************************************************/
//  Class:  JWheelEvent
//  Desc:   Event from mouse wheel
//****************************************************************************/
class JWheelEvent : public JEventID<JWheelEvent>
{
    int             m_Delta;

    friend class    JWindowServer;
public:
    int             Delta() const { return m_Delta; }

}; // class JWheelEvent

#endif // __JWHEELEVENT_H__


