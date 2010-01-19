/***********************************************************************************/
//  File:   JAmbient.cpp
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "jlight.h"
#include "jambient.h"

/***********************************************************************************/
/*  JAmbient implementation
/***********************************************************************************/
decl_class(JAmbient);
void JAmbient::Render()
{
    g_pRenderServer->SetAmbient( m_Color );
}

