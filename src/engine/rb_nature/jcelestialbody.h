/***********************************************************************************/
//  File:   JCelestialBody.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCELESTIALBODY_H__
#define __JCELESTIALBODY_H__

/***********************************************************************************/
//  Class:  JCelestialBody
//  Desc:   
/***********************************************************************************/
class JCelestialBody : public JObject
{
    int                 m_ModelID;
    int                 m_ShaderID;
    std::string         m_Model;

    float               m_Latitude;  //  degrees
    float               m_Longitude; //  degrees
    float               m_Distance;
    DWORD               m_Color;
    float               m_Scale;

    //  shader variables' handles
    DWORD               m_hColor;

public:
                        JCelestialBody  ();
    virtual void        Render();
    virtual void        Init  ();

    const char*         GetModel() const { return m_Model.c_str(); }
    void                SetModel( const char* name );

    expose( JCelestialBody )
    {
        parent( JObject );
        prop( "Model", GetModel, SetModel );
        field( "Latitude",   m_Latitude  );
        field( "Longitude",  m_Longitude );
        field( "Color",      m_Color     );
        field( "Scale",      m_Scale     );
    }
}; // class JCelestialBody

#endif //__JCELESTIALBODY_H__