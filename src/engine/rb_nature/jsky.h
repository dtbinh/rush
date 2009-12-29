/***********************************************************************************/
//  File:   JSky.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSKY_H__
#define __JSKY_H__

/***********************************************************************************/
//  Class:  JSky
//  Desc:   
/***********************************************************************************/
class JSky : public JObject
{
    int                 m_DomeID;
    int                 m_DomeShaderID;
    std::string         m_DomeModel;
public:
                        JSky  ();
    virtual void        Render();
    virtual void        Init  ();

    const char*         GetDomeModel() const { return m_DomeModel.c_str(); }
    void                SetDomeModel( const char* name );

    expose( JSky )
    {
        parent( JObject );
        prop( "DomeModel", GetDomeModel, SetDomeModel );
    }
}; // class JSky

#endif //__JSKY_H__