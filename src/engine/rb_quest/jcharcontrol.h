/***********************************************************************************/
//  File:   JCharControl.h
//  Date:   29.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCHARCONTROL_H__
#define __JCHARCONTROL_H__
#include <queue>
#include "JWeakRef.h"

class JCharacter;
class JHotSpot;

enum CharMoveState
{
    CharMove_Idle = 0,
    CharMove_Walk = 1,
    CharMove_Run  = 2
}; // enum CharMoveState

enum_beg(CharMoveState)
    enum_val( "Idle",   CharMove_Idle ),
    enum_val( "Walk",   CharMove_Walk ),
    enum_val( "Run",    CharMove_Run  )
enum_end(CharMoveState)

/***********************************************************************************/
//  Class:  JCharControl
//  Desc:   Used to control character movement inside the location
/***********************************************************************************/
class JCharControl : public JWidget
{
protected:
    std::string          m_CharName;
    JWeakRef<JCharacter> m_pChar;
    JWeakRef<JHotSpot>   m_pExit;

    float               m_TopScale;
    float               m_BotScale;
    float               m_TopHorizon;
    float               m_BotHorizon;
    float               m_CurTime;

    float               m_MoveSpeed;
    float               m_RunSpeed;
    float               m_RotSpeed;

    Vec3                m_StartPos;
    Vec3                m_DestPos;
    CharMoveState       m_MoveState;

    float               m_StartDir;
    float               m_DestDir;
    float               m_DirDelta;

    bool                m_bIs3D;

    std::string         m_CharState;

    JVec3List           m_Path;
    float               m_RunDistance;
    float               m_Direction;

public:
                        JCharControl();
    virtual void        Render      ();
    virtual void        PostRender  ();
    virtual void        DrawBounds  ();
    virtual void        Init        ();
    const char*         GetCharName () const { return m_CharName.c_str(); }
    void                SetCharName ( const char* name );

    void                SetCharPos  ( const Vec3& pos, float dir );
    virtual bool        MoveTo      ( const Vec3& pos, bool bImmediately = false, float direction = -1.0f );
    virtual bool        CanMoveTo   ( const Vec3& pos ) const { return true; }
    bool                IsMoving    () const { return m_MoveState != CharMove_Idle; }

    JCharacter*         GetChar     () { return m_pChar; }
    void                AddPathPoint( const Vec3& pt ) { m_Path.push_back( pt ); }
    void                ClearPath   ();

    JHotSpot*           GetExit     () { return m_pExit; }
    void                SetExit     ( JHotSpot* pExit ) { m_pExit = pExit; }
    float               GetCharScale() const;

    void                SetupCamera ();

    expose( JCharControl )
    {
        parent( JWidget );
        prop ( "Char",            GetCharName, SetCharName  );
        field( "TopScale",        m_TopScale     );
        field( "BotScale",        m_BotScale     );
        field( "TopHorizon",      m_TopHorizon   );
        field( "BotHorizon",      m_BotHorizon   );
        field( "MoveSpeed",       m_MoveSpeed    );
        field( "RotSpeed",        m_RotSpeed     );
        field( "CurTime",         m_CurTime      );
        field( "CharState",       m_CharState    );
        field( "Path",            m_Path         );
        field( "MoveState",       m_MoveState      );
        field( "StartPos",        m_StartPos     );
        field( "DestPos",         m_DestPos      );
        field( "StartDir",        m_StartDir     );
        field( "DestDir",         m_DestDir      );
        field( "DirDelta",        m_DirDelta     );
        field( "Is3D",            m_bIs3D        );
        field( "RunDistance",     m_RunDistance  );
        field( "RunSpeed",        m_RunSpeed     );
    }
protected:
    Vec3        ScreenToWorld( const Vec2& pt );
    Vec3        GetCharWorldDir();
    Vec3        WorldToScreen( const Vec3& pt );

    void        MoveToPath();
}; // class JCharControl

#endif //__JCHARCONTROL_H__