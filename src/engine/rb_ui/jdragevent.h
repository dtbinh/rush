//****************************************************************************/
//  File:   JDragEvent.h
//  Date:   26.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JDRAGEVENT_H__
#define __JDRAGEVENT_H__
#include "JEvent.h"
#include "Vec2.h"

class JWidget;
class JObject;

enum JDragEventType
{
    deNone          = 0,
    deDrag          = 1,    //  objects is being dragged
    deDragStart     = 2,    //  object started to be dragged
    deDrop          = 3,    //  object was dropped
}; // enum JDragEventType

//****************************************************************************/
//  Class:  JDragEvent
//  Desc:   User drag-n-drop object even
//****************************************************************************/
class JDragEvent : public JEventID<JDragEvent>
{
    Vec2            m_StartPos;         //  starting drag position
    Vec2            m_HotSpot;          //  starting drag position inside dragged widget

    Vec2            m_CurPos;           //  current drag position
    Vec2            m_PrevPos;          //  previous drag position
    JMouseKey       m_Key;
    JDragEventType  m_Type;
    bool            m_bSticky;

    JWidget*        m_pSource;          //  drag source widget
    JWidget*        m_pTarget;          //  drop target widget
    JWidget*        m_pObject;          //  dragged widget

    int             m_Tag;              //  user-specific tag

    friend class    JWindowServer;
    friend class    JWidget;
public:
    JDragEvent() { Reset(); }

    void            Reset          () 
    {
        m_pSource = NULL;
        m_pTarget = NULL;
        m_pObject = NULL;
        m_Key     = mkNone;
        m_Type    = deNone;
        m_bSticky = false;
        m_Tag     = -1;
    }

    JWidget*        GetTarget     () const { return m_pTarget; }
    JWidget*        GetSource     () const { return m_pSource; }
    JWidget*        GetDragObject () const { return m_pObject; }
    void            SetDragObject ( JWidget* pObject ) { m_pObject = pObject; }
    void            SetSource     ( JWidget* pObject ) { m_pSource = pObject; }
    void            SetSticky     ( bool bSticky ) { m_bSticky = bSticky; }
    Vec2            GetDelta      () const { return m_CurPos - m_PrevPos; }
    const Vec2&     GetCurPos     () const { return m_CurPos; }
    const Vec2&     GetStartPos   () const { return m_StartPos; }
    const Vec2&     GetHotSpot    () const { return m_HotSpot;  }
    JDragEventType  GetType       () const { return m_Type; }
    JMouseKey       Key           () const { return m_Key; }
    void            SetCurPos     ( const Vec2& pos ) { m_PrevPos = m_CurPos; m_CurPos = pos; }
    void            SetStartPos   ( const Vec2& pos ) { m_PrevPos = m_CurPos = m_StartPos = pos; }

    int             GetTag        () const { return m_Tag; }
    void            SetTag        ( int tag ) { m_Tag = tag; }

}; // class JDragEvent

#endif // __JDRAGEVENT_H__


