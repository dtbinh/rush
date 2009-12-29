//****************************************************************************/
//  File:   JCheckBox.h
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JCHECKBOX_H__
#define __JCHECKBOX_H__

//****************************************************************************/
//  Class:  JCheckBox
//  Desc:   Boolean button
//****************************************************************************/
class JCheckBox : public JButton
{
public:
                    JCheckBox       ();
    virtual void    Render          (); 
    void            Check           ( bool bCheck = true );
    bool            IsChecked       () const { return IsPressed(); }

    expose( JCheckBox )
    {
        parent(JButton);
    }
}; // class JCheckBox

#endif // __JCHECKBOX_H__


