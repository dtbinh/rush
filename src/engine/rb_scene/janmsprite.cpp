/***********************************************************************************/
//  File:   JAnmSprite.cpp
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "Color.h"
#include "JAnimation.h"
#include "JModelInstance.h"
#include "IModelServer.h"
#include "JAnmSprite.h"

/***********************************************************************************/
/*  JAnmSprite implementation
/***********************************************************************************/
decl_class(JAnmSprite);
JAnmSprite::JAnmSprite()
{
    m_StartAngle    = 0.0f;
    m_EndAngle      = 360.0f;
    m_Scale         = 1.0f;

    m_StartFrame    = 0;
    m_NumFrames     = -1;
    m_FramesPerSec  = 10.0f;

    m_Pivot         = Vec2::null;
    m_Pos           = Vec3::null;
    m_bScreenSpace  = true;

    m_PackID        = -1;
    m_FrameID       = -1;
    m_bFilter       = true;

    m_Rotation      = 0.0f;
    m_bMirrored     = false;
    m_bBackwards    = false;
    m_bAlwaysCache  = false;
    m_bBlendFrames  = false;
    m_Color         = 0xFFFFFFFF;
    m_Align         = Align_Ground;

    m_AttachInstanceID  = -1;
    m_AttachBoneID      = -1;
} // JAnmSprite::JAnmSprite

void JAnmSprite::Init()
{  
} // JAnmSprite::Init

void JAnmSprite::ResInit()
{
    //m_PackID = g_pDrawServer->GetSpriteID( m_PackageName.c_str(), 0, m_bAlwaysCache );
} // JAnmSprite::ResInit

float JAnmSprite::GetTransparency() const
{
    uint32_t transp = (m_Color&0xFF000000) >> 24;
    float res = float( transp )/255.0f;
    return res;
}

void  JAnmSprite::SetTransparency( float val )
{
    uint32_t transp = uint32_t( val*255.0f );
    m_Color = (m_Color&0x00FFFFFF)|(transp << 24);
}

void JAnmSprite::SetAttachInstance( const char* name )
{
    m_AttachInstance    = name;
    m_AttachBoneID      = -1;
    m_AttachInstanceID  = -1;
}

void JAnmSprite::SetAttachBone( const char* name )
{
    m_AttachBone    = name;
    m_AttachBoneID  = -1;
}

void JAnmSprite::OnPlay()
{
    if (m_PackID < 0) 
    {
        m_PackID = g_pDrawServer->GetSpriteID( m_PackageName.c_str(), 0, m_bAlwaysCache );
    }

    if (GetDuration() == 0.0f)
    {
        int nFrames = m_NumFrames;
        if (nFrames < 0) nFrames = g_pDrawServer->GetNFrames( m_PackID );
        float animLen = float( nFrames )/m_FramesPerSec;
        SetDuration( animLen );
    }
} // JAnmSprite::OnPlay

