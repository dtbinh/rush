//****************************************************************************/
//  File:  JSwapProp.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JSwapProp.h"

//****************************************************************************/
/*  JSwapProp implementation
//****************************************************************************/
decl_class(JSwapProp);
JSwapProp::JSwapProp()
{
    m_pObject1   = NULL;
    m_pObject2   = NULL;
} // JSwapProp::JSwapProp

void JSwapProp::Init()
{   
    m_pObject1 = g_pObjectServer->FindObject( m_Object1.c_str(), NULL, this );
    m_pObject2 = g_pObjectServer->FindObject( m_Object2.c_str(), NULL, this );
} // JSwapProp::Init

void JSwapProp::OnPlay()
{
    if (!m_pObject1 || !m_pObject2) 
    {
        Init();
    }

    if (m_pObject1 && m_pObject2)
    {
        JString val1, val2;
        m_pObject1->GetProperty( m_Prop1.c_str(), val1 );
        m_pObject2->GetProperty( m_Prop2.c_str(), val2 );
        
        m_pObject1->SetProperty( m_Prop1.c_str(), val2.c_str() );
        m_pObject2->SetProperty( m_Prop2.c_str(), val1.c_str() );
    }
} // JSwapProp::OnPlay




