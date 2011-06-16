/***********************************************************************************/
/*  File:   JSlideGame.h
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSlideGame_H__
#define __JSlideGame_H__

#include "JMathTypeCast.h"
#include "JSlidePiece.h"


/***********************************************************************************/
/*  Class:  JSlideGame
/*  Desc:   
/***********************************************************************************/
class JSlideGame : public JLocation
{
    int                 m_CatchID;          //  ID of piece which is right in this cycle
    int                 m_NumToCatch;       //  number of pieces of given ID to catch
    int                 m_MinToCatch;       
    int                 m_MaxToCatch;       
    int                 m_MaxIncorrect;     //  maximal number of incorrect pieces before restart
    int                 m_NumCaught;        //  number of pieces already caught
    int                 m_NumCaughtWrong;   //  number of incorrectly caught pieces

    int                 m_TopLine;          //  Y coordinate from which pieces start to fall
    int                 m_BottomLine;       //  Y coordinate at which pieces stop to fall
    int                 m_LeftLine;         //  X coordinate from which pieces random create
    int                 m_RightLine;        //  X coordinate to which pieces random create

    int                 m_NumCycles;        //  Count of cycles
    int                 m_MinSpeed;         //  Minimum of random speed
    int                 m_MaxSpeed;         //  Maximum of random speed
    int                 m_NumRightCycles;   //  Count of right cycles
    int                 m_CurRightCycle;    //  Count of right cycles

    bool                m_bRandomCatchID;   //  on each cycle randomly change id of the piece to catch

    std::string         m_SndTakeRight;     //  Right take piece sound
    std::string         m_SndTakeWrong;     //  Wrong take piece sound

    int                 m_NumID;            //  how much different kinds of pieces we have

    friend class        JSlidePiece;
public:
    
                        JSlideGame          ();
    virtual void        Render              ();
    virtual void        Init                ();
    void                StartCycle          ();
    void                StartGame           ();

    void                OnSuccess           (){}
    void                OnCycleEnd          (){}
    void                OnTakeWrong         ();
    void                OnTakeRight         ();

    int                 GetCatchID          () const { return m_CatchID; }
    int                 GetLeftToCatch      () const;
    
    expose( JSlideGame )
    {
        parent( JLocation );
        field ( "SndTakeRight",     m_SndTakeRight      );
        field ( "SndTakeWrong",     m_SndTakeWrong      );
        field ( "NumCycles",        m_NumCycles         );
        field ( "NumRightCycles",   m_NumRightCycles    );
        field ( "CatchID",          m_CatchID           );
        field ( "MaxIncorrect",     m_MaxIncorrect      );
        field ( "NumCaughtWrong",   m_NumCaughtWrong    );
        field ( "NumToCatch",       m_NumToCatch        );
        field ( "MinToCatch",       m_MinToCatch        );
        field ( "MaxToCatch",       m_MaxToCatch        );
        field ( "TopLine",          m_TopLine           );
        field ( "BottomLine",       m_BottomLine        );
        field ( "LeftLine",         m_LeftLine          );
        field ( "RightLine",        m_RightLine         );
        field ( "MinSpeed",         m_MinSpeed          );
        field ( "MaxSpeed",         m_MaxSpeed          );
        field ( "RandomCatchID",    m_bRandomCatchID    );
        rprop ( "LeftToCatch",      GetLeftToCatch      );
        method( "StartCycle",       StartCycle          );
        method( "OnSuccess",        OnSuccess           );
        method( "OnCycleEnd",       OnCycleEnd          );
        method( "OnTakeRight",      OnTakeRight         );
        method( "OnTakeWrong",      OnTakeWrong         );
        method( "StartGame",        StartGame           );
    }
}; // class JSlideGame

#endif //__JSlideGame_H__