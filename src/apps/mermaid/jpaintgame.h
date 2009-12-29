/***********************************************************************************/
/*  File:   JPaintGame.h
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPaintGame_H__
#define __JPaintGame_H__

/***********************************************************************************/
/*  Class:  JPaintGame
/*  Desc:   
/***********************************************************************************/
class JPaintGame : public JLocation
{
    int                 m_NPainted;
    int                 m_NAreas;
    DWORD               m_CurColor;

public:
                        JPaintGame  ();
    virtual void        Render      ();
    virtual void        Init        ();

    void                Reset       ();

    void                OnSuccess   (){}

    DWORD               GetCurColor () const { return m_CurColor; }
    void                SetCurColor ( DWORD color ) { m_CurColor = color; }

    expose( JPaintGame )
    {
        parent( JLocation );
        field( "NumPainted", m_NPainted );
        field( "NumAreas",   m_NAreas   );
        field( "CurColor",   m_CurColor );

        method( "OnSuccess", OnSuccess );
        method( "Reset", Reset );
    }
}; // class JPaintGame

#endif //__JPaintGame_H__