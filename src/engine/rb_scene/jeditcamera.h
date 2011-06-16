/***********************************************************************************/
//  File:   JEditCamera.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JEDITCAMERA_H__
#define __JEDITCAMERA_H__

#include "jcameracontroller.h"
#include "timer.h"

/***********************************************************************************/
//  Class:  JEditCamera
//  Desc:
/***********************************************************************************/
class JEditCamera : public JCameraController
{
public:

    enum ViewMode
    {
        ViewMode_Unknown    = 0,
        ViewMode_Idle       = 1,
        ViewMode_Rolling    = 2,
        ViewMode_Zooming    = 3,
        ViewMode_Panning    = 4,
        ViewMode_Homing     = 5
    }; // enum ViewMode

                    JEditCamera();
    virtual void    Render();
    virtual void    Init();
    virtual void    OnMouse( JMouseEvent& e );
    virtual void    OnWheel( JWheelEvent& e );

    void            Home();

    expose( JEditCamera )
    {
        parent( JCameraController );
        field( "Roll",          m_Roll              );
        field( "Tilt",          m_Tilt              );
        field( "Zoom",          m_Zoom              );

        field( "HomeRoll",      m_HomeRoll          );
        field( "HomeTilt",      m_HomeTilt          );
        field( "HomeZoom",      m_HomeZoom          );

        field( "MaxRoll",       m_MaxRoll           );
        field( "MaxTilt",       m_MaxTilt           );
        field( "MaxZoom",       m_MaxZoom           );

        field( "MinRoll",       m_MinRoll           );
        field( "MinTilt",       m_MinTilt           );
        field( "MinZoom",       m_MinZoom           );

        field( "RollSpeed",     m_RollSpeed         );
        field( "TiltSpeed",     m_TiltSpeed         );
        field( "ZoomSpeed",     m_ZoomSpeed         );
        field( "FocusDist",     m_FocusDist         );
        field( "UseControlKey", m_bUseControlKey    );
        field( "LookAt",        m_LookAt            );

        method( "Home",         Home );
    }

private:
    ViewMode        m_Mode;

    //  home camera parameters
    float           m_HomeRoll;
    float           m_HomeTilt;
    float           m_HomeZoom;

    //  current camera parameters
    float           m_Roll;
    float           m_Tilt;
    float           m_Zoom;

    //  limits
    float           m_MaxRoll;
    float           m_MaxTilt;
    float           m_MaxZoom;

    float           m_MinRoll;
    float           m_MinTilt;
    float           m_MinZoom;

    //  camera parameters speed
    float           m_RollSpeed;
    float           m_TiltSpeed;
    float           m_ZoomSpeed;

    //  camera parameters delta
    float           m_RollDelta;
    float           m_TiltDelta;
    float           m_ZoomDelta;
    Vec3            m_LookAtDelta;

    float           m_FocusDist;
    Vec3            m_LookAt;

    Vec2            m_MousePos;
    Timer           m_Timer;

    bool            m_bUseControlKey;
}; // class JEditCamera

#endif //__JEDITCAMERA_H__
