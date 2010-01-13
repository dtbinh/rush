//****************************************************************************/
//  File:  JMetaClass.cpp
//****************************************************************************/
#include "precompile.h"
#include "jobjectserver.h"
#include "jmetaclass.h"

//****************************************************************************/
/*	JMetaClass implementation
//****************************************************************************/
JClassAttr* JMetaClass::FindAttr( const char* name )
{
    int nAttr = m_Attr.size();
    for (int i = 0; i < nAttr; i++)
    {
        if (m_Attr[i]->HasName( name )) 
        {
            return m_Attr[i];
        }
    }
    if (!m_pParent) return NULL;
    return m_pParent->FindAttr( name );
} // JMetaClass::FindAttr

void JMetaClass::Expose( JObject* pObject ) 
{   
    pObject->Expose( *this );
    
    int nAttr = GetNAttr();
    JString s_AttrKey;
    for (int i = 0; i < nAttr; i++)
    {
        s_AttrKey.set( m_Name.c_str() );
        JClassAttr* pAttr = GetAttr( i );
        s_AttrKey += pAttr->GetName();
        s_AttrKey.to_lower();
        g_pObjectServer->RegAttr( s_AttrKey, pAttr );
    }
} // JMetaClass::Expose



