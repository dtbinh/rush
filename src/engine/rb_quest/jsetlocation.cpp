/***********************************************************************************/
//  File:   JSetLocation.cpp
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "float.h"
#include "Logger.h"
#include "JObjectIterator.h"
#include "JAnimation.h"
#include "JLocation.h"
#include "JAreaControl.h"
#include "JQuestGame.h"
#include "IWindowServer.h"
#include "JSetLocation.h"

const Vec3 c_NoCharPos( -FLT_MAX, -FLT_MAX, -FLT_MAX );
const Vec3 c_AutoCharPos( -1.0f, -1.0f, -1.0f );
/***********************************************************************************/
/*  JSetLocation implementation
/***********************************************************************************/
decl_class(JSetLocation);
JSetLocation::JSetLocation()
{
   m_CharDir    = 0.0f;
   m_CharPos    = c_NoCharPos;

   m_pDelegate  = NULL;
   m_Delegate   = "SetLocationDelegate";
} // JSetLocation::JSetLocation

void JSetLocation::OnPlay()
{
    if (m_pDelegate == NULL || m_pDelegate == this)
    {
        JLocation* pLoc = NULL;
        if (m_Location.size() > 0)
        {
            g_pQuestGame->SetCurLocation( NULL );
            //  HACK: draw black screen-sized rectangle to prevent location blinking
            Frame ext;
            g_pWindowServer->GetRootExt( ext );
            g_pDrawServer->Flush();
            g_pDrawServer->DrawRect( ext, 0xFF000000 ); 
            g_pDrawServer->Flush();

            pLoc = g_pObjectServer->FindObject<JLocation>( m_Location.c_str(), g_pQuestGame );
            if (!pLoc)
            {
                rlog.err( "Could not set current location: %s. Please, ensure, that it is in location list.", 
                    m_Location.c_str() );
                return;
            }
        }

        if (m_CharPos.is_equal( c_AutoCharPos ))
        {
            //  automatically pick the position
            JObjectIterator it( pLoc );
            while (it)
            {
                JAreaControl* pAreaC = obj_cast<JAreaControl>( *it );
                ++it;
                if (!pAreaC) continue;
                PolyLine2 area = pAreaC->GetArea();
                Vec2 pos = area.GetCenter();
                if (!area.PtIn( pos ))
                {
                    pos = area.GetPoint( rand()%area.GetNPoints() );
                }
                g_pQuestGame->SetCharPos( Vec3( pos.x, pos.y, 0.0f ), m_CharDir );
            }
        }
        else if (!m_CharPos.is_equal( c_NoCharPos ))
        {
            g_pQuestGame->SetCharPos( m_CharPos, m_CharDir );
        }

        if (m_Location.size() > 0)
        {
            g_pQuestGame->SetCurLocation( m_Location.c_str() );
        }
        g_pQuestGame->SetCursor     ( "default" );
        SetVisible( false );
    }
    else
    {
        m_pDelegate->m_CharDir = m_CharDir;
        m_pDelegate->m_CharPos = m_CharPos;
        m_pDelegate->m_Location = m_Location;
        m_pDelegate->SetVisible( true );
    }

    Stop();
} // JSetLocation::OnPlay

void JSetLocation::Init()
{
    if (!stricmp( GetName(), m_Delegate.c_str() ))
    {
        m_pDelegate = NULL;
    }
    else
    {
        m_pDelegate = g_pObjectServer->FindObject<JSetLocation>( m_Delegate.c_str() );
    }
}
