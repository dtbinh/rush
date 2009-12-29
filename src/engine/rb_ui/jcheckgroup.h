//****************************************************************************/
//  File:   JCheckGroup.h
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JCHECKGROUP_H__
#define __JCHECKGROUP_H__

class JCheckBox;
//****************************************************************************/
//  Class:  JCheckGroup
//  Desc:   Groups checkboxes, so only one can be active at once
//****************************************************************************/
class JCheckGroup : public JWidget
{
    int             m_ActiveIdx;
public:
                    JCheckGroup     ();
    virtual void    Render          (); 

    void            SetActiveIdx    ( int idx );
    int             GetActiveIdx    () const { return m_ActiveIdx; }

    expose( JCheckGroup )
    {
        parent(JWidget);
        prop( "ActiveIdx", GetActiveIdx, SetActiveIdx );
    }
}; // class JCheckGroup

#endif // __JCHECKGROUP_H__


