//****************************************************************************/
//  File:  JSwitch.cpp
//  Date:   22.05.2006
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "jswitch.h"


//****************************************************************************/
/*  JSwitch implementation
//****************************************************************************/
decl_class(JSwitch);
JSwitch::JSwitch()
{
    m_ActiveIdx = 0;
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
} // JSwitch::JSwitch

JObject* JSwitch::GetActiveChild() const
{
    if (m_ActiveIdx < 0 || m_ActiveIdx >= GetNChildren()) return NULL;
    return GetChild( m_ActiveIdx );
} // JSwitch::GetActiveChild

void JSwitch::SetActive( const char* name )
{
    bool bFound = false;
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        GetChild( i )->SetVisible( false );
    }
    m_ActiveIdx = -1;

    if (name)
    {
        JObject* pChild = g_pObjectServer->FindObject<JObject>( name, this );
        if (!pChild)
        {
            pChild = new JWidget();
            bFound = false;
            AddChild( pChild );
            pChild->SetName( name );
        }
        else bFound = true;
        pChild->SetVisible( true );
        m_ActiveIdx = GetChildIndex( pChild );
    }
} // JSwitch::SetActive

void JSwitch::SetActiveIdx( int idx )
{
    bool bFound = false;
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        GetChild( i )->SetVisible( false );
    }
    m_ActiveIdx = idx;
    if (idx < 0 || idx >= nCh) return;
    GetChild( idx )->SetVisible( true );
} // JSwitch::SetActiveIdx




