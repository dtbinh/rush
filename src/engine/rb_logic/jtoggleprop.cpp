//****************************************************************************/
//  File:  JToggleProp.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JToggleProp.h"

//****************************************************************************/
/*  JToggleProp implementation
//****************************************************************************/
decl_class(JToggleProp);
JToggleProp::JToggleProp()
{
    m_pObject   = NULL;
} // JToggleProp::JToggleProp

void JToggleProp::Init()
{   
    m_pObject = g_pObjectServer->FindObject( m_Object.c_str(), NULL, this );
} // JToggleProp::Init

void JToggleProp::OnPlay()
{
    if (!m_pObject) 
    {
        m_pObject = g_pObjectServer->FindObject( m_Object.c_str(), NULL, this );
    }

    if (m_pObject)
    {
        JString val;
        m_pObject->GetProperty( m_Prop.c_str(), val );
        if (!stricmp( val.c_str(), "true" ) )
        {
            m_pObject->SetProperty( m_Prop.c_str(), "false" );
        }
        else
        {
            m_pObject->SetProperty( m_Prop.c_str(), "true" );
        }
    }
} // JToggleProp::OnPlay




