//****************************************************************************/
//  File:  MemStream.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "MemStream.h"
#include "direct.h"

//****************************************************************************/
/*  MemInStream implementation
//****************************************************************************/
MemInStream::MemInStream()
{
    m_BufSize = c_DefBufferSize;
    m_pBuffer = new uint8_t[c_DefBufferSize];
    m_bOwnBuffer = true;
}

MemInStream::MemInStream ( void* pBuffer, int bufSize ) 
    :   m_pBuffer( (uint8_t*)pBuffer ), 
        m_BufSize( bufSize )
{
    m_TotalSize = bufSize;
    m_bOwnBuffer = false;
}

MemInStream::~MemInStream()
{
    if (m_bOwnBuffer) 
    {
        delete []m_pBuffer;
    }
}

int MemInStream::ReadStream( void* buf, int nuint8_ts )
{
    if (buf != NULL) 
    {
        memcpy( buf, m_pBuffer + m_Readuint8_ts, nuint8_ts );
    }
    return nuint8_ts;
} // MemInStream::ReadStream

//****************************************************************************/
/*  MemOutStream implementation
//****************************************************************************/
MemOutStream::MemOutStream( int bufSize )
{
    m_BufSize = bufSize;
    m_uint8_tsWritten = 0;
    m_pBuffer = new uint8_t[bufSize];
}

MemOutStream::~MemOutStream()
{
    if (m_BufSize >= 0) delete []m_pBuffer;
}

const int c_BufChunkSize = 1024;
int MemOutStream::Flush( const void* buf, int nuint8_ts )
{
    if (m_uint8_tsWritten + nuint8_ts > m_BufSize)
    {
        int newSize = m_uint8_tsWritten + nuint8_ts + c_BufChunkSize;
        uint8_t* pNewBuf = new uint8_t[newSize];
        memcpy( pNewBuf, m_pBuffer, m_uint8_tsWritten );
        delete []m_pBuffer;
        m_pBuffer = pNewBuf;
        m_BufSize = newSize;
    }

    memcpy( m_pBuffer + m_uint8_tsWritten, buf, nuint8_ts );
    m_uint8_tsWritten += nuint8_ts;
    return nuint8_ts;
} // MemOutStream::ReadStream
