//****************************************************************************/
//  File:  CountStream.h
//  Desc:  
//****************************************************************************/
#ifndef __COUNTSTREAM_H__
#define __COUNTSTREAM_H__

#include "stream.h"
//****************************************************************************/
//  Class:  CountStream
//  Desc:   Simple stream stub, which just counts number of uint8_ts passed in
//****************************************************************************/
class CountStream : public OutStream
{   
    int             m_uint8_tsWritten;

    virtual bool    IsValid     () const 
    { 
        return true; 
    }

    virtual int     Flush       ( void* buf, int nuint8_ts )
    { 
        m_uint8_tsWritten += nuint8_ts; 
        return nuint8_ts; 
    }

public:
    CountStream() : m_uint8_tsWritten(0) {}
    int GetSize() const 
    { 
        return m_uint8_tsWritten; 
    }
}; // class CountStream

#endif // __COUNTSTREAM_H__


