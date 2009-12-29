/***********************************************************************************/
//  File:   JMoveChar.cpp
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JCharacter.h"
#include "JCharControl.h"
#include "JAnimation.h"
#include "JMoveChar.h"

/***********************************************************************************/
/*  JMoveChar implementation
/***********************************************************************************/
decl_class(JMoveChar);
JMoveChar::JMoveChar()
{
    m_bImmediately = true;
    m_bTeleport    = false; 
    m_Pos          = Vec3::null;
    m_Dir          = -1.0f;
    m_pCharControl = NULL;
    m_CharControl  = "*";
}

void JMoveChar::OnPlay()
{
    if (!m_pCharControl) 
    {
        Stop();
        return; 
    }
    
    Vec3 pos( m_Pos.x, m_Pos.y, 0.0f );
    if (m_bTeleport)
    {
        JCharacter* pChar = m_pCharControl->GetChar();
        if (pChar)
        {
            pChar->SetPos( pos );
            if (m_Dir != -1.0f) pChar->SetDir( m_Dir );
        }
    }
    else
    {
        m_pCharControl->MoveTo( pos, m_bImmediately );    
    }
} // JMoveChar::OnPlay

void JMoveChar::Render()
{
    if (!m_pCharControl)
    {
        return;
    }
    JCharacter* pChar = m_pCharControl->GetChar();
    if (pChar)
    {
        Vec3 pos = pChar->GetPos();
        const float c_DistThreshold = 1.0f;
        if (pos.dist( m_Pos ) < c_DistThreshold)
        {
            Stop();
        }
    }
}

void JMoveChar::OnStop()
{
    if (!m_pCharControl)
    {
        return;
    }
    JCharacter* pChar = m_pCharControl->GetChar();
    if (pChar)
    {
        if (m_Dir != -1.0f) 
        {
            Vec3 pos( m_Pos.x, m_Pos.y, 0.0f );
            m_pCharControl->SetCharPos( pos, m_Dir );
        }
    }
} // JMoveChar::OnStop

void JMoveChar::Init()
{
    m_pCharControl = g_pObjectServer->FindObject<JCharControl>( m_CharControl.c_str(), this );
} // JMoveChar::Init