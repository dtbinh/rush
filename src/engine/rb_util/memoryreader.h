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
    const uint8_t*     m_pBuffer;
    int             m_BufSize;
    int             m_Readuint8_ts;

public:
    MemoryReader() : 
      m_pBuffer( NULL ), 
      m_BufSize( 0 ), 
      m_Readuint8_ts( 0 )
    {}

    MemoryReader( const uint8_t* pBuf, int bufSize ) : 
      m_pBuffer( pBuf ), 
      m_BufSize( bufSize ),
      m_Readuint8_ts( 0 )
    {}

    virtual void Close(){}

    virtual int ReadStream( void* buf, int nuint8_ts )
    {
        if (buf)
        {
            memcpy( buf, m_pBuffer + m_Readuint8_ts, nuint8_ts );
        }
        m_Readuint8_ts += nuint8_ts;
        return nuint8_ts;
    }

    virtual bool IsValid() const 
    { 
        return ((m_pBuffer != NULL) && (m_Readuint8_ts < m_BufSize)); 
    }

    virtual int GetTotalSize() const { return m_BufSize; }
}; // class MemoryReader

#endif // __MEMORYREADER_H__


