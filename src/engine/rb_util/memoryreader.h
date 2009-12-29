//****************************************************************************/
//  File:  MemoryReader.h
//  Desc:  
//****************************************************************************/
#ifndef __MEMORYREADER_H__
#define __MEMORYREADER_H__

//****************************************************************************/
//  Class:  MemoryReader
//  Desc:   Implementation of the stream reader which reads from memory
//****************************************************************************/
class MemoryReader : public IStreamReader
{
    const BYTE*     m_pBuffer;
    int             m_BufSize;
    int             m_ReadBytes;

public:
    MemoryReader() : 
      m_pBuffer( NULL ), 
      m_BufSize( 0 ), 
      m_ReadBytes( 0 )
    {}

    MemoryReader( const BYTE* pBuf, int bufSize ) : 
      m_pBuffer( pBuf ), 
      m_BufSize( bufSize ),
      m_ReadBytes( 0 )
    {}

    virtual void Close(){}

    virtual int ReadStream( void* buf, int nBytes )
    {
        if (buf)
        {
            memcpy( buf, m_pBuffer + m_ReadBytes, nBytes );
        }
        m_ReadBytes += nBytes;
        return nBytes;
    }

    virtual bool IsValid() const 
    { 
        return ((m_pBuffer != NULL) && (m_ReadBytes < m_BufSize)); 
    }

    virtual int GetTotalSize() const { return m_BufSize; }
}; // class MemoryReader

#endif // __MEMORYREADER_H__


