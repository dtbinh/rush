/***********************************************************************************/
//  File:   JDirLight.h
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JDIRLIGHT_H__
#define __JDIRLIGHT_H__

/***********************************************************************************/
//  Class:  JDirLight
//  Desc:   
/***********************************************************************************/
class JDirLight : public JLight
{
    float           m_Latitude;
    float           m_Longitude;
    DWORD           m_Diffuse;
    DWORD           m_Specular;

public:
                    JDirLight   ();
    virtual void    Render      ();


    expose(JDirLight)
    {
        parent(JLight);
        field( "Diffuse",    m_Diffuse   );
        field( "Specular",   m_Specular  );
        field( "Latitude",   m_Latitude  );
        field( "Longitude",  m_Longitude );
    }
protected:
   

private:

}; // class JDirLight

#endif //__JDIRLIGHT_H__