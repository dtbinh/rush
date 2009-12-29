//****************************************************************************/
//  File:   JSwitch.h
//  Date:   22.05.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JSWITCH_H__
#define __JSWITCH_H__

//****************************************************************************/
//  Class:  JSwitch
//  Desc:   
//****************************************************************************/
class JSwitch : public JWidget
{
    int             m_ActiveIdx;
public: 
                    JSwitch         ();

    JObject*        GetActiveChild  () const;
    void            SetActive       ( const char* name );
    const char*     GetActive       () const 
    {
        JObject* pActive = GetActiveChild();
        if (!pActive) return "";
        return pActive->GetName();
    }

    int             GetActiveIdx    () const { return m_ActiveIdx; }
    void            SetActiveIdx    ( int idx );

    expose( JSwitch )
    {
        parent(JWidget);
        prop ( "ActiveIdx", GetActiveIdx, SetActiveIdx );
        prop ( "Active", GetActive, SetActive );
    }
}; // class JSwitch

#endif // __JSWITCH_H__


