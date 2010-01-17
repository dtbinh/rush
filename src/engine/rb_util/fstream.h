//****************************************************************************/
//  File:  FStream.h
//  Desc:
//****************************************************************************/
#ifndef __FSTREAM_H__
#define __FSTREAM_H__

#include "stream.h"
#include <vector>
#include <set>


//****************************************************************************/
//  Class:  FOutStream
//  Desc:
//****************************************************************************/
class FOutStream : public OutStream
{
    std::string     m_FileName;
    FILE*			m_hFile;

public:
                    FOutStream  ( const char* fname );
                    ~FOutStream () { OnClose(); }
    virtual const char* GetName() const { return m_FileName.c_str(); }
private:
    virtual void    OnClose     ();
    virtual int     Flush       ( const void* buf, int nBytes );
    bool            OpenFile    ( const char* fname );
    virtual bool    IsValid     () const { return (m_hFile != NULL); }
}; // class FOutStream

//****************************************************************************/
//  Class:  FInStream
//  Desc:   Read-only file input stream
//****************************************************************************/
class FInStream : public InStream
{
    std::string     m_FileName;
    FILE*			m_hFile;

public:
                    FInStream   () : m_hFile(NULL) {}
                    FInStream   ( const char* fname );
                    ~FInStream  () { Close(); }
    virtual void    Close       ();
    bool            OpenFile    ( const char* fname );
    virtual const char* GetName() const { return m_FileName.c_str(); }


private:
    virtual bool    IsValid     () const { return (m_hFile != NULL); }
    virtual int     ReadStream  ( void* buf, int nBytes );


}; // class FInStream

#endif // __FSTREAM_H__


