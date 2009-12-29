//****************************************************************************/
//  File:  JPopupPanel.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JPanel.h"
#include "Timer.h"
#include "JPopupPanel.h"

//****************************************************************************/
/*  JPopupPanel implementation
//****************************************************************************/
decl_class(JPopupPanel);
JPopupPanel::JPopupPanel()
{
    m_PopTime   = 0.5f;
    m_HideTime  = 0.5f;
    m_HidePos   = Vec2::null;
    m_PopPos    = Vec2::null;
    m_Timeout   = 5.0f;
    m_State     = PopupState_Hidden;
} 

void JPopupPanel::SetState( JPopupState state )
{
    m_State = state;
    if (m_State == PopupState_Hiding || 
        m_State == PopupState_Popping)
    {
        m_Timer.Start();
    }
    else
    {
        m_Timer.Reset();
    }
}  

void JPopupPanel::Render()
{   
    Frame ext = GetExt();
    if (m_State == PopupState_Popping)
    {
        float t  = m_Timer.Seconds()/m_PopTime; 
        if (t >= 1.0f)
        {
            m_State = PopupState_Popped;
            t = 1.0f;
        }
        Vec2 pos = m_HidePos + t*(m_PopPos - m_HidePos);
        ext.x = pos.x;
        ext.y = pos.y;
    }   
    else if (m_State == PopupState_Hiding)
    {
        float t  = m_Timer.Seconds()/m_PopTime; 
        if (t >= 1.0f)
        {
            m_State = PopupState_Hidden;
            t = 1.0f;
        }
        Vec2 pos = m_PopPos + t*(m_HidePos - m_PopPos);
        ext.x = pos.x;
        ext.y = pos.y;
    }
    else if (m_State == PopupState_Popped)
    {
        if (IsHovered())
        {
            m_Timer.Start();
        }
        else
        {
            float t  = m_Timer.Seconds()/m_Timeout; 
            if (t >= 1.0f)
            {
                m_State = PopupState_Hiding;
                m_Timer.Start();
            }
        }
        ext.x = m_PopPos.x;
        ext.y = m_PopPos.y;
    }
    else if (m_State == PopupState_Hidden)
    {
        ext.x = m_HidePos.x;
        ext.y = m_HidePos.y;
    }

    ext.x = floorf( ext.x );
    ext.y = floorf( ext.y );
    SetExt( ext );
}  

void JPopupPanel::Pop()
{
    if (m_State == PopupState_Popped) return;
    m_Timer.Start();
    m_State = PopupState_Popping;
}  

void JPopupPanel::Hide()
{
    if (m_State == PopupState_Hidden) return;
    m_Timer.Start();
    m_State = PopupState_Hiding;
} 

void JPopupPanel::TogglePop()
{
    if (m_State == PopupState_Popped || 
        m_State == PopupState_Popping) 
    {
        Hide();
    }
    else
    {
        Pop();
    }
}  

void JPopupPanel::OnMouse( JMouseEvent& e )
{ 
    if (m_State == PopupState_Hidden && e.Action() == aMouseMove)
    {
        m_State = PopupState_Popping;
        m_Timer.Start();
    }
}  

    



