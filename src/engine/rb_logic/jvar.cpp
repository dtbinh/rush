//****************************************************************************/
//  File:  JVar.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "JAnimation.h"
#include "JVar.h"

//****************************************************************************/
/*  JVar implementation
//****************************************************************************/
decl_class(JVar);

bool JVar::GetFloatValue( float& val ) const
{
    return cast( val, m_Value );
}

bool JVar::GetIntValue( int& val ) const
{
    return cast( val, m_Value );
}

void JVar::SetValue( const char* val )
{
    m_Value = val;
    SendSignal( "Value" );
}

void JVar::SetDefault( const char* val )
{
    m_Default = val;
    SendSignal( "Default" );
}




