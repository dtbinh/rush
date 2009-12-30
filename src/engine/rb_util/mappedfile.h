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
    uint8_t*       m_pBuffer;
    int         m_MappedSize;
    int         m_FileSize;
    int         m_FirstMappedPage;
    HANDLE      m_hFile;
    HANDLE      m_hMapping;

public:
                MappedFile      ();
                ~MappedFile     ();

    bool        Map             ( const char* fileName, int startPage = 0, int nuint8_ts = 0 );
    bool        Unmap           ();
    bool        IsMapped        () const { return (m_pBuffer != NULL); }
    uint8_t*       GetDataPtr      () const { return m_pBuffer; }
    uint8_t        Warmup          ( int startPage = 0, int nuint8_ts = 0 );
    uint32_t       GetPageSize     () const;

}; // class MappedFile

#endif // __MAPPEDFILE_H__


