/*****************************************************************************/
//  File:  JCall.cpp
//  Desc:  
/*****************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JCall.h"

/*****************************************************************************/
/*  JCall implementation
/*****************************************************************************/
decl_class(JCall);
JCall::JCall()
{
    m_pObject = NULL;
} // JCall::JCall

void JCall::Init()
{   
    m_pObject = g_pObjectServer->FindObject( m_Object.c_str(), NULL, this );

    //  check whether we have connected properly
    static JString path;
    if (!m_pObject)
    {
        GetPath( path );
        rlog.err( "JCall at path <%s>: Could not find object <%s>", path.c_str(), m_Object.c_str() );
        return;
    }
    if (g_pObjectServer->FindClassAttr( m_pObject->ClassName(), m_Method.c_str() ) == NULL)
    {
        GetPath( path );
        rlog.err( "JCall at path <%s>: Could not find method <%s> for object <%s>.", 
                    path.c_str(), m_Method.c_str(), m_Object.c_str() );
    }
} // JCall::Init

void JCall::OnPlay()
{
    if (!m_pObject)
    {
        Init();
    }
    if (m_pObject)
    {
        m_pObject->CallMethod( m_Method.c_str(), GetTag() );
    }
} // JCall::OnPlay




