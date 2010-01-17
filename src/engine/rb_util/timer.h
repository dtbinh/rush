//****************************************************************************/
/*    File:    Timer.h
/*    Author:  Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

//****************************************************************************/
/*    Class:   Timer
/*    Desc:    High resolution timer
//****************************************************************************/
class Timer
{
public:
                Timer    ( double period = 0.0 );
    void        Start    ( double period = 0.0 );
    void        Pause    ();
    void        Set      ( double t );
    void        Reset    ();
    double      Period   () const { return m_Period; }
    void        Resume   ();
    double      Seconds  ( bool bTotal = false ) const;

private:
    double       m_TotalTime;    //  total timer time
    double       m_TimeStart;    //  current start time
    double       m_Frequency;    //  timer frequency
    double       m_Period;
    bool         m_bPaused;

}; // class Timer

uint32_t GetProcessTimeMs();

#endif // __TIMER_H__
