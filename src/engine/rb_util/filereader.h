//****************************************************************************/
//  File:  FileReader.h
//  Desc:
//****************************************************************************/
#ifndef __FILEREADER_H__
#define __FILEREADER_H__

#include "inputstream.h"
#include "windows.h"

//****************************************************************************/
//  Class:  FileReader
//  Desc:
//****************************************************************************/
class FileReader : public IStreamReader
{
    std::string     m_FileName;
    HANDLE			m_hFile;
    int             m_TotalSize;
public:
                    FileReader  ();
                    FileReader  ( const char* fileName );
    virtual void    Close       ();
    virtual int     ReadStream  ( void* buf, int nBytes );
    virtual bool    IsValid     () const;
    virtual int     GetTotalSize() const { return m_TotalSize; }

    bool            OpenFile    ( const char* fileName );
}; // class FileReader

#endif // __FILEREADER_H__


