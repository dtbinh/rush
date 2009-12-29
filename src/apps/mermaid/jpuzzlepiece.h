/***********************************************************************************/
/*  File:   JPuzzlePiece.h
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPuzzlePiece_H__
#define __JPuzzlePiece_H__

class JPuzzleGame;
/***********************************************************************************/
/*  Class:  JPuzzlePiece
/*  Desc:   
/***********************************************************************************/
class JPuzzlePiece : public JWidget
{
    Vec2                m_HomePos;
    float               m_HomeDir;

    Vec2                m_InitPos;
    float               m_InitDir;

    float               m_Scale;

    bool                m_bAtHome;

    JPuzzleGame*        m_pPuzzle;


public:
                        JPuzzlePiece    (); 
    virtual void        Render          ();
    virtual void        Init            ();
    virtual void        OnDrag          ( JDragEvent& e );
    virtual void        DrawBounds      ();
    virtual bool        PtIn            ( int px, int py ) const;

    bool                IsAtHome        () const { return m_bAtHome; }
    void                MoveToInitPos   ();

    expose( JPuzzlePiece )
    {
        parent( JWidget );
        field( "HomePos",       m_HomePos       );
        field( "HomeDir",       m_HomeDir       );
        field( "InitPos",       m_InitPos       );
        field( "InitDir",       m_InitDir       );
        field( "AtHome",        m_bAtHome       );
        field( "Scale",         m_Scale         );
    }
}; // class JPuzzlePiece

#endif //__JPuzzlePiece_H__