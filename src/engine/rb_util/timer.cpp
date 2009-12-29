//****************************************************************************/
//  File:  timer.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include <windows.h>
#include "timer.h"

//****************************************************************************/
/*  Timer implementation
//****************************************************************************/
Timer::Timer( double period )
{
    m_bPaused = false;
    m_Period  = period;
    LARGE_INTEGER freq;
    if (QueryPerformanceFrequency( &freq ) == FALSE)
    {
        assert( !"No performance counter available!" );
    }
    m_Frequency = (double)freq.QuadPart;
    Start( period );
}

void Timer::Start( double period )
{
    m_Period = period;
    Reset();
    Resume();
}

void Timer::Pause()
{
    LARGE_INTEGER stopTime;

    QueryPerformanceCounter( &stopTime );
    m_TotalTime += (double( stopTime.QuadPart ) - m_TimeStart)/m_Frequency;
    m_bPaused = true;
}

void Timer::Set( double t )
{
    LARGE_INTEGER timeStart;
    QueryPerformanceCounter( &timeStart );
    m_TimeStart = double( timeStart.QuadPart );
    m_TotalTime = t;
}

void Timer::Reset()
{
    m_TotalTime = 0.0;
    m_TimeStart = 0.0;
}

void Timer::Resume()
{
    LARGE_INTEGER timeStart;
    QueryPerformanceCounter( &timeStart );
    m_TimeStart = double( timeStart.QuadPart );
    m_bPaused = false;
}

double Timer::Seconds( bool bTotal ) const
{
    if (m_bPaused) return m_TotalTime;
    LARGE_INTEGER curTime;

    QueryPerformanceCounter( &curTime );
    double t = ((double)curTime.QuadPart - m_TimeStart)/m_Frequency;
    t += m_TotalTime;
    if (m_Period != 0.0f && !bTotal) t = fmodf( t, m_Period );
    return t;
}