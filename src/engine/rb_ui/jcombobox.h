/***********************************************************************************/
//  File:   JComboBox.h
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCOMBOBOX_H__
#define __JCOMBOBOX_H__

#include "JWeakRef.h"

class JButton;
class JListBox;
/***********************************************************************************/
//  Class:  JComboBox
//  Desc:   Drop-list selection box
/***********************************************************************************/
class JComboBox : public JWidget
{
    JWeakRef<JButton>   m_pThumb;
    JWeakRef<JListBox>  m_pDropList;

    int                 m_DropHeight;       //  default height of the drop-list
    bool                m_bThumbLeft;       //  whether draw thumb at left part of edit box
    int                 m_LastSelection;    //  last item being selected before drop-down
    int                 m_SelectedItem;     //  currently selected item

public:
                        JComboBox       ();
    virtual void        OnSize          ();
    virtual void        OnFocus         ( bool bEnter );
    virtual void        Init            ();
    virtual void        Render          ();

    int                 GetDropHeight   () const;
    void                SetDropHeight   ( int val );
    bool                IsDropVisible   () const;
    void                SetDropVisible  ( bool bVisible );
    virtual void        OnMouse         ( JMouseEvent& m );

    void                DropDown        ();
    void                DropUp          ();
    int                 GetSelectedItem () const { return m_SelectedItem; }
    void                SelectItem      ( int idx ); 

    expose(JComboBox)
    {
        parent(JWidget);
        prop ( "DropHeight",     GetDropHeight,  SetDropHeight    );
        prop ( "DropVisible",    IsDropVisible,  SetDropVisible   );
        field( "ThumbLeft",      m_bThumbLeft                     );
        method( "DropDown",      DropDown                         );
        method( "DropUp",        DropUp                           );
        prop ( "SelectedItem",   GetSelectedItem, SelectItem      );
    }
}; // class JComboBox

#endif //__JCOMBOBOX_H__