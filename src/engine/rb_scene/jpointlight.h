/***********************************************************************************/
//  File:   JPointLight.h
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPOINTLIGHT_H__
#define __JPOINTLIGHT_H__

/***********************************************************************************/
//  Class:  JPointLight
//  Desc:   
/***********************************************************************************/
class JPointLight : public JLight
{
    Vec3            m_Position;
    float           m_Radius;
    float           m_Falloff;
    uint32_t           m_Diffuse;
    uint32_t           m_Specular;
public:
                    JPointLight ();
    virtual void    Render      ();

    expose(JPointLight)
    {
        parent(JLight);
        field( "Position",  m_Position   );
        field( "Radius",    m_Radius     );
        field( "Falloff",   m_Falloff    );
        field( "Diffuse",   m_Diffuse    );
        field( "Specular",  m_Specular   );
    }

protected:
   
}; // class JPointLight

#endif //__JPOINTLIGHT_H__