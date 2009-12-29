//****************************************************************************/
//  File:   JEvent.h
//  Date:   26.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JEVENT_H__
#define __JEVENT_H__

//****************************************************************************/
//  Class:  JEvent
//  Desc:   Generic user interface event
//****************************************************************************/
class JEvent 
{
public:
                JEvent  () : m_bConsumed( false ), m_ID( -1 ) {}  
    void        Consume () { m_bConsumed = true; }
    bool        Consumed() const { return m_bConsumed; }
    int         GetID   () const { return m_ID; }

protected:
    bool        m_bConsumed;    // whether event was already processed by someone
    int         m_ID;
}; // class JEvent

extern int g_NEventIDs;

template <class TEvent>
class JEventID : public JEvent
{
public:
    JEventID() { m_ID = ID; }
    
    static int  ID;

private:
    int        GetEventID() const { return ID; }

}; // class JEventID

template <class TEvent>
int JEventID<TEvent>::ID = g_NEventIDs++;

#endif // __JEVENT_H__


