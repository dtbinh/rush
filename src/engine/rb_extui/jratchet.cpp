//****************************************************************************/
//  File:   JRatchet.cpp
//  Date:   24.10.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "JRatchet.h"

//****************************************************************************/
/*  JRatchet implementation
//****************************************************************************/
decl_class(JRatchet);
JRatchet::JRatchet()
{
    m_GizmoSprite       = "JRatchet";   
    m_GizmoFrameRate    = 1.0f;

    m_DefaultValue      = 0.0f;      
    m_ValueStep         = 1.0f;          
    m_Value             = 0.0f;   
    m_bVertical         = false;
    m_Color             = 0xFFFFFFFF;

    m_SprID             = -1;
    m_CurFrame          = 0;
} // JRatchet::JRatchet

void JRatchet::Render()
{    
    //Frame ext = GetScreenExt();
    //g_pDrawServer->DrawSprite( ext, m_SprID, m_CurFrame, m_Color );
} // JRatchet::Render
