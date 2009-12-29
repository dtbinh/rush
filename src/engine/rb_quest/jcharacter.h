/***********************************************************************************/
//  File:   JCharacter.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCHARACTER_H__
#define __JCHARACTER_H__

#include "Timer.h"
#include "JWidget.h"

/***********************************************************************************/
//  Class:  JCharacter
//  Desc:   
/***********************************************************************************/
class JCharacter : public JWidget
{
    Vec3                m_Pos;
    Vec3                m_PrevPos;
    Vec2                m_Dir;
    JString             m_State;

    JStringList         m_IdleStates;
    JVec3List           m_IdleParam;

    float               m_CurIdleDuration;
    float               m_CurTime;
    int                 m_CurIdle;

public:
                        JCharacter  ();
    virtual void        Render      ();
    virtual void        PostRender  ();
    virtual void        DrawBounds  ();
    virtual void        Init        ();

    const char*         GetState    () const { return m_State.c_str(); }
    void                SetState    ( const char* state );
    
    float               GetDir      () const { return RadToDeg( atan2f( m_Dir.y, m_Dir.x ) ); }
    void                SetDir      ( float dir );
    Vec2                GetDirVec   () const { return m_Dir; }

    Vec3                GetPos      () const { return m_Pos; }
    Vec3                GetPrevPos  () const { return m_PrevPos; }
    void                SetPos      ( Vec3 pos ) { m_PrevPos = m_Pos; m_Pos = pos; }
    void                SetPrevPos  ( Vec3 pos ) { m_PrevPos = pos; }


    expose( JCharacter )
    {
        parent( JWidget );
        prop( "State",                GetState,  SetState );
        prop( "Dir",                  GetDir,    SetDir   );
        prop( "Position",             GetPos,    SetPos   );
        field( "CurIdle",             m_CurIdle           );
        field( "CurIdleDuration",     m_CurIdleDuration   );
        field( "IdleStates",          m_IdleStates        );
        field( "IdleParam",           m_IdleParam         );
        field( "PrevPos",             m_PrevPos           );
        field( "CurTime",             m_CurTime           );
    }
}; // class JCharacter

#endif //__JCHARACTER_H__