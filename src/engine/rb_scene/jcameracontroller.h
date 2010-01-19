/***********************************************************************************/
//  File:   JCameraController.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCAMERACONTROLLER_H__
#define __JCAMERACONTROLLER_H__

#include "jweakref.h"

class JCamera;
/***********************************************************************************/
//  Class:  JCameraController
//  Desc:
/***********************************************************************************/
class JCameraController : public JWidget
{
	JWeakRef<JCamera>		m_pCamera;
	JString					m_CameraName;

public:
							JCameraController();
	const char*				GetCameraName() const { return m_CameraName.c_str(); }
	void					SetCameraName( const char* name );
	virtual void			Init();
	JCamera*				GetCamera() const { return m_pCamera; }

    expose( JCameraController )
    {
        parent( JWidget );
		prop( "Camera", GetCameraName, SetCameraName );
    }
}; // class JCameraController

#endif //__JCAMERACONTROLLER_H__
