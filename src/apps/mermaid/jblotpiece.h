/***********************************************************************************/
/*  File:   JBlotPiece.h
/*  Date:   01.08.2006
/*  Author: Juriy Skolyar
/***********************************************************************************/
#ifndef __JBlotPiece_H__
#define __JBlotPiece_H__

class JBlotGame;
/***********************************************************************************/
/*  Class:  JBlotPiece
/*  Desc:   
/***********************************************************************************/
class JBlotPiece : public JRipplePicture
{
    Vec2                m_HomePos;      //  right position of the piece
	float               m_Scale;        //  selected piece scale
    bool                m_bRipple;      //  whether piece is rippling  

    Vec2				m_MovePos;      //  position to move to
	double				m_StartMoveTime;//  time when it started to move
	bool				m_bSelected;    //  whether the piece is selected  

	bool                m_bAtHome;
    JBlotGame*			m_pBlot;

    bool                m_bMoving;

    friend class		JBlotGame;

public:
                        JBlotPiece      (); 
    virtual void        Render          ();
    virtual void        PostRender      ();
    virtual void        Init            ();
    virtual void        DrawBounds      ();
    virtual bool        PtIn            ( int px, int py ) const;
	virtual void        OnMouse         ( JMouseEvent& e );

    bool                IsAtHome        () const { return m_bAtHome; }
	void				MoveTo			( Vec2 pos, bool bImmediately );
    void                MoveToHomePos   ();

    expose( JBlotPiece )
    {
        parent( JRipplePicture );
        field( "HomePos",       m_HomePos       );
        field( "InitPos",       m_HomePos       );
        field( "MovePos",       m_MovePos       );
        field( "Selected",      m_bSelected     );
        field( "Ripple",        m_bRipple       );
        field( "AtHome",        m_bAtHome       );
        field( "SelScale",      m_Scale         );
    }
}; // class JBlotPiece

#endif //__JBlotPiece_H__