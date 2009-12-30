//****************************************************************************/
//  File:  FStream.h
//  Desc:
//****************************************************************************/
#ifndef __FSTREAM_H__
#define __FSTREAM_H__

#include "stream.h"
#include <vector>
#include <set>
#include <windows.h>


//****************************************************************************/
//  Class:  FOutStream
//  Desc:
//****************************************************************************/
class FOutStream : public OutStream
{
    std::string     m_FileName;
    HANDLE			m_hFile;

public:
                    FOutStream  ( const char* fname );
                    ~FOutStream () { OnClose(); }
    virtual const char* GetName() const { return m_FileName.c_str(); }
private:
    virtual void    OnClose     ();
    virtual int     Flush       ( const void* buf, int nBytes );
    bool            OpenFile    ( const char* fname );
    virtual bool    IsValid     () const { return (m_hFile != INVALID_HANDLE_VALUE); }
}; // class FOutStream

//****************************************************************************/
//  Class:  FInStream
//  Desc:   Read-only file input stream
//****************************************************************************/
class FInStream : public InStream
{
    std::string     m_FileName;
    HANDLE			m_hFile;

public:
                    FInStream   () : m_hFile(INVALID_HANDLE_VALUE) {}
                    FInStream   ( const char* fname );
                    ~FInStream  () { Close(); }
    virtual void    Close       ();
    bool            OpenFile    ( const char* fname );
    virtual const char* GetName() const { return m_FileName.c_str(); }


private:
    virtual bool    IsValid     () const { return (m_hFile != INVALID_HANDLE_VALUE); }
    virtual int     ReadStream  ( void* buf, int nBytes );


}; // class FInStream

void Save( const std::wstring& s, FILE* fp );
void Load( std::wstring& s, FILE* fp );

template <class T>
void Save( const std::vector<T> v, FILE* fp )
{
    int sz = v.size();
    Save( sz, fp );
    for (int i = 0; i < sz; i++) Save( v[i], fp );
} // Save

template <class T>
void Load( std::vector<T>& v, FILE* fp )
{
    int sz = 0;
    Load( sz, fp );
    v.resize( sz );
    for (int i = 0; i < sz; i++) Load( v[i], fp );
} // Load

template <class T>
void Save( const std::set<T> v, FILE* fp )
{
    int sz = v.size();
    Save( sz, fp );
    std::set<T>::const_iterator it = v.begin();
    for (int i = 0; i < sz; i++)
    {
        Save( *it, fp );
        ++it;
    }
} // Save

template <class T>
void Load( std::set<T>& v, FILE* fp )
{
    int sz = 0;
    Load( sz, fp );
    v.clear();
    T val;
    for (int i = 0; i < sz; i++)
    {
        Load( val, fp );
        v.insert( val );
    }
} // Load

template <class T> void Save( const T& val, FILE* fp )
{
    fwrite( &val, sizeof( val ), 1, fp );
}
template <class T> void Load( T& val, FILE* fp )
{
    fread( &val, sizeof( val ), 1, fp );
}

#endif // __FSTREAM_H__


