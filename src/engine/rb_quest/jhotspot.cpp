/***********************************************************************************/
//  File:   JHotSpot.cpp
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JButton.h"
#include "JPolyButton.h"
#include "JLocation.h"
#include "JQuestGame.h"
#include "JCharacter.h"
#include "JCharControl.h"
#include "IWindowServer.h"
#include "JObjectIterator.h"
#include "JHotSpot.h"

/***********************************************************************************/
/*  JHotSpot implementation
/***********************************************************************************/
decl_class(JHotSpot);
JHotSpot::JHotSpot()
{
    m_bAttractChar  = false;
    m_bPassive      = false;
    m_Direction     = 0.0f;
} // JHotSpot::JHotSpot

void JHotSpot::Init()
{
} // JHotSpot:Init

void JHotSpot::OnHover()
{
}

void JHotSpot::OnStopHover()
{
}

JCharControl* JHotSpot::GetCharControl() const
{
    JObject* pParent = GetParent();
    while (pParent)
    {
        JCharControl* pCharControl = obj_cast<JCharControl>( pParent );
        if (pCharControl)
        {
            return pCharControl;
        }
        pParent = pParent->GetParent();
    }
    return NULL;
}

void JHotSpot::OnUnpress() 
{
    JCharControl* pControl = GetCharControl();
    if (pControl) 
    {
        //  hotspot is used as exit
        Vec2 pivot = GetPivot();
        pControl->MoveTo( Vec3( pivot.x, pivot.y, 0.0f ), true, m_Direction );
        pControl->SetExit( this );
    }
    else if (m_bAttractChar)
    {
        //  hotspot is used as non-exit active element
        //  search for character control
        JObject* pParent = GetParent();
        if (!pParent) return;
        JObjectIterator it( pParent );
        while (it)
        {
            pControl = obj_cast<JCharControl>( *it );
            if (pControl) break;
            ++it;
        }
        if (pControl)
        {
            Vec3 pos = pControl->GetChar()->GetPos();
            Vec2 c   = m_Contour.GetCenter();
            Vec3 dir = Vec3( c.x, c.y, 0.0f ) - pos;
            dir.normalize();
            const float c_AttractShift = 1.0f;
            dir *= c_AttractShift;
            pos += dir;
            pControl->MoveTo( pos, true, m_Direction );
        }
    }
} // JHotSpot::OnUnpress

void JHotSpot::DrawBounds()
{
    JPolyButton::DrawBounds();
    Frame ext = GetParentExt();
    Vec2 pivot = GetPivot();
    pivot.x += ext.x;
    pivot.y += ext.y;
    g_pDrawServer->DrawRect( Frame( pivot.x - 3, pivot.y - 3, 6, 6 ), 0xFFEEEE00 );
    g_pDrawServer->DrawFrame( Frame( pivot.x - 3, pivot.y - 3, 6, 6 ), 0xFF000000 );
} // JHotSpot::DrawBounds

bool JHotSpot::InSpot( const Vec3& pos ) const
{
    Vec2 pos2( pos.x, pos.y );
    const int c_PivotRadius = 5.0f;
    return (PtIn( pos.x, pos.y ) || pos2.dist( GetPivot() ) < c_PivotRadius);
} // JHotSpot::InSpot

void JHotSpot::Render()
{
    JCharControl* pControl = GetCharControl();
    if (pControl && pControl->GetExit() != this && !m_bPassive)
    {
        return;
    }
    if (InSpot( g_pQuestGame->GetCharPos() ))
    {
        //  check whether at least one of the parents is disabled
        JObject* pParent = this;
        while (pParent)
        {
            JWidget* pWParent = obj_cast<JWidget>( pParent );
            if (pWParent && !pWParent->IsEnabled())
            {
                return;
            }
            pParent = pParent->GetParent();
        }
        if (pControl)
        {
            pControl->SetExit( NULL );
            pControl->ClearPath();
        }
        SendSignal( "OnCharIn" );
        OnCharIn();
    }
} // JHotSpot::Render
