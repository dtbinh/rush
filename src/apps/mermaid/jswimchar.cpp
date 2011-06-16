/*****************************************************************************/
/*  File:   JSwimChar.cpp
/*  Date:   26.08.2006
/*  Desc:   Implementation of the swimming mermaid infernal hackmachine
/*  Author: Ruslan Shestopalyuk 
/*****************************************************************************/
#include "stdafx.h"
#include "Angle.h"
#include "JAnimation.h"
#include "JAnmSprite.h"
#include "JCharacter.h"
#include "JSwimChar.h"

/*****************************************************************************/
/*  JSwimChar implementation
/*****************************************************************************/
decl_class(JSwimChar);
JSwimChar::JSwimChar()
{
    m_pMoveL        = NULL;
    m_pStandL       = NULL;
    m_pStandDL      = NULL;
    m_pStandUL      = NULL;
    m_pTalkL        = NULL;
    m_pTalkDL       = NULL;
    m_pMoveStandL   = NULL;
    m_pTurnLR       = NULL;

    m_State         = csIdle;
    m_StartAngle    = 0.0f;
    m_pCurAnm       = NULL;
} // JSwimChar::JSwimChar

void JSwimChar::Init()
{
    m_pMoveL        = FindObject<JAnmSprite>( "moveL" );
    m_pStandL       = FindObject<JAnmSprite>( "standL" );
    m_pStandDL      = FindObject<JAnmSprite>( "standDL" );
    m_pStandUL      = FindObject<JAnmSprite>( "standUL" );
    m_pTalkL        = FindObject<JAnmSprite>( "talkL" );
    m_pTalkDL       = FindObject<JAnmSprite>( "talkDL" );
    m_pMoveStandL   = FindObject<JAnmSprite>( "movestandL" );
    m_pTurnLR       = FindObject<JAnmSprite>( "turnLR" );

    m_pMoveL->Stop();     
    m_pStandL->Stop();       
    m_pStandDL->Stop();      
    m_pStandUL->Stop();      
    m_pTalkL->Stop();        
    m_pTalkDL->Stop();       
    m_pMoveStandL->Stop();   
    m_pTurnLR->Stop();       

} // JSwimChar:Init

void JSwimChar::Render()
{   
    JCharacter::Render();
    JAnmContext& ctx = JAnimation::GetContext();   
    JAnmSprite* pAnm = NULL;

    if (!stricmp( ctx.m_State.c_str(), "move" ))
    {
        float angle = 0.0f;
        
        //  180 degrees turning animation
        Vec2 tDir( ctx.m_Target.x - ctx.m_Pos.x, ctx.m_Target.y - ctx.m_Pos.y );
        float dLen = tDir.normalize();
        float cAngR = DegToRad( ctx.m_Angle );
        Vec2 cDir( cosf( cAngR ), sinf( cAngR ) );
        float dAng = acosf( cDir.dot( tDir ) );
        bool bStartTurning = false;
        if ( dLen > 5.0f && _fabs( dAng ) > c_PI*0.6f && 
            m_State != csTurnLR && m_State != csTurnRL)
        {
            m_StartAngle = Angle( RadToDeg( atan2f( tDir.y, tDir.x ) ) );
            if (ctx.m_Angle >= 90.0f && ctx.m_Angle < 270.0f)
            {
                m_State = csTurnLR;
            }
            else
            {
                m_State = csTurnRL;
            }
            bStartTurning = true;
        } 
        
        if (m_State == csTurnLR || m_State == csTurnRL)
        {
            pAnm = m_pTurnLR;
            float t = m_pTurnLR->GetCurTime()/m_pTurnLR->GetDuration();
            if ((t >= 0.95f || !m_pTurnLR->IsPlaying()) && !bStartTurning)
            {
                m_State = csMove;
            }

            if (m_State == csTurnRL)
            {
                pAnm->SetMirrored( true );
                angle = m_StartAngle + 180;
            }
            else if (m_State == csTurnLR)
            {
                pAnm->SetMirrored( false );
                angle = m_StartAngle;
            }
        }

        if (m_State == csStandMove && 
            !m_pMoveStandL->IsPlaying())
        {
            m_State = csMove;
            pAnm = m_pMoveL;
        }

        //  start transition move-stand
        if (m_State == csIdle || m_State == csMoveStand)
        {
            m_State = csStandMove;
            m_StartAngle = ctx.m_Angle;
            m_pMoveStandL->SetBackwards( true );
            pAnm = m_pMoveStandL;
        }

        //  correct animation rotation, so when it ends we should move in right direction
        if (m_State == csStandMove)
        {
            pAnm = m_pMoveStandL;
            float t = m_pMoveStandL->GetCurTime()/m_pMoveStandL->GetDuration();
            float dAng = 0.0f;
            if (m_StartAngle > 90.0f && m_StartAngle < 270.0f)
            {
                pAnm->SetMirrored( false );
                dAng = Angle( ctx.m_Angle + 180 );   
            }
            else
            {
                pAnm->SetMirrored( true );
                dAng = Angle( ctx.m_Angle );
            }

            if (dAng > 180.0f) dAng -= 360.0f;
            angle = t*dAng;

            if (t >= 0.95f) m_State = csMove;
        }
   
        //  play moving animation
        if (m_State == csMove)
        {
            pAnm = m_pMoveL; 
            if (ctx.m_Angle > 90.0f && ctx.m_Angle <= 270.0f)
            {
                pAnm->SetMirrored( false );
                angle = ctx.m_Angle + 180;
            }
            else
            {
                pAnm->SetMirrored( true );
                angle = ctx.m_Angle;
            }
        }
        pAnm->SetRotation( angle );
    }
    else if (!stricmp( ctx.m_State.c_str(), "idle" ) ||
             !stricmp( ctx.m_State.c_str(), "talk" ))
    {
        float angle = 0.0f;

        if (m_State == csMoveStand && 
            !m_pMoveStandL->IsPlaying())
        {
            m_State = csIdle;
        }

        //  start transition stand-move
        if (m_State == csMove || m_State == csStandMove || 
            m_State == csTurnLR || m_State == csTurnRL)
        {
            m_State = csMoveStand;
            m_StartAngle = ctx.m_Angle;
            m_pMoveStandL->SetBackwards( false );
        }

        //  correct animation rotation, so when it ends we should be in standing position
        if (m_State == csMoveStand)
        {
            pAnm = m_pMoveStandL;
            float t = 1.0f - m_pMoveStandL->GetCurTime()/m_pMoveStandL->GetDuration();
            float dAng = 0.0f;
            if (m_StartAngle > 90.0f && m_StartAngle < 270.0f)
            {
                pAnm->SetMirrored( false );
                dAng = Angle( ctx.m_Angle - 180 );   
            }
            else
            {
                pAnm->SetMirrored( true );
                dAng = Angle( ctx.m_Angle );
                if (dAng > 180.0f) 
                {
                    dAng -= 360.0f;
                }
            }

            if (dAng > 180.0f) 
            {
                dAng -= 360.0f;
            }

            angle = t*dAng;

            m_pMoveStandL->SetRotation( angle );

            if (t <= 0.05f) m_State = csIdle;
        }

        //  play idle animation
        if (m_State == csIdle)
        {
            if (!stricmp( ctx.m_State.c_str(), "idle" ))
            {
                pAnm = PickIdleAnimation();
            }
            else if (!stricmp( ctx.m_State.c_str(), "talk" ))
            {
                pAnm = PickTalkAnimation();
            }
        }
    }

    if (m_pCurAnm != pAnm)
    {
        if (m_pCurAnm) m_pCurAnm->Stop();
        m_pCurAnm = pAnm;
        if (pAnm) pAnm->Play();
    }

    if (m_pCurAnm && !m_pCurAnm->IsPlaying()) 
    {
        m_pCurAnm->Play();
    }
} // JSwimChar::Render

