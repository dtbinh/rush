/***********************************************************************************/
//  File:   Logger.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __LOG_H__
#define __LOG_H__

#include "JString.h"

enum LogMessageType
{
    lmtUnknown  = 0,
    lmtError    = 1,
    lmtWarning  = 2,
    lmtMessage  = 3,
}; // enum LogMessageType

/***********************************************************************************/
//  Class:  Logger
//  Desc:   
/***********************************************************************************/
class Logger
{
    JStringList     m_Lines;
    DWORD           m_StartTick;
    JString         m_LogFileName;

public:
                Logger  ();
                ~Logger ();
    void        clear   ();
    void        err     ( const char* format, ... );
    void        warn    ( const char* format, ... );
    void        msg     ( const char* format, ... );

    JStringList lines   () const { return m_Lines; }

private:
    void        addmsg  ( LogMessageType type, const char* msg );

    void        DumpLog ();
}; // class Logger

extern Logger rlog;
#endif //__LOG_H__