//****************************************************************************/
//  File:   JPopupPanel.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JPOPUPPANEL_H__
#define __JPOPUPPANEL_H__

enum JPopupState
{
    PopupState_Popped   = 0,
    PopupState_Hidden   = 1,
    PopupState_Popping  = 2,
    PopupState_Hiding   = 3,
}; // enum JPopupState

enum_beg(JPopupState)
    enum_val( "Popped",     PopupState_Popped  ),
    enum_val( "Hidden",     PopupState_Hidden  ),
    enum_val( "Popping",    PopupState_Popping ),
    enum_val( "Hiding",     PopupState_Hiding  )
enum_end(JPopupState)

//****************************************************************************/
//  Class:  JPopupPanel
//  Desc:   Panel, which animates to pop up and down
//****************************************************************************/
class JPopupPanel : public JPanel
{
    float           m_PopTime;
    Vec2            m_PopPos;
    float           m_HideTime;
    Vec2            m_HidePos;

    float           m_Timeout;
    Frame            m_Hotspot;

    Timer           m_Timer;
    JPopupState     m_State;

public: 
                    JPopupPanel     ();
    virtual void    Render          (); 
    virtual void    OnMouse         ( JMouseEvent& e );

    void            Pop             ();
    void            Hide            ();
    void            TogglePop       ();

    JPopupState     GetState        () const { return m_State; }
    void            SetState        ( JPopupState state );

    expose( JPopupPanel )
    {
        parent(JPanel);
        field ( "PopTime",      m_PopTime   );
        field ( "HideTime",     m_HideTime  );
        field ( "HidePos",      m_HidePos   );
        field ( "PopPos",       m_PopPos    );
        field ( "Timeout",      m_Timeout   );
        field ( "Hotspot",      m_Hotspot   );
        prop  ( "State",        GetState, SetState );
        method( "Pop",          Pop         );
        method( "Hide",         Hide        );
        method( "TogglePop",    TogglePop   );
    }
}; // class JPopupPanel

#endif // __JPOPUPPANEL_H__


