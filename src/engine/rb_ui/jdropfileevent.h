//****************************************************************************/
//  File:   JDropFileEvent.h
//  Date:   26.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JDROPFILEEVENT_H__
#define __JDROPFILEEVENT_H__
#include "jevent.h"

//****************************************************************************/
//  Class:  JDropFileEvent
//  Desc:   Describes user keyboard event
//****************************************************************************/
class JDropFileEvent : public JEventID<JDropFileEvent>
{
public:
    JDropFileEvent( const char* path = "", const Vec2& pos = Vec2::null ) :
        m_Path( path ),
        m_Pos ( pos )
    {}

    const char* GetPath() const { return m_Path.c_str(); }
    const Vec2& GetPos () const { return m_Pos; }

private:
    JString         m_Path;
    Vec2            m_Pos;
}; // class JDropFileEvent

#endif // __JDROPFILEEVENT_H__


