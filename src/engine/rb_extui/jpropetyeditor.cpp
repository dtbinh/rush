/***********************************************************************************/
//  File:   JPropertyEditor.cpp
//  Date:   15.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JPropertyEditor.h"

/***********************************************************************************/
/*  JPropertyEditor implementation
/***********************************************************************************/
decl_class(JPropertyEditor);

void JPropertyEditor::Render()
{
    PollSlot( "OutVal" );
} // JPropertyEditor::Render

void JPropertyEditor::SetInVal( const char* val )
{
    m_InVal = val;
    if (m_InVal == m_OutVal) return;
    m_OutVal = m_InVal;

    //  in case in enumeration, convert int->string
    if (m_EnumList.size() > 0)
    {
        JString strVal( val );
        int selItem = -1;
        if (cast( selItem, strVal ) && 
            selItem >= 0 && selItem < m_EnumList.size())
        {
            m_OutVal = m_EnumList[selItem];
        }
    }

    SendSignal( "OutVal" );
} // JPropertyEditor::SetInVal

void JPropertyEditor::SetOutVal( const char* val )
{
    m_OutVal = val;

    //  in case in enumeration, convert string->int
    int enumSize = (int)m_EnumList.size();
    if (enumSize > 0)
    {
        int selItem = -1;
        for (int i = 0; i < enumSize; i++)
        {
            if (!stricmp( m_EnumList[i], m_OutVal.c_str() ))
            {
                cast( m_OutVal, i );
                break;
            }
        }
    }

    if (m_InVal == m_OutVal) return;
    m_InVal = m_OutVal;

    SendSignal( "InVal" );
} // JPropertyEditor::SetOutVal

void JPropertyEditor::CallMethod()
{
    SendSignal( "OutVal" );
} // JPropertyEditor::CallMethod

void JPropertyEditor::SetEnumList( JStringList lst ) 
{ 
    m_EnumList = lst; 
    SendSignal( "EnumList" );
} // JPropertyEditor::SetEnumList




