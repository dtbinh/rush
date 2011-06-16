/*****************************************************************************/
/*  File:   JNumber.cpp
/*  Date:   08.08.2006
/*  Author: Juriy Skolyar
/*****************************************************************************/

#include "stdafx.h"
#include "JObject.h"
#include "JNumber.h"

/*****************************************************************************/
/*  JNumber implementation
/*****************************************************************************/
decl_class(JNumber);

void JNumber::SetAdded( const char* val)
{
    int i = atoi (val);
    m_Value += i;
}
void JNumber::SetDeduction( const char* val)
{
    int i = atoi (val);
    m_Value -= i;
}
void JNumber::SetMeasure( const char* val)
{
    int i = atoi (val);
    m_Value *= i;
}
void JNumber::SetMultiplier( const char* val)
{
    int i = atoi (val);
    m_Value /= i;
}