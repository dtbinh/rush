/***********************************************************************************/
/*  File:   JMazeControl.h
/*  Date:   29.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JMAZECONTROL_H__
#define __JMAZECONTROL_H__

/***********************************************************************************/
/*  Class:  JMazeControl
/*  Desc:   Used to control character movement inside the maze mini-game
/***********************************************************************************/
class JMazeControl : public JCharControl
{
    std::string         m_Start;
    std::string         m_End;

    JWaypoint*          m_pStart;
    JWaypoint*          m_pEnd;

public:
                        JMazeControl    ();
    void                SetStartPoint   ( const char* name );
    void                SetEndPoint     ( const char* name );
    const char*         GetStartPoint   () const { return m_Start.c_str(); }
    const char*         GetEndPoint     () const { return m_End.c_str();   }

    void                Init            ();
    void                Start           ();
    void                OnFoundExit     ();

    void                Render          ();

    bool                MoveTo          ( float dir );

    virtual void        OnMouse         ( JMouseEvent& e );
    virtual void        OnKey           ( JKeyEvent& e   );

    expose( JMazeControl )
    {
        parent( JCharControl );
        prop  ( "StartPoint", GetStartPoint, SetStartPoint );
        prop  ( "EndPoint",   GetEndPoint,   SetEndPoint   );
        method( "Start",      Start         );
        method( "FoundExit",  OnFoundExit   );
    }
}; // class JMazeControl

#endif //__JMAZECONTROL_H__