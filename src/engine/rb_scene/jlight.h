/***********************************************************************************/
//  File:   JLight.h
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JLIGHT_H__
#define __JLIGHT_H__

/***********************************************************************************/
//  Class:  JLight
//  Desc:   
/***********************************************************************************/
class JLight : public JObject
{
public:
    JLight() : m_ID( -1 ) {}

    virtual void PostRender() { m_ID = -1; }

    expose(JLight)
    {
        parent(JObject);
        field( "PerPixel",  m_bPerPixel );
        field( "ID",        m_ID );
    }
protected:
    int         m_ID;
    bool        m_bPerPixel;
}; // class JLight

#endif //__JLIGHT_H__