/***********************************************************************************/
//  File:   JMoveChar.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JMOVECHAR_H__
#define __JMOVECHAR_H__

/***********************************************************************************/
//  Class:  JMoveChar
//  Desc:   
/***********************************************************************************/
class JMoveChar : public JAnimation
{
    bool                m_bImmediately;
    bool                m_bTeleport;
    std::string         m_CharControl;
    Vec3                m_Pos;
    float               m_Dir;

    JCharControl*       m_pCharControl;

public:
                        JMoveChar   ();
    virtual void        OnPlay      ();
    virtual void        OnStop      ();
    virtual void        Init        ();
    virtual void        Render      ();

    expose( JMoveChar )
    {
        parent( JAnimation );
        field   ( "Immediately",  m_bImmediately  );
        field   ( "Teleport",     m_bTeleport     );
        field   ( "CharControl",  m_CharControl   );
        field   ( "Position",     m_Pos           );
        field   ( "Dir",          m_Dir           );
    }
}; // class JMoveChar

#endif //__JMOVECHAR_H__