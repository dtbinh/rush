//****************************************************************************/
//  File:   JUVScroll.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JUVSCROLL_H__
#define __JUVSCROLL_H__

//****************************************************************************/
//  Class:  JUVScroll
//  Desc:   Moving animation
//****************************************************************************/
class JUVScroll : public JAnimation
{
    float           m_USpeed;
    float           m_VSpeed;

public: 
                    JUVScroll     ();
    virtual void    Render        (); 
    virtual void    PostRender    ();

    expose( JUVScroll )
    {
        parent(JAnimation);
        field( "USpeed", m_USpeed  );
        field( "VSpeed", m_VSpeed  );
    }
}; // class JUVScroll

#endif // __JUVSCROLL_H__


