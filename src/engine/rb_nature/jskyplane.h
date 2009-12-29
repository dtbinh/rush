/***********************************************************************************/
//  File:   JSkyPlane.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSKYPLANE_H__
#define __JSKYPLANE_H__

/***********************************************************************************/
//  Class:  JSkyPlane
//  Desc:   
/***********************************************************************************/
class JSkyPlane : public JObject
{
    int                 m_PlaneID;
    int                 m_PlaneShaderID;
    std::string         m_PlaneModel;
public:
                        JSkyPlane   ();
    virtual void        Render      ();
    virtual void        Init        ();

    const char*         GetPlaneModel() const { return m_PlaneModel.c_str(); }
    void                SetPlaneModel( const char* name );

    expose( JSkyPlane )
    {
        parent( JObject );
        prop( "PlaneModel", GetPlaneModel, SetPlaneModel );
    }
}; // class JSkyPlane

#endif //__JSKYPLANE_H__