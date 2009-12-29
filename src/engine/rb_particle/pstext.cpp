//****************************************************************************/
//  File:   PSText.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "ParticleServer.h"
#include "PSOperator.h"
#include "PSEmitter.h"
#include "PSText.h"

//****************************************************************************/
/*  PSText implementation
//****************************************************************************/
decl_class(PSText);
PSText::PSText()
{
    m_String        = "Particle Text";    
    m_Font          = "font_base";      
    m_Mode          = TextMode_String;      
    m_ShadowColor   = 0xFF000000;
    m_ShadowShift   = Vec2( 2.0f, 3.0f );
    m_bFilter       = true;   
    m_bStretch      = true;  
    m_bAdditive     = false;
    m_FontID        = -1;
} // PSText::PSText

void PSText::Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_Visual) == 0) return;

    Particle* p = pInst->m_pParticle;
    g_pDrawServer->SetAdditive( m_bAdditive );
    g_pDrawServer->SetLinFilter( m_bFilter );
    bool bLocalSpace = pInst->m_pEmitter->IsParentSpace();
    int nChar        = m_String.size();
    while (p)
    {
        Frame rct( p->m_Pos.x, p->m_Pos.y, 
                  p->m_Size.x, p->m_Size.y );
        if (bLocalSpace)
        {
            Vec3 pos = pInst->GetPos();
            rct.x += pos.x;
            rct.y += pos.y;
        }

        if (m_Mode == TextMode_String)
        {
            g_pDrawServer->DrawString( rct.x, rct.y, m_FontID, m_String.c_str(), p->m_Color, p->m_Size.y );
        }
        else if (m_Mode == TextMode_Character)
        {
            int charIdx = p->m_Index;
            if (charIdx < nChar)
            {
                char str[2];
                str[0] = m_String[charIdx];
                str[1] = 0;
                g_pDrawServer->DrawString( rct.x, rct.y, m_FontID, str, p->m_Color, p->m_Size.y );
            }
        }
        p = p->m_pNext;
    }

    g_pDrawServer->Flush();
} // PSText::Update

void PSText::Init()
{
    m_FontID = g_pDrawServer->GetSpriteID( m_Font.c_str(), NULL, true );
} // PSText::Init

