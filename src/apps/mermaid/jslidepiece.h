/***********************************************************************************/
/*  File:   JSlidePiece.h
/*  Date:   07.08.2006
/*  Author: Juriy Skolyar
/***********************************************************************************/
#ifndef __JSLIDEPIECE_H__
#define __JSLIDEPIECE_H__

class JSlideGame;
/***********************************************************************************/
/*  Class:  JSlidePiece
/*  Desc:   
/***********************************************************************************/
class JSlidePiece : public JWidget
{
    double              m_StartTime;
    float               m_Speed;
    Vec2                m_Pos;

    JSlideGame*         m_pSlideGame;

    friend class        JSlideGame;
public:
                        JSlidePiece      (); 
    virtual void        Render          ();
    virtual void        Init            ();
    virtual void        DrawBounds      ();
    virtual bool        PtIn            ( int px, int py ) const;
    virtual void        OnMouse         ( JMouseEvent& e );
    void                StartFalling    ();
    void                SetSpeed        ( float speed ) { m_Speed = speed; }

    expose( JSlidePiece )
    {
        parent( JWidget );
    }
}; // class JSlidePiece

#endif //__JSLIDEPIECE_H__