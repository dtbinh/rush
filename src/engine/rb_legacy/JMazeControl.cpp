/***********************************************************************************/
/*  File:   JMazeControl.cpp
/*  Date:   29.05.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "JCharacter.h"
#include "JObjectIterator.h"
#include "JPath.h"
#include "JCharControl.h"
#include "JMazeControl.h"

/***********************************************************************************/
/*  JMazeControl implementation
/***********************************************************************************/
decl_class(JMazeControl);
JMazeControl::JMazeControl()
{
} // JMazeControl::JMazeControl

void JMazeControl::Init()
{
} // JMazeControl::Init

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
} // JMazeControl::Render

bool JMazeControl::MoveTo( float dir )
{
    if (!GetChar()) return false;
    ClearPath();
    Vec2 pos( GetChar()->GetPos().x, GetChar()->GetPos().y );
    float rDir = DegToRad( dir );
    Vec2 destDir( cosf( rDir ), sinf( rDir ) );

    //  find all relevant tracks, and pick one wich fits given direction best
    float   bestDot     = -1.0f;
    JPath*  pBestPath   = NULL;
    int     bestPt      = -1;
    bool    bForward    = true;

    JObjectIterator it( this );
    while (it)
    {
        JPath* pPath = obj_cast<JPath>( *it );
        ++it;
        if (!pPath || !pPath->IsVisible())
        {
            continue;
        }
        const PolyLine2& poly = pPath->GetPath();
        int nPt = poly.GetNPoints();
        for (int i = 1; i < nPt; i++)
        {
            const Vec2& a = poly.GetPoint( i - 1 );
            const Vec2& b = poly.GetPoint( i     );
            Seg2 seg( a, b );
            const float c_PathEpsilon = 100.0f;
            float dist2 = seg.dist2( pos );
            if (dist2 > c_PathEpsilon) continue;
            //  check if forward and backward directions are better that 
            //  ones that are already found
            Vec2 dir = b - a;
            dir.normalize();
            float cDot = dir.dot( destDir );
            if (cDot > bestDot && pos.dist2( b ) > c_PathEpsilon)
            {
                pBestPath  = pPath;
                bForward    = true;
                bestPt      = i - 1;
                bestDot     = cDot;
            }
            cDot = -dir.dot( destDir );
            if (cDot > bestDot && pos.dist2( a ) > c_PathEpsilon)
            {
                pBestPath  = pPath;
                bForward    = false;
                bestPt      = i - 1;
                bestDot     = cDot;
            }
        }
    }

    //  fill in the path
    if (pBestPath != NULL)
    {
         const PolyLine2& poly = pBestPath->GetPath();
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
} // JMazeControl::OnKey