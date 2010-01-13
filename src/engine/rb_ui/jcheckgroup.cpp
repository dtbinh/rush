//****************************************************************************/
//  File:   JCheckGroup.cpp
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk 
//****************************************************************************/
#include "precompile.h"
#include "JButton.h"
#include "JCheckBox.h"
#include "JCheckGroup.h"

//****************************************************************************/
/*  JCheckGroup implementation
//****************************************************************************/
decl_class(JCheckGroup);
JCheckGroup::JCheckGroup()
{
    m_ActiveIdx = -1;
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
} // JCheckGroup::JCheckGroup

void JCheckGroup::Render()
{   
    int nCh = GetNChildren();
    int cChild = 0;
    for (int i = 0; i < nCh; i++)
    {
        JCheckBox* pCB = obj_cast<JCheckBox>( GetChild( i ) );
        if (!pCB) continue;
        bool bChecked = (cChild == m_ActiveIdx);
        if (pCB->IsChecked() != bChecked)
        {
            //  check changed
            SetActiveIdx( cChild );
            return;
        }
        cChild++;
    }
} // JCheckGroup::Render

void JCheckGroup::SetActiveIdx( int idx )
{
    m_ActiveIdx = idx;
    int nCh = GetNChildren();
    int cChild = 0;
    for (int i = 0; i < nCh; i++)
    {
        JCheckBox* pCB = obj_cast<JCheckBox>( GetChild( i ) );
        if (!pCB) continue;
        bool bChecked = (cChild == m_ActiveIdx);
        if (pCB->IsChecked() != bChecked)
        {
            pCB->Check( bChecked );
        }
        cChild++;
    }
    SendSignal( "activeidx" );
} // JCheckGroup::SetCheck


