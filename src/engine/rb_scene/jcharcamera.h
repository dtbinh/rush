/***********************************************************************************/
//  File:   JCharCamera.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCHARCAMERA_H__
#define __JCHARCAMERA_H__

#include "jcameracontroller.h"
#include "timer.h"

class JModelInstance;
/***********************************************************************************/
//  Class:  JCharCamera
//  Desc:
/***********************************************************************************/
class JCharCamera : public JCameraController
{
    Timer                       m_Timer;

    //  instance of the character model
    JString                     m_ModelInstance;

    //  name of the bone to track
    JString                     m_TrackBone;

    //  track point, relative to the bone being tracked
    Vec3                        m_TrackPoint;

    //  distance from camera to the track point
    float                       m_TrackDist;

    //  camera rotation speed
    float                       m_RotateSpeed;

    //  current camera heading (rotation angle in horizontal plane)
    float                       m_Heading;

    //  current camera pitch (rotation angle in vertical plane)
    float                       m_Pitch;

    //  camera rotation limits
    float                       m_MinPitch;
    float                       m_MaxPitch;
    float                       m_MinHeading;
    float                       m_MaxHeading;

    JWeakRef<JModelInstance>    m_pModelInst;

    float                       m_HeadingDelta;
    float                       m_PitchDelta;

    Vec2                        m_MousePos;

public:

                    JCharCamera ();
    virtual void    Render      ();
    virtual void    OnMouse     ( JMouseEvent& e );
    virtual void    OnWheel     ( JWheelEvent& e );

    virtual void    Init        ();

    expose( JCharCamera )
    {
        parent( JCameraController );
        field( "ModelInstance",  m_ModelInstance    );
        field( "TrackBone",      m_TrackBone        );
        field( "TrackPoint",     m_TrackPoint       );
        field( "TrackDist",      m_TrackDist        );
        field( "RotateSpeed",    m_RotateSpeed      );
        field( "Heading",        m_Heading          );
        field( "Pitch",          m_Pitch            );
        field( "MinPitch",       m_MinPitch         );
        field( "MaxPitch",       m_MaxPitch         );
        field( "MinHeading",     m_MinHeading       );
        field( "MaxHeading",     m_MaxHeading       );
    }

private:
}; // class JCharCamera

#endif //__JCHARCAMERA_H__
