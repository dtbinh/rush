//****************************************************************************/
//  File:  MappedFile.h
//  Desc:
//****************************************************************************/
#ifndef __MAPPEDFILE_H__
#define __MAPPEDFILE_H__

#include "windows.h"

//****************************************************************************/
//  Class:  MappedFile
//  Desc:   Wrapper around Windows file-mapping
//****************************************************************************/
class MappedFile
{
    BYTE*       m_pBuffer;
    int         m_MappedSize;
    int         m_FileSize;
    int         m_FirstMappedPage;
    HANDLE      m_hFile;
    HANDLE      m_hMapping;

public:
                MappedFile      ();
                ~MappedFile     ();

    bool        Map             ( const char* fileName, int startPage = 0, int nBytes = 0 );
    bool        Unmap           ();
    bool        IsMapped        () const { return (m_pBuffer != NULL); }
    BYTE*       GetDataPtr      () const { return m_pBuffer; }
    BYTE        Warmup          ( int startPage = 0, int nBytes = 0 );
    uint32_t       GetPageSize     () const;

}; // class MappedFile

#endif // __MAPPEDFILE_H__


