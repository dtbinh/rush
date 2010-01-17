//****************************************************************************/
//  File:  Thread.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "thread.h"

uint32_t WINAPI ThreadStarter( LPVOID lpParameter )
{
    Thread* pThread = (Thread*)lpParameter;
    if (pThread)
    {
        pThread->OnStart();
        pThread->Terminate();
        pThread->OnEnd();
    }
    return 0;
} // ThreadStarter

//****************************************************************************/
/*  Thread implementation
//****************************************************************************/
Thread::Thread()
{
    m_State    = ThreadState_Idle;
    m_hThread  = NULL;
    m_Priority = THREAD_PRIORITY_NORMAL;
} // Thread::Thread

Thread::~Thread()
{
} // Thread::~Thread

void Thread::Start()
{
    if (m_State != ThreadState_Idle) return;
    if (m_hThread != NULL) return;

    uint32_t threadID = 0;
    m_hThread = CreateThread( NULL, 0, ThreadStarter, this, 0, &threadID );
    SetThreadPriority( m_hThread, m_Priority );

    m_State = ThreadState_Running;
} // Thread::Start

void Thread::OnEnd()
{
    if (m_State != ThreadState_Terminating) return;
    if (m_hThread == NULL) return;
    CloseHandle( m_hThread );
    m_hThread = NULL;
    m_State = ThreadState_Idle;
} // Thread::Start

void Thread::Terminate()
{
    if (m_State == ThreadState_Suspended) Resume();
    if (m_State == ThreadState_Running)
    {
        m_State = ThreadState_Terminating;
    }
    OnTerminate();
} // Thread::Terminate

void Thread::Suspend()
{
    if (m_State != ThreadState_Running || m_hThread == NULL) return;
    SuspendThread( m_hThread );
    m_State = ThreadState_Suspended;
} // Thread::Suspend

void Thread::Resume()
{
    if (m_State != ThreadState_Suspended || m_hThread == NULL) return;
    ResumeThread( m_hThread );
    m_State = ThreadState_Running;
} // Thread::Suspend


