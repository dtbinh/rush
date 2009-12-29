/*****************************************************************************/
/*	File:   JSwimChar.h
/*  Date:   26.08.2006
/*  Author: Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __JSwimChar_H__
#define __JSwimChar_H__


enum JSwimCharState
{
    csUnknown   = 0,
    csIdle      = 1,
    csMove      = 2,
    csMoveStand = 3,
    csStandMove = 4,
    csTalk      = 5,
    csTurnLR    = 6,
    csTurnRL    = 7
}; // enum JSwimCharState

enum_beg(JSwimCharState)
    enum_val( "Unknown",    csUnknown   ),
    enum_val( "Idle",       csIdle      ),
    enum_val( "Move",       csMove      ),
    enum_val( "MoveStand",  csMoveStand ),
    enum_val( "StandMove",  csStandMove ),
    enum_val( "Talk",       csTalk      ),
    enum_val( "TurnLR",     csTurnLR    ),
    enum_val( "TurnRL",     csTurnRL    )
enum_end(JSwimCharState)

/*****************************************************************************/
/*	Class:  JSwimChar
/*	Desc:   Boolean button
/*****************************************************************************/
class JSwimChar : public JCharacter
{
    JAnmSprite*     m_pMoveL;
    JAnmSprite*     m_pStandL;
    JAnmSprite*     m_pStandDL;
    JAnmSprite*     m_pStandUL;
    JAnmSprite*     m_pTalkL;
    JAnmSprite*     m_pTalkDL;
    JAnmSprite*     m_pMoveStandL;
    JAnmSprite*     m_pTurnLR;

    JAnimation*     m_pCurAnm;

    JSwimCharState  m_State;
    float           m_StartAngle;

public:
                    JSwimChar       ();
    virtual void    Render          (); 
    virtual void    Init            (); 

    const char*     GetCurAnmName   () const { return m_pCurAnm ? m_pCurAnm->GetName() : ""; }
    void            SetCurAnmName   ( const char* name )
    {
        m_pCurAnm = FindObject<JAnimation>( name );
    }

    expose( JSwimChar )
    {
        parent(JCharacter);
        field( "State", m_State );
        field( "StartAngle", m_StartAngle );
        prop( "CurAnimation", GetCurAnmName, SetCurAnmName );
    }

private:
    JAnmSprite*     PickIdleAnimation();
    JAnmSprite*     PickTalkAnimation();
}; // class JSwimChar

#endif // __JSwimChar_H__


