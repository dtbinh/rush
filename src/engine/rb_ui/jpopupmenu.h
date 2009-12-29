//****************************************************************************/
//  File:   JPopupMenu.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JPOPUPMENU_H__
#define __JPOPUPMENU_H__

//****************************************************************************/
//  Class:  JPopupMenu
//  Desc:    
//****************************************************************************/
class JPopupMenu : public JListBox
{
public: 
                    JPopupMenu     ();
    virtual void    Render         (); 

    expose( JPopupMenu )
    {
        parent(JListBox);
    }
}; // class JPopupMenu

#endif // __JPOPUPMENU_H__


