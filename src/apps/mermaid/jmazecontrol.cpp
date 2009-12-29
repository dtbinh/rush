/***********************************************************************************/
/*  File:   JMazeControl.cpp
/*  Date:   29.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "JCharacter.h"
#include "JWaypoint.h"
#include "JTrack.h"
#include "JCharControl.h"
#include "JMazeControl.h"

/***********************************************************************************/
/*  JMazeControl implementation
/***********************************************************************************/
decl_class(JMazeControl);
JMazeControl::JMazeControl()
{
    m_pStart        = NULL;
    m_pEnd          = NULL;
} // JMazeControl::JMazeControl

void JMazeControl::SetStartPoint( const char* name )
{
    m_Start  = name;
    m_pStart = FindObject<JWaypoint>( m_Start.c_str() ); 
} // JMazeControl::SetStartPoint

void JMazeControl::SetEndPoint( const char* name )
{
    m_End    = name;
    m_pEnd   = FindObject<JWaypoint>( m_End.c_str() ); 
} // JMazeControl::SetEndPoint

void JMazeControl::Init()
{
    m_pStart = FindObject<JWaypoint>( m_Start.c_str() ); 
    m_pEnd   = FindObject<JWaypoint>( m_End.c_str() ); 
} // JMazeControl::Init

void JMazeControl::Start()
{
    if (!m_pStart) return;
    Vec2 pos = m_pStart->GetPos();
    SetCharPos( Vec3( pos.x, pos.y, 0.0f ), m_pStart->GetDir() );
} // JMazeControl::Start

void JMazeControl::Render()
{
    JCharControl::Render();

    if (!IsMoving())
    {
        if (GetKeyState( VK_RIGHT ) < 0) MoveTo( 0.0f   );
        if (GetKeyState( VK_DOWN  ) < 0) MoveTo( 90.0f  );
        if (GetKeyState( VK_LEFT  ) < 0) MoveTo( 180.0f );
        if (GetKeyState( VK_UP    ) < 0) MoveTo( 270.0f );
    }
    
    //  check whether we have found exit
    if (!GetChar() || !m_pEnd) return;
    Vec3 pos = GetChar()->GetPos();
    Vec2 cPos( pos.x, pos.y );
    if (cPos.dist( m_pEnd->GetPos() ) < m_pEnd->GetRadius())
    {
        OnFoundExit();
    }
} // JMazeControl::Render

void JMazeControl::OnFoundExit()
{
    SendSignal( "FoundExit" );
} // JMazeControl::OnFoundExit

bool JMazeControl::MoveTo( float dir )
{
    if (!GetChar()) return false;
    GetChar()->SetState( "idle" );
    ClearPath();
    Vec2 pos( GetChar()->GetPos().x, GetChar()->GetPos().y );
    float rDir = DegToRad( dir );
    Vec2 destDir( cosf( rDir ), sinf( rDir ) );

    //  find all relevant tracks, and pick one wich fits given direction best
    float   bestDot     = -1.0f;
    JTrack* pBestTrack  = NULL;
    int     bestPt      = -1;
    bool    bForward    = true;

    JObjectIterator it( this );
    while (it)
    {
        JTrack* pTrack = dynamic_cast<JTrack*>( *it );
        if (pTrack)
        {
            const PolyLine2& poly = pTrack->GetPath();
            int nPt = poly.GetNPoints();
            for (int i = 1; i < nPt; i++)
            {
                const Vec2& a = poly.GetPoint( i - 1 );
                const Vec2& b = poly.GetPoint( i     );
                Seg2 seg( a, b );
                const float c_TrackEpsilon = 25.0f;
                if (seg.dist2( pos ) > c_TrackEpsilon) continue;
                //  check if forward and backward directions are better that 
                //  ones that are already found
                Vec2 dir = b - a;
                dir.normalize();
                float cDot = dir.dot( destDir );
                if (cDot > bestDot && pos.dist2( b ) > c_TrackEpsilon)
                {
                    pBestTrack  = pTrack;
                    bForward    = true;
                    bestPt      = i - 1;
                    bestDot     = cDot;
                }
                cDot = -dir.dot( destDir );
                if (cDot > bestDot && pos.dist2( a ) > c_TrackEpsilon)
                {
                    pBestTrack  = pTrack;
                    bForward    = false;
                    bestPt      = i - 1;
                    bestDot     = cDot;
                }
            }
        }
        ++it;
    }

    //  fill in the path
    if (pBestTrack != NULL)
    {
         const PolyLine2& poly = pBestTrack->GetPath();
         int nPt = poly.GetNPoints();
         if (bForward)
         {
            for (int i = bestPt + 1; i < nPt; i++)
            {   
                const Vec2& pt = poly.GetPoint( i );
                AddPathPoint( Vec3( pt.x, pt.y, 0.0f ) );
            }
         }
         else
         {
            for (int i = bestPt; i >= 0; i--)
            {
                const Vec2& pt = poly.GetPoint( i );
                AddPathPoint( Vec3( pt.x, pt.y, 0.0f ) );
            }
         }
         return true;
    }
    return false;
} // JMazeControl::MoveTo

void JMazeControl::OnMouse( JMouseEvent& e )
{
    if (!GetChar()) return;
    if (e.MouseKey() == mkLeft && e.Action() == aKeyDown)
    {
        Vec3 dpos( e.MouseX(), e.MouseY(), 0.0f );
        Vec3 dir = dpos - GetChar()->GetPos();
        dir.normalize();
        float destDir = RadToDeg( atan2f( dir.y, dir.x ) );
        MoveTo( destDir );
    }
} // JMazeControl::OnMouse

void JMazeControl::OnKey( JKeyEvent& e )
{
/*
    if (e.Action() == aKeyDown)
    {
        if (e.Key() == VK_RIGHT) MoveTo( 0.0f   );
        else if (e.Key() == VK_DOWN)  MoveTo( 90.0f  );
        else if (e.Key() == VK_LEFT)  MoveTo( 180.0f );
        else if (e.Key() == VK_UP)    MoveTo( 270.0f );
        else return;
        e.Consume();
    }
*/
} // JMazeControl::OnKey