/***********************************************************************************/
//  File:   JStringSelector.cpp
//  Date:   09.11.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"

#include "jstringselector.h"

/***********************************************************************************/
/*  JStringSelector implementation
/***********************************************************************************/
decl_class(JStringSelector);

void JStringSelector::SetItems( JStringList items )
{
    m_Strings = items;
    if (m_SelectedItem >= items.size()) m_SelectedItem = items.size() - 1;
} // JStringSelector::SetItems

const char* JStringSelector::GetSelectedItem() const
{
    return "";
}

void JStringSelector::SetSelectedItemIdx( int val )
{

}

bool JStringSelector::CanSelectNext() const
{
    return true;
}

bool JStringSelector::CanSelectPrevious() const
{
    return true;
}

void JStringSelector::SelectNext()
{

}

void JStringSelector::SelectPrevious()
{

}
