/***********************************************************************************/
//  File:   Date.cpp
//  Date:   17.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "Date.h"
#include <time.h>

/***********************************************************************************/
//  Date implementation
/***********************************************************************************/
Date::Date()
{
    SetToCurrent();
} // Date::Date

void Date::SetToCurrent()
{
    time_t long_time;
    time( &long_time );
    tm* newtime = localtime( &long_time );

    m_Year      = newtime->tm_year + 1900;
    m_Month     = newtime->tm_mon + 1;
    m_Day       = newtime->tm_mday;
    m_Hour      = newtime->tm_hour;
    m_Minutes   = newtime->tm_min;
    m_Seconds   = newtime->tm_sec;
} // Date::SetToCurrent
