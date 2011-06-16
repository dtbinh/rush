/***********************************************************************************/
/*  File:   JBlotGame.h
/*  Date:   01.08.2006
/*  Author: Juriy Skolyar
/***********************************************************************************/
#ifndef __JBlotGame_H__
#define __JBlotGame_H__

class JBlotPiece;
/***********************************************************************************/
/*  Class:  JBlotGame
/*  Desc:   
/***********************************************************************************/
class JBlotGame : public JLocation
{
    int                 m_BlotsCount;//Pieces count

    std::string         m_SndChange;//Change pieces sound
    std::string         m_SndSelect;//Select piece sound

    JBlotPiece*         m_pSelected1;//Selected piece
    JBlotPiece*         m_pSelected2;//Selected piece

    Vec3                m_PiecePos;

    friend class        JBlotPiece;
public:
    
                        JBlotGame           ();
    virtual void        Render              ();
    virtual void        Init                ();
    void                Shuffle             ();
    void                SwapPieces          (JBlotPiece* p1,JBlotPiece* p2, bool bImmediately);//Swap two pieces

    void                OnSuccess           (){}
    void                OnSelectPiece       ();
    void                SetPiecePos         ( const Vec2& pos ) { m_PiecePos = Vec3( pos.x, pos.y, 0.0f ); }
    bool                AllAtHome           ();
    JBlotPiece*         GetPiece            ( int idx );

    expose( JBlotGame )
    {
        parent( JLocation );
        field ( "SndChange",        m_SndChange     );
        field ( "SndSelect",        m_SndSelect     );
        field ( "PiecePos",         m_PiecePos      );
        method( "Shuffle",          Shuffle         );
        method( "OnSuccess",        OnSuccess       );
        method( "OnSelectPiece",    OnSelectPiece   );
    }
}; // class JBlotGame

#endif //__JBlotGame_H__