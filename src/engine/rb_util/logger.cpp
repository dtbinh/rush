/***********************************************************************************/
//  File:   Logger.cpp
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "time.h"
#include "Logger.h"
#include "windows.h"

const int   c_LogBufferSize = 1024;
static char s_Buffer[c_LogBufferSize];
Logger         rlog;
/***********************************************************************************/
/*  Logger implementation
/***********************************************************************************/
Logger::Logger()
{
    m_LogFileName = "engine.log";

    m_StartTick = GetTickCount();
    time_t long_time;
    time( &long_time );
    tm* newtime = localtime( &long_time );
    m_Lines.push_back( "**********************************************************************************" );
    sprintf( s_Buffer, "*    Started logging: %.19s", asctime( newtime ) );
    m_Lines.push_back( s_Buffer );
    m_Lines.push_back( "**********************************************************************************" );
} // Logger::Logger

Logger::~Logger()
{
    time_t long_time;
    time( &long_time );
    tm* newtime = localtime( &long_time );
    m_Lines.push_back( "**********************************************************************************" );
    sprintf( s_Buffer, "*    Ended logging: %.19s", asctime( newtime ) );
    m_Lines.push_back( s_Buffer );
    m_Lines.push_back( "**********************************************************************************" );

    //  write log file
    //DumpLog();
} // Logger::~Logger

void Logger::DumpLog()
{
    FILE* fp = fopen( m_LogFileName.c_str(), "wt" );
    if (!fp) return;
    for (int i = 0; i < m_Lines.size(); i++)
    {
        fprintf( fp, "%s\n", m_Lines[i].c_str() );
    }
    fclose( fp );
}

void Logger::clear()
{
    m_Lines.clear();
} // Logger::clear

void Logger::addmsg( LogMessageType type, const char* msg )
{
    char buf[c_LogBufferSize];
    char* pBuf = buf;
    uint32_t cTick = GetTickCount() - m_StartTick;
    cTick /= 1000;
    int h = cTick/(60*60);
    int m = cTick/60;
    int s = cTick%60;
    sprintf( pBuf, "%02d:%02d:%02d", h, m, s );
    pBuf += strlen( pBuf );
    if (type == lmtError)   sprintf( pBuf, " ERROR:   " );
    if (type == lmtWarning) sprintf( pBuf, " WARNING: " );
    if (type == lmtMessage) sprintf( pBuf, "          " );
    pBuf += strlen( pBuf );

    sprintf( pBuf, "%s", msg );
    m_Lines.push_back( buf );
//#ifdef _DEBUG
    OutputDebugString( buf );
    OutputDebugString( "\n" );
//#endif // _DEBUG
} // Logger::addmsg

void Logger::err( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    vsprintf( s_Buffer, format, argList );
    addmsg( lmtError, s_Buffer );
    va_end( argList );
} // Logger::warn

void Logger::warn( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    vsprintf( s_Buffer, format, argList );
    addmsg( lmtWarning, s_Buffer );
    va_end( argList );
} // Logger::warn

void Logger::msg( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    vsprintf( s_Buffer, format, argList );
    addmsg( lmtMessage, s_Buffer );
    va_end( argList );
} // Logger::warn
