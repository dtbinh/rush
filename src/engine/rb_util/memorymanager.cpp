/***********************************************************************************/
//  File:   MemoryManager.cpp
//  Date:   15.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "MemoryManager.h"

/***********************************************************************************/
/*  MemoryManager implementation
/***********************************************************************************/
MemoryManager::MemoryManager()
{
    m_CurStatEntry = 0;
    m_Stats[0].Reset();
    m_TotalStat.Reset();
    m_InvalidStat.Reset();
    m_InvalidStat.m_Frame = -1;

} // MemoryManager::MemoryManager

void MemoryManager::OnFrame()
{
    FrameStat& stat = m_Stats[m_CurStatEntry];

    m_TotalStat.m_NAllocs      += stat.m_NAllocs;
    m_TotalStat.m_AllocBytes   += stat.m_AllocBytes;
    m_TotalStat.m_MaxAlloc     += stat.m_MaxAlloc;
    m_TotalStat.m_MinAlloc     += stat.m_MinAlloc;
    m_TotalStat.m_NDeletes     += stat.m_NDeletes;
    m_TotalStat.m_DeletedBytes += stat.m_DeletedBytes;
    m_TotalStat.m_MaxDelete    += stat.m_MaxDelete;
    m_TotalStat.m_MinDelete    += stat.m_MinDelete;

    m_TotalStat.m_Frame++;
    m_CurStatEntry++;
    if (m_CurStatEntry == c_NTrackedMemFrames)
    {
        m_CurStatEntry = 0;
    }
    m_Stats[m_CurStatEntry].Reset();
    m_Stats[m_CurStatEntry].m_Frame = m_TotalStat.m_Frame;
} // MemoryManager::OnFrame

const FrameStat& MemoryManager::GetStat( int nFramesAgo )
{
    if (nFramesAgo >= c_NTrackedMemFrames) return m_InvalidStat;
    int cEntry = m_CurStatEntry - nFramesAgo;
    if (cEntry < 0) cEntry += c_NTrackedMemFrames;
    return m_Stats[cEntry];
} // MemoryManager::GetStat

void MemoryManager::OnAllocate( void* p, size_t size )
{
    FrameStat& stat = m_Stats[m_CurStatEntry];
    if (stat.m_NAllocs == 0)
    {
        stat.m_MinAlloc = size;
        stat.m_MaxAlloc = size;
    }
    stat.m_NAllocs++;
    stat.m_AllocBytes += size;
    if (stat.m_MinAlloc > size) stat.m_MinAlloc = size;
    if (stat.m_MaxAlloc < size) stat.m_MaxAlloc = size;
} // MemoryManager::OnAllocate

void MemoryManager::OnDelete( void* p )
{
    FrameStat& stat = m_Stats[m_CurStatEntry];
    stat.m_NDeletes++;
} // MemoryManager::OnDelete

MemoryManager& MemoryManager::inst()
{
    static MemoryManager me;
    return me;
}