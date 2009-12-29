//****************************************************************************/
//  File:  JSetVar.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JVar.h"
#include "JSetVar.h"

//****************************************************************************/
/*  JSetVar implementation
//****************************************************************************/
decl_class(JSetVar);
JSetVar::JSetVar()
{
    m_pVar = NULL;
    m_Operation = SetOp_Set;
} // JSetVar::JSetVar

void JSetVar::OnPlay()
{   
    if (!m_pVar)
    {
        Init();
    }
    if (m_pVar)
    {
        float fValS = 0.0f;
        float fValD = 0.0f;
        if (!m_pVar->GetFloatValue( fValD ) || !cast( fValS, m_Value ))
        {
            m_pVar->SetValue( m_Value.c_str() );
        }
        else
        {
            switch (m_Operation)
            {
            case SetOp_Set:
                fValD = fValS;
                break;         
            case SetOp_Add: 
                fValD += fValS;
                break;
            case SetOp_Subtract:
                fValD -= fValS;
                break;
            case SetOp_Multiply:
                fValD *= fValS;
                break;
            case SetOp_SetIfGreater:
                if (fValD > fValS) fValD = fValS;
                break;
            case SetOp_SetIfLess:
                if (fValD < fValS) fValD = fValS;
                break;
            }

            static JString sVal;
            cast( sVal, fValD );
            m_pVar->SetValue( sVal.c_str() );
        }
    }
} // JSetVar::OnPlay

void JSetVar::Init()
{   
    m_pVar = g_pObjectServer->FindObject<JVar>( m_Var.c_str(), this );
} // JSetVar::Init




