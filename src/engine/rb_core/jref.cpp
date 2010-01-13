//****************************************************************************/
//  File:  JRef.cpp
//****************************************************************************/
#include "precompile.h"
#include "jobject.h"
#include "jref.h"

//****************************************************************************/
/*  JRef implementation
//****************************************************************************/
decl_class(JRef);
void JRef::Render()
{
    if (!m_pObject)
    {
        m_pObject = g_pObjectServer->FindObject<JObject>( m_Object.c_str(), this );
    }
    else
    {
        m_pObject->RenderTree();
    }
} // JRef::Render

void JRef::Init()
{
    SetObject( m_Object.c_str() );
} // JRef::Init

void JRef::SetObject( const char* name )
{
    m_Object = name;
    m_pObject = g_pObjectServer->FindObject<JObject>( name, this );
} // JRef::SetObject