/***********************************************************************************/
//  File:   JCharControl.cpp
//  Date:   29.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "JCharacter.h"
#include "JAnimation.h"
#include "JPolyButton.h"
#include "JHotSpot.h"
#include "JAnimServer.h"
#include "JQuestGame.h"
#include "JCharControl.h"

/***********************************************************************************/
/*  JCharControl implementation
/***********************************************************************************/
decl_class(JCharControl);
JCharControl::JCharControl()
{
    m_pChar         = NULL;
    m_pExit         = NULL;
    m_TopScale      = 1.0f;
    m_BotScale      = 1.0f;

    m_TopHorizon    = 0.0f;
    m_BotHorizon    = 1024.0f;

    m_MoveSpeed     = 100.0f;
    m_RunSpeed      = 180.0f;
    m_RotSpeed      = 360.0f;
    m_MoveState     = CharMove_Idle;
    m_CurTime       = 0.0f;
    m_CharState     = "idle";
    m_bIs3D         = false;
    m_RunDistance   = 10000.0f;
    m_Direction     = -1.0f;

    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );

	SetCursor( "walk" );
} // JCharControl::JCharControl

void JCharControl::ClearPath() 
{ 
    m_Path.clear();
} // JCharControl::ClearPath

void JCharControl::Init()
{
    m_pChar = g_pObjectServer->FindObject<JCharacter>( m_CharName.c_str(), this );
} // JCharControl::Init

void JCharControl::DrawBounds()
{
    //  draw horizons
    Frame ext = GetExt();
    g_pDrawServer->DrawLine( ext.x, m_TopHorizon, ext.r(), m_TopHorizon, 0xFFFF00FF, 0xFFFF00FF );
    g_pDrawServer->DrawLine( ext.x, m_BotHorizon, ext.r(), m_BotHorizon, 0xFFFF00FF, 0xFFFF00FF );

    if (!m_pChar) return;
    m_pChar->DrawBounds();
    Vec3 cpos = m_pChar->GetPos();
    const DWORD c_PathClr = 0xEEFF7777;
    const DWORD c_CurPathClr = 0xEEFF2222;
    if (m_MoveState != CharMove_Idle)
    {
        g_pDrawServer->DrawLine( cpos.x, cpos.y, m_DestPos.x, m_DestPos.y, c_CurPathClr, c_CurPathClr );
    }
    //  draw character path
    int pathPt = m_Path.size();
    if (pathPt > 0)
    {
        if (m_MoveState != CharMove_Idle)
        {
            g_pDrawServer->DrawLine( m_DestPos.x, m_DestPos.y, m_Path[0].x, m_Path[0].y, c_CurPathClr, c_CurPathClr );
        }
        else
        {
            g_pDrawServer->DrawLine( cpos.x, cpos.y, m_Path[0].x, m_Path[0].y, c_PathClr, c_PathClr );
        }
        JVec3List::const_iterator it = m_Path.begin();
        for (int i = 1; i < pathPt; i++)
        {
            const Vec3& a = *it;
            it++;
            const Vec3& b = *it;
            g_pDrawServer->DrawLine( a.x, a.y, b.x, b.y, c_PathClr, c_PathClr );
        }
    }
} // JCharControl::DrawBounds

void JCharControl::SetCharName( const char* name )
{
    m_CharName  = name;
    m_pChar = obj_cast<JCharacter>( g_pObjectServer->FindObject( m_CharName.c_str(), NULL, this ) );
} // JCharControl::SetChar

void JCharControl::SetCharPos( const Vec3& pos, float dir )
{
    if (!m_pChar) return;
    m_pChar->SetPos( pos );
    m_pChar->SetDir( dir );
    ClearPath();
    m_MoveState = CharMove_Idle;
} // JCharControl::SetCharPos

bool JCharControl::MoveTo( const Vec3& pos, bool bImmediately, float direction )
{
    if (!m_pChar) return false;
    if (bImmediately) ClearPath();
    
    if (m_Path.size() > 0)
    {
        const float c_WeldBias = 10.0f;
        Vec3& top = m_Path.back();
        if (top.dist( pos ) < c_WeldBias)
        {
            m_Path.back() = pos;
            return true;
        }
    }
    m_Path.push_back( pos );    
    m_Direction = direction;
    return true;
} // JCharControl::MoveTo

