/***********************************************************************************/
//  File:   JLocation.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JLOCATION_H__
#define __JLOCATION_H__

#include "JWeakRef.h"

class JAnimation;
/***********************************************************************************/
//  Class:  JLocation
//  Desc:   
/***********************************************************************************/
class JLocation : public JDialog
{
    JWeakRef<JAnimation> m_pAnmFadeIn;
    float               m_CameraFOV;
    float               m_CameraTilt;  
    float               m_CameraDist;  
    DWORD               m_ShadowColor;
    Vec3                m_LightDir;

public:
                        JLocation   ();
    virtual void        Init        ();
    virtual void        OnClose     ();
    virtual void        OnShow      ();
    virtual void        Render      ();    
    virtual void        PostRender  ();
    expose(JLocation)
    {
        parent(JDialog);
        field( "CameraFOV",     m_CameraFOV  );
        field( "CameraTilt",    m_CameraTilt );
        field( "CameraDist",    m_CameraDist );
        field( "ShadowColor",   m_ShadowColor );
        field( "LightDir",      m_LightDir    );
    }

private:
    void                SetupCamera ();
}; // class JLocation

#endif //__JLOCATION_H__