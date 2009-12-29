//****************************************************************************/
//  File:  CountStream.h
//  Desc:  
//****************************************************************************/
#ifndef __COUNTSTREAM_H__
#define __COUNTSTREAM_H__

#include "Stream.h"
//****************************************************************************/
//  Class:  CountStream
//  Desc:   Simple stream stub, which just counts number of bytes passed in
//****************************************************************************/
class CountStream : public OutStream
{   
    int             m_BytesWritten;

    virtual bool    IsValid     () const 
    { 
        return true; 
    }

    virtual int     Flush       ( void* buf, int nBytes )
    { 
        m_BytesWritten += nBytes; 
        return nBytes; 
    }

public:
    CountStream() : m_BytesWritten(0) {}
    int GetSize() const 
    { 
        return m_BytesWritten; 
    }
}; // class CountStream

#endif // __COUNTSTREAM_H__


