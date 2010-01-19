/***********************************************************************************/
//  File:   JCameraController.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "jcameracontroller.h"
#include "jcamera.h"

/***********************************************************************************/
/*  JCameraController implementation
/***********************************************************************************/
decl_class(JCameraController);
JCameraController::JCameraController()
{
	m_pCamera = NULL;

    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );

    SetLocalExt( Frame( 0, 0, 1024, 768 ) );
}

void JCameraController::SetCameraName( const char* name )
{
	m_pCamera = g_pObjectServer->FindObject<JCamera>( name, this );
	m_CameraName = name;
}

void JCameraController::Init()
{
	m_pCamera = g_pObjectServer->FindObject<JCamera>( GetCameraName(), this );
}
