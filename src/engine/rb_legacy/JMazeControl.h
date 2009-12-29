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
public:
                        JMazeControl    ();
    void                Init            ();
    void                Render          ();

    bool                MoveTo          ( float dir );
    virtual void        OnMouse         ( JMouseEvent& e );
    virtual void        OnKey           ( JKeyEvent& e   );

    expose( JMazeControl )
    {
        parent( JCharControl );
    }
}; // class JMazeControl

#endif //__JMAZECONTROL_H__