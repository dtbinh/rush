//****************************************************************************/
//  File:   JPanel.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JPANEL_H__
#define __JPANEL_H__

//****************************************************************************/
//  Class:  JPanel
//  Desc:   Serves as visually drawn other widgets' container
//****************************************************************************/
class JPanel : public JWidget
{
public: 
                    JPanel          ();
    virtual void    Render          (); 
    virtual void    PostRender      (); 
    virtual void    OnMouse         ( JMouseEvent& e );
    virtual void    OnKey           ( JKeyEvent& e );

    void            OnLBDbl         (){}

    expose( JPanel )
    {
        parent(JWidget);
        method( "OnLBDbl",          OnLBDbl           );
    }
}; // class JPanel

#endif // __JPANEL_H__