void JAnmSprite::Render()
{
    JAnimation::Render();
    
    JAnmContext& ctx = GetContext();

    //  filter by direction angle
    float angle = ctx.m_Angle;
    //  treat differently sector which contains zero
    if (m_EndAngle > 360.0f)
    {
        bool bInSector = false;
        if (angle >= m_StartAngle && angle <= 360.0f)
        {
            bInSector = true;
        }
        if (angle >= 0.0 && angle < m_EndAngle - 360.0f)
        {
            bInSector = true;
        }
        if (!bInSector) return;
    }
    else if (angle < m_StartAngle || angle >= m_EndAngle) 
    {
        return;
    }

    //  FIXME:
    if (m_PackID == -1) 
    {
        m_PackID = g_pDrawServer->GetSpriteID( m_PackageName.c_str() );
    }

    int nFrames = m_NumFrames;
    if (nFrames < 0) 
    {
        nFrames = g_pDrawServer->GetNFrames( m_PackID );
    }

    float cTime = GetCurTime();
    if (cTime > GetDuration() && !IsLooped() && IsPlaying())
    {
        Stop();
    }

    if (ctx.m_bStateOnly)
    {
        return;
    }

    m_FrameID = GetPlayedFrame( cTime );

    float scale = ctx.m_Scale*m_Scale;
    bool bLinFilter = m_bFilter ? (_fabs( scale - 1.0f ) > c_FltEpsilon) : false;
    g_pDrawServer->SetLinFilter( bLinFilter );

    if (m_bScreenSpace)
    {
        Frame rect( ctx.m_Pos.x + m_Pos.x, ctx.m_Pos.y + m_Pos.y, 
                g_pDrawServer->GetFrameW( m_PackID, m_FrameID ), 
                g_pDrawServer->GetFrameH( m_PackID, m_FrameID ) );
        rect.x -= m_Pivot.x*scale;
        rect.y -= m_Pivot.y*scale;
        rect.w *= scale;
        rect.h *= scale;

        //  snap to integer coordinates
        rect.x = floorf( rect.x );
        rect.y = floorf( rect.y );
        rect.w = floorf( rect.w );
        rect.h = floorf( rect.h );

        if (bLinFilter)
        {
            g_pDrawServer->SetAdjustUV( true );
        }

        if (m_bBlendFrames)
        {
            ColorF clr = m_Color;
            uint32_t clr1 = clr;
            uint32_t clr2 = clr;
            g_pDrawServer->DrawSprite( rect, m_PackID, m_FrameID, clr1, DegToRad( m_Rotation ), m_bMirrored );
            g_pDrawServer->DrawSprite( rect, m_PackID, m_FrameID, clr2, DegToRad( m_Rotation ), m_bMirrored );
        }
        else
        {
            g_pDrawServer->DrawSprite( rect, m_PackID, m_FrameID, m_Color, DegToRad( m_Rotation ), m_bMirrored );
        }
        g_pDrawServer->Flush();
        if (bLinFilter)
        {
            g_pDrawServer->SetAdjustUV( false );
        }
    }
    else
    {
        int boneID = -1;
        Mat4 boneTM = Mat4::identity;
        g_pDrawServer->SetZEnable( true );
        g_pDrawServer->SetZWriteEnable( true );

        if (m_AttachInstance.size() > 0)
        {
            if (m_AttachInstanceID == -1)
            {
                JModelInstance* pModelInst = g_pObjectServer->FindObject<JModelInstance>( m_AttachInstance.c_str(), this );
                if (pModelInst)
                {
                    m_AttachInstanceID = pModelInst->GetInstanceID();
                    m_AttachBoneID = g_pModelServer->GetModelBoneID( pModelInst->GetModelID(), m_AttachBone.c_str() );
                }
            }
            g_pModelServer->GetBoneTM( m_AttachInstanceID, m_AttachBoneID, boneTM );
        }

        Vec3 pos = ctx.m_Pos + m_Pos;
        Mat4 tm, flipTM = Mat4::identity;
        if (m_bMirrored)
        {
            flipTM = Mat4( 1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           0, 0, 0, 1);
        }

        tm.st( scale, pos ); 

        if (m_Align == Align_Camera)
        {
            Vec3 boneScale, boneTrans;
            Mat3 boneRot;
            boneTM.factorize( boneScale, boneRot, boneTrans );
            boneTM.st( boneScale, boneTrans );
        }
        tm = flipTM*tm*boneTM;
        Vec2 pivot = g_pDrawServer->GetFramePivot( m_PackID, m_FrameID ) + m_Pivot;

        if (m_Align == Align_Vertical)
        {
            tm = Mat4( 1.0f,    0.0f, 0.0f,   0.0f, 
                       0.0f,    0.0f, -1.0f,  0.0f, 
                       0.0f,    1.0f, 0.0f,   0.0f, 
                       -pivot.x, 0.0f, pivot.y, 1.0f )*tm;
        }
        if (m_Align == Align_XAxis)
        {
            tm = Mat4( 1.0f,    0.0f, 0.0f,   0.0f, 
                       0.0f,    0.0f, -1.0f,  0.0f, 
                       0.0f,    1.0f, 0.0f,   0.0f, 
                       -pivot.x, 0.0f, pivot.y, 1.0f )*tm;
        }
        if (m_Align == Align_YAxis)
        {
            tm = Mat4( 1.0f,    0.0f, 0.0f,   0.0f, 
                       0.0f,    0.0f, -1.0f,  0.0f, 
                       0.0f,    1.0f, 0.0f,   0.0f, 
                       -pivot.x, 0.0f, pivot.y, 1.0f )*tm;
        }
        else if (m_Align == Align_Ground)
        {
            tm = Mat4(  1.0f,   0.0f,    0.0f, 0.0f,  
                            0.0f,   1.0f,    0.0f, 0.0f, 
                            0.0f,   0.0f,    1.0f, 0.0f, 
                           -pivot.x, -pivot.y, 0.0f, 1.0f )*tm;
        }
        else if (m_Align == Align_Camera)
        {
            const Mat4& m = g_pRenderServer->GetViewTM();
            tm = Mat4( 
                 m.e00, m.e10, m.e20, 0.0f, 
                -m.e01,-m.e11,-m.e21, 0.0f, 
                 m.e02, m.e12, m.e22, 0, 
                -m.e00*pivot.x + m.e01*pivot.y, 
                -m.e10*pivot.x + m.e11*pivot.y,
                -m.e20*pivot.x + m.e21*pivot.y, 1.0f )*tm;
        }

        g_pDrawServer->DrawSprite( tm, m_PackID, m_FrameID, m_Color );
    }
} // JAnmSprite::Render

int JAnmSprite::GetPlayedFrame( float cTime )
{
    int nFrames = m_NumFrames;
    if (nFrames < 0) nFrames = g_pDrawServer->GetNFrames( m_PackID );
    float seqTime = float( nFrames )/m_FramesPerSec;
    float nCycles = floorf( cTime/seqTime );
    int   cFrame  = m_FramesPerSec*fmodf( cTime, seqTime );
    cFrame += m_StartFrame;

    if (nCycles >= 1.0f && !IsLooped())
    {
        cFrame = m_StartFrame + nFrames - 1;
    }

    //  if animation is alredy stopped, but still is trying to render, show the last
    //  frame (most probably animation has ended exactly with this one)
    if (!IsPlaying())
    {
        cFrame = m_StartFrame + nFrames - 1;
    }

    if (m_bBackwards)
    {
        cFrame = (nFrames - cFrame - 1) + m_StartFrame;
    }
    return cFrame;
} // JAnmSprite::GetPlayedFrame