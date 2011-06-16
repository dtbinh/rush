//****************************************************************************/
//  File:  FileReader.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "filereader.h"

//****************************************************************************/
//  FileReader implementation
//****************************************************************************/
FileReader::FileReader()
{
    m_hFile     = NULL;
    m_TotalSize = 0;
}

FileReader::FileReader( const char* fileName )
{
    m_FileName  = fileName;
    m_hFile     = NULL;
    m_TotalSize = 0;
    OpenFile( fileName );
}

bool FileReader::OpenFile( const char* fileName )
{
    m_FileName = fileName;
    m_hFile = fopen( fileName, "rb" );

    if (m_hFile == NULL)
    {
        m_TotalSize = 0;
        return false;
    }
    else
    {
        fseek( m_hFile, 0, SEEK_END );
        m_TotalSize = ftell( m_hFile );
        fseek( m_hFile, 0, SEEK_SET );
        return true;
    }
}

int FileReader::ReadStream( void* buf, int nBytes )
{
    if (m_hFile == NULL) return false;
    if (buf == 0)
    {
        fseek( m_hFile, nBytes, SEEK_CUR );
        return nBytes;
    }

    uint32_t nReadBytes = fread( buf, nBytes, 1, m_hFile );
    return nReadBytes;
}

void FileReader::Close()
{
    if (m_hFile == NULL) return;
    fclose( m_hFile );
    m_hFile = NULL;
}

bool FileReader::IsValid() const
{
    return (m_hFile != NULL);
}

