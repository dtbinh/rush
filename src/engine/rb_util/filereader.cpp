//****************************************************************************/
//  File:  FileReader.cpp
//  Desc:
//****************************************************************************/
#include "stdafx.h"
#include "filereader.h"

//****************************************************************************/
//  FileReader implementation
//****************************************************************************/
FileReader::FileReader()
{
    m_hFile		= INVALID_HANDLE_VALUE;
    m_TotalSize = 0;
}

FileReader::FileReader( const char* fileName )
{
    m_FileName  = fileName;
    m_hFile		= INVALID_HANDLE_VALUE;
    m_TotalSize = 0;
    OpenFile( fileName );
}

bool FileReader::OpenFile( const char* fileName )
{
    m_FileName = fileName;
    m_hFile = CreateFile(   fileName,
                            GENERIC_READ,
                            FILE_SHARE_READ, 0,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL );

    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        uint32_t err = GetLastError();
        m_TotalSize = 0;
        return false;
    }
    else
    {
        m_TotalSize = ::GetFileSize( m_hFile, NULL );
        return true;
    }
} // FileReader::OpenFile

int FileReader::ReadStream( void* buf, int nuint8_ts )
{
    if (m_hFile == INVALID_HANDLE_VALUE) return false;
    uint32_t readuint8_ts;

    if (buf == 0)
    {
        SetFilePointer( m_hFile, nuint8_ts, NULL, FILE_CURRENT );
        return nuint8_ts;
    }

    BOOL res = ReadFile( m_hFile, buf, nuint8_ts, &readuint8_ts, NULL );
    if (res == FALSE)
    {
        uint32_t err = GetLastError();
        return 0;
    }
    return readuint8_ts;
} // FileReader::ReadStream

void FileReader::Close()
{
    if (m_hFile == INVALID_HANDLE_VALUE) return;
    BOOL res = CloseHandle( m_hFile );
    if (res == FALSE) return;
    m_hFile = INVALID_HANDLE_VALUE;
} // FileReader::Close

bool FileReader::IsValid() const
{
    return (m_hFile != INVALID_HANDLE_VALUE);
} // FileReader::IsValid

