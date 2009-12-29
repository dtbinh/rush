/***********************************************************************************/
//  File:   JAmbient.h
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JAMBIENT_H__
#define __JAMBIENT_H__

/***********************************************************************************/
//  Class:  JAmbient
//  Desc:   
/***********************************************************************************/
class JAmbient : public JLight
{
    DWORD           m_Color;
public:
    JAmbient () : m_Color( 0xFF333333 ) {}
    virtual void    Render   ();

    expose(JAmbient)
    {
        parent(JLight);
        field( "Color", m_Color );
    }
protected:
   
}; // class JAmbient

#endif //__JAMBIENT_H__