JAnmSprite* JSwimChar::PickIdleAnimation()
{
    JAnmContext& ctx = JAnimation::GetContext();
    if (ctx.m_Angle > 90.0f && ctx.m_Angle <= 150.0f)
    {
        m_pStandDL->SetMirrored( false );
        return m_pStandDL;
    }
    else if (ctx.m_Angle > 150.0f && ctx.m_Angle <= 210.0f)
    {
        m_pStandL->SetMirrored( false );
        return m_pStandL;
    }
    else if (ctx.m_Angle > 210.0f && ctx.m_Angle <= 270.0)
    {
        m_pStandUL->SetMirrored( false );
        return m_pStandUL;
    }
    else if (ctx.m_Angle > 270.0 && ctx.m_Angle <= 330.0f)
    {
        m_pStandUL->SetMirrored( true );
        return m_pStandUL;
    }
    else if ((ctx.m_Angle > 330.0f && ctx.m_Angle <= 360.0f) ||
             (ctx.m_Angle >= 0.0f && ctx.m_Angle <= 30.0f))
    {
        m_pStandL->SetMirrored( true );
        return m_pStandL;
    }
    else if (ctx.m_Angle > 30.0f && ctx.m_Angle <= 90.0f)
    {
        m_pStandDL->SetMirrored( true );
        return m_pStandDL;
    }
    return NULL;
} // JSwimChar::PickIdleAnimation

JAnmSprite* JSwimChar::PickTalkAnimation()
{
    JAnmContext& ctx = JAnimation::GetContext();
    if (ctx.m_Angle > 90.0f && ctx.m_Angle <= 150.0f)
    {
        m_pTalkDL->SetMirrored( false );
        return m_pTalkDL;
    }
    else if (ctx.m_Angle > 150.0f && ctx.m_Angle <= 210.0f)
    {
        m_pTalkL->SetMirrored( false );
        return m_pTalkL;
    }
    else if (ctx.m_Angle > 210.0f && ctx.m_Angle <= 270.0f)
    {
        m_pStandUL->SetMirrored( false );
        return m_pStandUL;
    }
    else if (ctx.m_Angle > 270.0f && ctx.m_Angle <= 330.0f)
    {
        m_pStandUL->SetMirrored( true );
        return m_pStandUL;
    }
    else if ((ctx.m_Angle > 330.0f && ctx.m_Angle <= 360.0f) ||
             (ctx.m_Angle > 0.0f && ctx.m_Angle <= 30.0f))
    {
        m_pTalkL->SetMirrored( true );
        return m_pTalkL;
    }
    else if (ctx.m_Angle > 30.0f && ctx.m_Angle <= 90.0f)
    {
        m_pTalkDL->SetMirrored( true );
        return m_pTalkDL;
    }

    return NULL;
} // JSwimChar::PickTalkAnimation


