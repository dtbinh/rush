//****************************************************************************/
//  File:  MemStream.h
//  Desc:  
//****************************************************************************/
#ifndef __MEMSTREAM_H__
#define __MEMSTREAM_H__

#include "Stream.h"
#include <vector>
#include <set>

const int c_DefBufferSize = 8192;
//****************************************************************************/
//  Class:  MemOutStream
//  Desc:   
//****************************************************************************/
class MemOutStream : public OutStream
{   
    BYTE*           m_pBuffer;
    int             m_BufSize;
    int             m_BytesWritten;

    virtual bool    IsValid         () const { return (m_pBuffer != NULL); } 
    virtual int     Flush           ( const void* buf, int nBytes );
    virtual const char* GetName() const { return "MemOutStream"; }

public:
                    MemOutStream    ( void* pBuffer ) : m_pBuffer( (BYTE*)pBuffer ), m_BytesWritten(0), m_BufSize(-1) {}
                    MemOutStream    ( int bufSize = c_DefBufferSize );
                    ~MemOutStream   ();
    void*           GetBuffer       () { return m_pBuffer; }
}; // class MemOutStream

//****************************************************************************/
//  Class:  MemInStream
//  Desc:   Read-only file input stream
//****************************************************************************/
class MemInStream : public InStream
{
    BYTE*           m_pBuffer;
    int             m_BufSize;
    bool            m_bOwnBuffer;

    virtual bool    IsValid     () const { return (m_pBuffer != NULL); } 
    virtual int     ReadStream  ( void* buf, int nBytes );
    virtual const char* GetName () const { return "MemInStream"; }

public:
                    MemInStream ( void* pBuffer, int bufSize = -1 );
                    MemInStream ();
                    ~MemInStream();
    void*           GetBuffer   () { return m_pBuffer; }
}; // class MemInStream

#endif // __MEMSTREAM_H__


