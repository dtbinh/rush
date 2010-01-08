//****************************************************************************/
//  File:  MemStream.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
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

int MemInStream::ReadStream( void* buf, int nBytes )
{
    if (buf != NULL)
    {
        memcpy( buf, m_pBuffer + m_ReadBytes, nBytes );
    }
    return nBytes;
} // MemInStream::ReadStream

//****************************************************************************/
/*  MemOutStream implementation
//****************************************************************************/
MemOutStream::MemOutStream( int bufSize )
{
    m_BufSize = bufSize;
    m_BytesWritten = 0;
    m_pBuffer = new uint8_t[bufSize];
}

MemOutStream::~MemOutStream()
{
    if (m_BufSize >= 0) delete []m_pBuffer;
}

const int c_BufChunkSize = 1024;
int MemOutStream::Flush( const void* buf, int nBytes )
{
    if (m_BytesWritten + nBytes > m_BufSize)
    {
        int newSize = m_BytesWritten + nBytes + c_BufChunkSize;
        uint8_t* pNewBuf = new uint8_t[newSize];
        memcpy( pNewBuf, m_pBuffer, m_BytesWritten );
        delete []m_pBuffer;
        m_pBuffer = pNewBuf;
        m_BufSize = newSize;
    }

    memcpy( m_pBuffer + m_BytesWritten, buf, nBytes );
    m_BytesWritten += nBytes;
    return nBytes;
} // MemOutStream::ReadStream