float JCharControl::GetCharScale() const
{
    if (!m_pChar) return 1.0f;
    float ratio = (m_pChar->GetPos().y - m_TopHorizon)/(m_BotHorizon - m_TopHorizon);
    return m_TopScale + (m_BotScale - m_TopScale)*ratio;
} // JCharControl::GetCharScale

void JCharControl::MoveToPath()
{
    Vec3 dpos = m_Path[0];
   m_Path.erase( m_Path.begin() );
   Vec3 dir = dpos - m_pChar->GetPos();
   float dirLen = dir.normalize();
   float destDir = RadToDeg( atan2f( dir.y, dir.x ) );
   if (dirLen < c_FltEpsilon)
   {
       destDir = m_pChar->GetDir();
   }

   m_StartPos = m_pChar->GetPos();
   m_DestPos  = dpos;

   //  some black magic to find shortest arc rotation direction
   m_StartDir = m_pChar->GetDir();
   m_DestDir  = destDir;
   float dirDelta = m_DestDir - m_StartDir;
   if (m_DestDir < m_StartDir) dirDelta += 360.0f;
   dirDelta = fmodf( dirDelta, 360.0f );
   if (_fabs( dirDelta ) > 180.0f) 
   {
       if (dirDelta < 0.0f) 
       {
           dirDelta = 360.0f + dirDelta;
       }
       else if (dirDelta > 0.0f) 
       {
           dirDelta = -(360.0f - dirDelta);
       }
   }
   m_DirDelta = dirDelta;
} // JCharControl::MoveToPath

void JCharControl::Render()
{
    JAnmContext& ctx = JAnimation::PushContext();

    if (!m_pChar) 
    {
        SetCharName( m_CharName.c_str() );
    }

    if (!m_pChar) 
    {
        return;
    }

    const Vec3& cpos = m_pChar->GetPos();

    if (m_MoveState == CharMove_Walk || m_MoveState == CharMove_Run)
    {
        double cTime = m_CurTime;
        m_CurTime += JAnimation::GetContext().m_TimeDelta;
        double moveSpeed = (m_MoveState == CharMove_Walk) ? m_MoveSpeed : m_RunSpeed;
        double dist = cTime*moveSpeed;

        float dirDelta = cTime*m_RotSpeed;
        if (dirDelta < _fabs( m_DirDelta ))
        {
            if (m_DirDelta < 0.0f) dirDelta = -dirDelta;
        }
        else 
        {
            dirDelta = m_DirDelta;
        }
        
        Vec3 cPos;
        float totalDist = 0.0f;
        if (m_bIs3D)
        {
            Vec3 wsStart = ScreenToWorld( Vec2( m_StartPos.x, m_StartPos.y ) );
            Vec3 wsDest  = ScreenToWorld( Vec2( m_DestPos.x, m_DestPos.y ) );
            Vec3 moveDir = wsDest - wsStart;
            totalDist = moveDir.normalize();
            if (dist > totalDist) 
            {
                dist = totalDist;
            }
            cPos = moveDir*dist + wsStart;
            cPos = WorldToScreen( cPos );
            cPos.z = 0.0f;
        }
        else
        {
            Vec3 moveDir = m_DestPos - m_StartPos;
            totalDist = moveDir.normalize();
            if (dist > totalDist) 
            {
                dist = totalDist;
            }
            cPos = moveDir*dist + m_StartPos;
        }

        float cDir = m_StartDir + dirDelta;

        m_pChar->SetDir( cDir );
        m_pChar->SetPos( cPos );

        if (_fabs( m_DirDelta - dirDelta ) < c_FltEpsilon && 
            _fabs( dist - totalDist ) < c_FltEpsilon)
        {
            m_MoveState = CharMove_Idle;
            if (m_Direction != -1.0f)
            {
                m_pChar->SetDir( m_Direction );
                m_Direction = -1.0f;
            }
        }
    }
    else if (m_Path.size() > 0)
    {
        MoveToPath();

        m_CurTime = 0.0f;
        m_MoveState = CharMove_Walk;
        m_pChar->SetState( "move" );

        float dist = (m_DestPos - m_StartPos).norm();
        if (m_bIs3D)
        {
            Vec3 wDest = ScreenToWorld( Vec2( m_DestPos.x, m_DestPos.y ) );
            Vec3 wStart = ScreenToWorld( Vec2( m_StartPos.x, m_StartPos.y ) );
            dist = wDest.dist( wStart );
        }
        if (dist > m_RunDistance)
        {
            m_pChar->SetState( "run" );
            m_MoveState = CharMove_Run;
        }
    }
    else
    {
        //  hack:
        if (IsEnabled())
        {
            m_CharState = "idle";
        }
        m_pChar->SetState( m_CharState.c_str() );
    }

    ctx.m_Scale *= GetCharScale();
    ctx.m_Target = m_DestPos;

    if (m_bIs3D)
    {
        Vec3 charPos = m_pChar->GetPos();
        Vec3 wsPos = ScreenToWorld( Vec2( charPos.x, charPos.y ) );
        
        //  build rotation matrix
        Vec3 charFw = GetCharWorldDir();
        Vec3 charR;
        charR.cross( Vec3::oZ, charFw );
        charR.normalize();
        Mat3 wsRot( charFw, charR, Vec3::oZ );

        ctx.m_TM.srt( 1.0f, wsRot, wsPos );
    }

    m_pChar->RenderTree();
} // JCharControl::Render

