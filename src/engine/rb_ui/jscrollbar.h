/***********************************************************************************/
//  File:   JScrollBar.h
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSCROLLBAR_H__
#define __JSCROLLBAR_H__

class JButton;
class JSlider;
/***********************************************************************************/
//  Class:  JScrollBar
//  Desc:   
/***********************************************************************************/
class JScrollBar : public JWidget
{
    JButton*            m_pUp;
    JButton*            m_pDown;
    JButton*            m_pSlider;
    JButton*            m_pPage;

    bool                m_bVertical;

public:
                        JScrollBar  ();
    virtual void        Init        ();
    virtual void        OnSize      ();

    expose(JScrollBar)
    {
       parent(JWidget);
       field( "Vertical", m_bVertical );
    }
}; // class JScrollBar

#endif //__JSCROLLBAR_H__