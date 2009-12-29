/***********************************************************************************/
/*  File:   JPuzzleGame.h
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPUZZLEGAME_H__
#define __JPUZZLEGAME_H__

#include "JMathTypeCast.h"

/***********************************************************************************/
/*  Class:  JPuzzleGame
/*  Desc:   
/***********************************************************************************/
class JPuzzleGame : public JLocation
{
    bool                m_bHomeFreeze;
    bool                m_bDropReturn;
    float               m_SnapDist;
    std::string         m_SndTake;
    std::string         m_SndDropWrong;
    std::string         m_SndDropRight;

    JPointList          m_Criteria;
    Vec3                m_PiecePos;

public:
                        JPuzzleGame ();
    virtual void        Render      ();
    virtual void        Init        ();
    void                Shuffle     ();
    bool                IsHomeFreeze() const { return m_bHomeFreeze; }
    bool                IsDropReturn() const { return m_bDropReturn; }
    float               GetSnapDist () const { return m_SnapDist; }
    int                 GetNumAtHome( int tag ) const;

    JPointList          GetCriteria () const 
    { 
        return m_Criteria; 
    }
    void                SetCriteria ( JPointList criteria ) { m_Criteria = criteria; }

    void                OnSuccess   (){}
    void                OnTakePiece ();
    void                OnDropWrong ();
    void                OnDropRight ();
    void                SetPiecePos  ( const Vec2& pos ) { m_PiecePos = Vec3( pos.x, pos.y, 0.0f ); }

    expose( JPuzzleGame )
    {
        parent( JLocation );
        field ( "HomeFreeze",   m_bHomeFreeze   );
        field ( "DropReturn",   m_bDropReturn   );
        field ( "SnapDist",     m_SnapDist      );
        field ( "SndTake",      m_SndTake       );
        field ( "SndDropWrong", m_SndDropWrong  );
        field ( "SndDropRight", m_SndDropRight  );
        field ( "PiecePos",     m_PiecePos       );
        prop  ( "Criteria",     GetCriteria, SetCriteria );
        rprop ( "NumAtHome",    GetNumAtHome    );
        method( "OnSuccess",    OnSuccess       );
        method( "Shuffle",      Shuffle         );
        method( "OnDropRight",  OnDropRight     );
        method( "OnTakePiece",  OnTakePiece     );
        method( "OnDropWrong",  OnDropWrong     );
    }
}; // class JPuzzleGame

#endif //__JPUZZLEGAME_H__