//****************************************************************************/
//  File:  JSetProp.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JSetProp.h"

//****************************************************************************/
/*  JSetProp implementation
//****************************************************************************/
decl_class(JSetProp);
JSetProp::JSetProp()
{
    m_pObject   = NULL;
    m_pSrcObject = NULL;
    m_MinVal    = -1;
    m_MaxVal    = -1;
}  

void JSetProp::Init()
{   
    m_pObject    = g_pObjectServer->FindObject( m_Object.c_str(), NULL, this );
    m_pSrcObject = g_pObjectServer->FindObject( m_SrcObject.c_str(), NULL, this );
}  

void JSetProp::OnPlay()
{
    if (!m_pObject) 
    {
        m_pObject = g_pObjectServer->FindObject( m_Object.c_str(), NULL, this );
    }

    if (!m_pSrcObject && m_SrcObject.size()) 
    {
        m_pSrcObject = g_pObjectServer->FindObject( m_SrcObject.c_str(), NULL, this );
    }

    if (m_pObject)
    {
        if (m_MinVal != -1 && m_MaxVal != -1)
        {
            cast( m_Value, m_MinVal + rand()%(m_MaxVal - m_MinVal));
        }
        
        if (m_pSrcObject)
        {
            m_pSrcObject->GetProperty( m_SrcProp.c_str(), m_Value, GetTag() );
        }

        m_pObject->SetProperty( m_Prop.c_str(), m_Value.c_str(), GetTag() );
    }
}  