Vec3 JCharControl::GetCharWorldDir()
{
    if (!m_pChar) return Vec3::oX;
    Vec2 dir = m_pChar->GetDirVec();
    Vec3 charPos = m_pChar->GetPos();
    Vec3 wsPos = ScreenToWorld( Vec2( charPos.x, charPos.y ) );
    const float c_FaceLen = 10.0f;
    Vec3 wsPosF = ScreenToWorld( Vec2( charPos.x + dir.x*c_FaceLen, charPos.y + dir.y*c_FaceLen ) );
    Vec3 charFw = wsPosF - wsPos;
    charFw.normalize();
    return charFw;
} // JCharControl::GetCharWorldDir

Vec3 JCharControl::WorldToScreen( const Vec3& pt )
{
    Mat4 viewTM = g_pRenderServer->GetViewTM();
    Mat4 projTM = g_pRenderServer->GetProjTM();
    Mat4 pToSTM, sToPTM;
    ScreenToProjTM( sToPTM, GetExt() );
    pToSTM.inverse( sToPTM );
    Mat4 tm = viewTM*projTM*pToSTM;
    Vec4 res( pt.x, pt.y, pt.z, 1.0f );
    tm.tm( res );
    res /= res.w;
    return Vec3( res.x, res.y, res.z );
} // JCharControl::WorldToScreen

Vec3 JCharControl::ScreenToWorld( const Vec2& pt )
{
    Mat4 viewTM = g_pRenderServer->GetViewTM();
    Mat4 projTM = g_pRenderServer->GetProjTM();
    Mat4 sToPTM;
    ScreenToProjTM( sToPTM, GetExt() );
    Mat4 backTM = sToPTM*inverse( viewTM*projTM );
    
    Vec4 begRay( pt.x, pt.y, 0.0f, 1.0f );
    Vec4 endRay( pt.x, pt.y, 1.0f, 1.0f );
    backTM.tm( begRay );
    begRay /= begRay.w;
    backTM.tm( endRay );
    endRay /= endRay.w;
    Vec4 ray = endRay - begRay;
    ray.normalize();
    
    Mat4 camTM;
    camTM.inverse( viewTM );
    
    //  intersect ray with xOy plane
    float t = -begRay.z/ray.z;
    Vec4 res = begRay + t*ray;
    return Vec3( res.x, res.y, res.z );
} // JCharControl::ScreenToWorld

void JCharControl::PostRender()
{
    JAnimation::PopContext();
} // JCharControl::PostRender
