//****************************************************************************/
/*    File:    Thread.h
/*    Author:  Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __THREAD_H__
#define __THREAD_H__

#include "windows.h"

//****************************************************************************/
/*  Enum:   ThreadState
//  Desc:   
//****************************************************************************/
enum ThreadState
{
    ThreadState_Unknown     = 0,
    ThreadState_Idle        = 1,
    ThreadState_Running     = 2,
    ThreadState_Terminating = 3,
    ThreadState_Suspended   = 4
}; // enum ThreadState

//****************************************************************************/
//  Class:   Thread
//  Desc:    Simple execution thread wrapper
//****************************************************************************/
class Thread
{
    ThreadState     m_State;
    HANDLE          m_hThread;
    int             m_Priority;

public:
                    Thread      ();
    virtual         ~Thread     ();

    bool            IsTerminated() const { return (m_State == ThreadState_Terminating); }
    void            Terminate   ();
    void            Start       ();
    void            Suspend     ();
    void            Resume      ();
    void            SetPriority ( int priority ) { m_Priority = priority; }

    ThreadState     GetState    () const { return m_State; }

    friend DWORD WINAPI ThreadStarter( LPVOID lpParameter );

protected:
    virtual void    OnStart     () = 0;
    virtual void    OnTerminate () {}

private:
    void    OnEnd               ();
}; // class Thread

#endif // __THREAD_H__