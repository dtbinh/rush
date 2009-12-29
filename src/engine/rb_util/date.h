/***********************************************************************************/
//  File:   Date.h
//  Date:   17.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __DATE_H__
#define __DATE_H__

#include "Stream.h"

/***********************************************************************************/
//  Class:  Date
//  Desc:   Represents calendar date/time value
/***********************************************************************************/
class Date
{
    int         m_Year;
    int         m_Month;
    int         m_Day;

    int         m_Hour;
    int         m_Minutes;
    int         m_Seconds;

public:
                Date            ();       
    void        SetToCurrent    ();

    int         GetYear         () const { return m_Year; }
    int         GetMonth        () const { return m_Month; }
    int         GetDay          () const { return m_Day; }

    int         GetHour         () const { return m_Hour; }
    int         GetMinutes      () const { return m_Minutes; }
    int         GetSeconds      () const { return m_Seconds; }

    void        SetYear         ( int val )  { m_Year = val; }
    void        SetMonth        ( int val )  { m_Month = val; }
    void        SetDay          ( int val )  { m_Day = val; }

    void        SetHour         ( int val )  { m_Hour = val; }
    void        SetMinutes      ( int val )  { m_Minutes = val; }
    void        SetSeconds      ( int val )  { m_Seconds = val; }

    friend OutStream& operator << ( OutStream& os, Date d )
    {
        os << d.m_Year << d.m_Month << d.m_Day << d.m_Hour << d.m_Minutes << d.m_Seconds;
        return os;
    }

    friend InStream& operator << ( InStream& is, Date d )
    {
        is >> d.m_Year >> d.m_Month >> d.m_Day >> d.m_Hour >> d.m_Minutes >> d.m_Seconds;
        return is;
    }
                 
    bool        operator == ( const Date& d ) const
    {
        return  (m_Year == d.m_Year) && (m_Month == d.m_Month) && (m_Day == d.m_Day) &&
                (m_Hour == d.m_Hour) && (m_Minutes == d.m_Minutes) && (m_Seconds == d.m_Seconds);
    }

}; // class Date

#endif //__DATE_H__