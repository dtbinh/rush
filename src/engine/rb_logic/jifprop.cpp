//****************************************************************************/
//  File:  JIfProp.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JIfVar.h"
#include "JIfProp.h"

//****************************************************************************/
/*  JIfProp implementation
//****************************************************************************/
decl_class(JIfProp);
JIfProp::JIfProp()
{
    m_pObject       = NULL;
    m_Cmp           = Cmp_Equal;
    m_bStatic       = false;
    m_bAlwaysTrue   = false;
} // JIfProp::JIfProp

void JIfProp::Render()
{
    if (!m_pObject) Init();
    if (!m_pObject) return;
    if (m_bStatic && !m_bAlwaysTrue)
    {
        JString val;
        if (m_pObject->GetProperty( m_Prop.c_str(), val ))
        {
            SetVisible( IsTrue( val.c_str(), m_Value.c_str(), m_Cmp ) );
        }
    }
} // JIfProp::Render

void JIfProp::OnPlay()
{
    if (!m_pObject) Init();
    if (m_pObject)
    {
        JString val;
        if (m_pObject->GetProperty( m_Prop.c_str(), val ))
        {
            if (IsTrue( val.c_str(), m_Value.c_str(), m_Cmp ) ||
                m_bAlwaysTrue)
            {
                return;
            }
        }
    }
} // JIfProp::OnPlay

void JIfProp::Init()
{   
    m_pObject = g_pObjectServer->FindObject( m_Object.c_str(), NULL, this );

    //  check whether we have connected properly
    static JString path;
    if (!m_pObject)
    {
        GetPath( path );
        rlog.err( "JIfProp at path <%s>: Could not find object <%s>", path.c_str(), m_Object.c_str() );
    }
    if (!GetProperty( m_Prop.c_str(), path ))
    {
        GetPath( path );
        rlog.err( "JIfProp at path <%s>: Could not find property <%s> for object <%s>.", 
                    path.c_str(), m_Prop.c_str(), m_Object.c_str() );
    }
} // JIfProp::Init




