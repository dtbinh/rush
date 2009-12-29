/***********************************************************************************/
//  File:   JQuestState.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JQUESTSTATE_H__
#define __JQUESTSTATE_H__

/***********************************************************************************/
//  Class:  JQuestState
//  Desc:   Persistent part of the quest game
/***********************************************************************************/
class JQuestState : public JObject
{
    std::string         m_CharName;

public:
                        JQuestState ();
    virtual void        Init        ();

    const char*         GetCharName () const;

    expose( JQuestState )
    {
        parent( JObject );
        field( "CharName", m_CharName );
    }
}; // class JQuestState

#endif //__JQUESTSTATE_H__