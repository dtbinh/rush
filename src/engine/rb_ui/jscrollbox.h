/***********************************************************************************/
//  File:   JScrollBox.h
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSCROLLBOX_H__
#define __JSCROLLBOX_H__

class JScrollBar;
/***********************************************************************************/
//  Class:  JScrollBox
//  Desc:   
/***********************************************************************************/
class JScrollBox : public JWidget
{
    JScrollBar*         m_pVBar;
    JScrollBar*         m_pHBar;

public:
                        JScrollBox();
    virtual void        Init();

    expose(JScrollBox)
    {
        parent(JWidget);
    }
}; // class JScrollBox

#endif //__JSCROLLBOX_H__