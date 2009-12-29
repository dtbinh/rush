//****************************************************************************/
//  File:   JMove.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JMOVE_H__
#define __JMOVE_H__

//****************************************************************************/
//  Class:  JMove
//  Desc:   Moving animation
//****************************************************************************/
class JMove : public JAnimation
{
    PolyLine2       m_Path;
    bool            m_bClosed;
    bool            m_bSmooth;
    float           m_Speed;

public: 
                    JMove         ();
    virtual void    Render        (); 
    virtual void    PostRender    ();
    virtual void    DrawBounds    ();

    expose( JMove )
    {
        parent(JAnimation);
        field( "Path",   m_Path    );
        field( "Speed",  m_Speed   );
        field( "Closed", m_bClosed );
        field( "Smooth", m_bSmooth );
    }
}; // class JMove

#endif // __JMOVE_H__


