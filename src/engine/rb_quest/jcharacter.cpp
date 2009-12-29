/***********************************************************************************/
//  File:   JCharacter.cpp
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "Timer.h"
#include "JAnimation.h"
#include "JAnimServer.h"
#include "JAnmSprite.h"
#include "JCharacter.h"

/***********************************************************************************/
/*  JCharacter implementation
/***********************************************************************************/
decl_class(JCharacter);
JCharacter::JCharacter()
{
   m_Dir                = Vec2( 1.0f, 0.0f );
   m_Pos                = Vec3::null;
   m_CurIdleDuration    = 0.0f;
   m_CurIdle            = -1;
   m_CurTime            = 0.0f;
} // JCharacter::JCharacter

void JCharacter::SetDir( float dir )
{
    float ang = DegToRad( dir );
    m_Dir.x = cosf( ang );
    m_Dir.y = sinf( ang );
} // JCharacter::SetDir

void JCharacter::SetState( const char* state )
{
    if (!stricmp( m_State.c_str(), state )) return;
    m_State = state;
    m_CurTime = 0.0f;
} // JCharacter::SetState

void JCharacter::Init()
{
} // JCharacter::Init

void JCharacter::DrawBounds()
{
    Frame rc( m_Pos.x - 10, m_Pos.y - 10, 20, 20 );
    g_pDrawServer->DrawRect( rc, 0xFFFF0000 );
    Vec2 end( m_Pos.x, m_Pos.y );
    end += m_Dir*20.0f;
    g_pDrawServer->DrawLine( m_Pos.x, m_Pos.y, end.x, end.y, 0xFFFF0000, 0xFFFF0000 );

    g_pDrawServer->Flush();
    char posStr[_MAX_PATH];
    int nx = m_Pos.x;
    int ny = m_Pos.y;
    sprintf( posStr, "CharPos={%d, %d}", nx, ny );
    g_pDrawServer->DrawString( 10, 100, GetFontID(), posStr, 0xFF000055 );
    g_pDrawServer->Flush();

} // JCharacter::DrawBounds

void JCharacter::Render()
{
    //  fill animation context for the character:
    JAnmContext& ctx = JAnimation::PushContext();
    float cTime = m_CurTime;
    m_CurTime += JAnimation::GetContext().m_TimeDelta;
    if (!stricmp( m_State.c_str(), "idle" ) && m_IdleStates.size() > 0)
    {
        if (cTime >= m_CurIdleDuration)
        {
            int nIdles = m_IdleParam.size();
            //  HACK: force stand after something else
            if (m_CurIdle == 0)
            {
                //  pick random idle animation, corresponding to the probabilities
                float val    = frand( 0.0f, 1.0f );
                float factor = 1.0f;
                for (int i = 0; i < nIdles; i++)
                {
                    factor -= m_IdleParam[i].z;
                    if (val >= factor)
                    {
                        m_CurIdle = i;
                        break;
                    }
                }
            }
            else
            {
                m_CurIdle = 0;
            }
            if (m_IdleParam.size() > m_CurIdle)
            {
                m_CurIdleDuration = frand( m_IdleParam[m_CurIdle].x, m_IdleParam[m_CurIdle].y );
            }
            m_CurTime = 0.0f;
            cTime = 0.0f;
        }
        m_CurIdle = clamp( m_CurIdle, 0, (int)m_IdleStates.size() - 1 );
        ctx.m_State = m_IdleStates[m_CurIdle].c_str();
    }
    else
    {
        m_CurIdle = -1;
        m_CurIdleDuration = 0.0f;
        ctx.m_State = m_State.c_str();
    }

    ctx.m_Angle = RadToDeg( atan2f( m_Dir.y, m_Dir.x ) );
    if (ctx.m_Angle < 0.0f) ctx.m_Angle += 360.0f;
    ctx.m_Pos   += m_Pos;
    ctx.m_Time  = cTime;
} // JCharacter::Render

void JCharacter::PostRender()
{
    JAnimation::PopContext();
} // JCharacter::