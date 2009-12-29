/***********************************************************************************/
//  File:   JStringSelector.h
//  Date:   09.11.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSTRINGSELECTOR_H__
#define __JSTRINGSELECTOR_H__

/***********************************************************************************/
//  Class:  JStringSelector
//  Desc:   
/***********************************************************************************/
class JStringSelector : public JObject
{
    JStringList         m_Strings;
    int                 m_SelectedItem;

public:
    JStringSelector() : m_SelectedItem(-1){}

    JStringList         GetItems() const { return m_Strings; }
    void                SetItems( JStringList items );

    int                 GetSelectedItemIdx() const { return m_SelectedItem; }
    
    const char*         GetSelectedItem() const; 
    void                SetSelectedItemIdx( int val );
    bool                CanSelectNext() const;
    bool                CanSelectPrevious() const;
    void                SelectNext();
    void                SelectPrevious();

    expose(JStringSelector)
    {
        parent(JObject);
        prop( "Items", GetItems, SetItems );
        rprop( "SelectedItem", GetSelectedItem );
        prop( "SelectedItemIdx", GetSelectedItemIdx, SetSelectedItemIdx );
        rprop( "CanSelectNext", CanSelectNext );
        rprop( "CanSelectPrevious", CanSelectPrevious );
        method( "SelectNext", SelectNext );
        method( "SelectPrevious", SelectPrevious );
    }
}; // class JStringSelector

#endif //__JSTRINGSELECTOR_H__