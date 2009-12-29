//****************************************************************************/
//  File:  JIfVar.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JVar.h"
#include "JIfVar.h"

bool IsTrue( const char* a, const char* b, JCmp cmp )
{
    float da, db;
    if (sscanf( a, "%f", &da ) != 0 &&
        sscanf( b, "%f", &db ) != 0)
    {
        switch (cmp)
        {
        case Cmp_Equal:     return da == db;
        case Cmp_NotEqual:  return da != db;
        case Cmp_Less:      return da <  db;
        case Cmp_Greater:   return da >  db;
        case Cmp_LEqual:    return da <= db;
        case Cmp_GEqual:    return da >= db;
        default:
            return false;
        }
    }

    int bStrEq = stricmp( a, b );
    if (cmp == Cmp_Equal)    return (bStrEq == 0);
    if (cmp == Cmp_NotEqual) return (bStrEq != 0);
    return false;
} // IsTrue 

//****************************************************************************/
/*  JIfVar implementation
//****************************************************************************/
decl_class(JIfVar);
JIfVar::JIfVar()
{
    m_pVar          = NULL;
    m_Cmp           = Cmp_Equal;
    m_bAlwaysTrue   = false;
    SetVisible( true );
} // JIfVar::JIfVar

void JIfVar::Init()
{   
    m_pVar = g_pObjectServer->FindObject<JVar>( m_Var.c_str(), this );

    //  check whether we have connected properly
    static JString path;
    if (!m_pVar)
    {
        GetPath( path );
        rlog.err( "JIfVar at path <%s>: Could not find variable <%s>", path.c_str(), m_Var.c_str() );
    }
} // JIfVar::Init

void JIfVar::Render()
{
    JAnimation::Render();
    if (!m_pVar)
    {
        Init();
    }

    if (!m_pVar) return;

    bool bTrue = true; 
    if (!m_bAlwaysTrue)
    {
        bTrue = IsTrue( m_pVar->GetValue(), m_Value.c_str(), m_Cmp );
    }
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JAnimation* pChild = obj_cast<JAnimation>( GetChild( i ) );
        if (!pChild) GetChild( i )->SetVisible( bTrue );
    }
} // JIfVar::Render

void JIfVar::OnPlay()
{
    if (!m_pVar)
    {
        Init();
    }

    if (m_pVar)
    {
        if (IsTrue( m_pVar->GetValue(), m_Value.c_str(), m_Cmp ) || 
            m_bAlwaysTrue)
        {
            return;
        }
    }
} // JIfVar::OnPlay




