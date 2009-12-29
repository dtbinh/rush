/***********************************************************************************/
//  File:   MemoryManager.h
//  Date:   15.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __MEMORYMANAGER_H__
#define __MEMORYMANAGER_H__

#include "stdio.h"
#include "stdlib.h"

/***********************************************************************************/
//  Class:  FrameStat
//  Desc:   Memory allocation statistics for one frame
/***********************************************************************************/
struct FrameStat
{
    int     m_Frame;

    int     m_NAllocs;
    int     m_AllocBytes;
    int     m_MaxAlloc;
    int     m_MinAlloc;

    int     m_NDeletes;
    int     m_DeletedBytes;
    int     m_MaxDelete;
    int     m_MinDelete;

    void Reset()
    {
        m_Frame         = 0;
        m_NAllocs       = 0;
        m_AllocBytes    = 0;
        m_MaxAlloc      = 0;
        m_MinAlloc      = 0;
        m_NDeletes      = 0;
        m_DeletedBytes  = 0;
        m_MaxDelete     = 0;
        m_MinDelete     = 0;
    }
}; // struct FrameStat

const int c_NTrackedMemFrames = 1024;
/***********************************************************************************/
//  Class:  MemoryManager
//  Desc:   Keeps track of memory allocation, gathers statistics, performs  custom 
/*              memory allocation
/***********************************************************************************/
class MemoryManager 
{
public:
                            MemoryManager   ();
    static MemoryManager&   inst            ();
    void                    OnFrame         ();

    void                    OnAllocate      ( void* p, size_t size );
    void                    OnDelete        ( void* p );
    const FrameStat&        GetStat         () const { return m_TotalStat; }
    const FrameStat&        GetStat         ( int nFramesAgo );

private:
    FrameStat               m_TotalStat;
    FrameStat               m_InvalidStat;

    FrameStat               m_Stats[c_NTrackedMemFrames];
    int                     m_CurStatEntry;
}; // class MemoryManager
/*
#ifdef _DEBUG

inline void* __cdecl operator new( size_t nSize )
{
    void* pResult = malloc( nSize );
    MemoryManager::inst().OnAllocate( pResult, nSize );
    return pResult;
}

inline void __cdecl operator delete( void* p )
{
    if (p == NULL) return;
    MemoryManager::inst().OnDelete( p );
    free( p );
}

inline void* __cdecl operator new[]( size_t nSize )
{
    void* pResult = malloc( nSize );
    MemoryManager::inst().OnAllocate( pResult, nSize );
    return pResult;
}

inline void __cdecl operator delete[]( void* p )
{
    if (p == NULL) return;
    MemoryManager::inst().OnDelete( p );
    free( p );
}

#endif // _DEBUG
*/
#endif //__MEMORYMANAGER_H__