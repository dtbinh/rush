//****************************************************************************/
//  File:  FStream.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "fstream.h"

void Save( const std::wstring& s, FILE* fp )
{
    const wchar_t* pName = s.c_str();
    int strSize = (int)s.size();
    fwrite( &strSize, sizeof( strSize ), 1, fp );
    if (strSize > 0) fwrite( &(s[0]), sizeof( s[0] )*strSize, 1, fp );
}

void Load( std::wstring& s, FILE* fp )
{
    int strSize = 0;
    fread( &strSize, sizeof( strSize ), 1, fp );
    if (strSize > 0)
    {
        s.resize( strSize );
        fread( &(s[0]), sizeof( s[0] )*strSize, 1, fp );
    }
}

//****************************************************************************/
/*  FInStream implementation
//****************************************************************************/
FInStream::FInStream( const char* fname )
{
    m_FileName  = fname;
    m_hFile		= NULL;
    OpenFile( fname );
}

bool FInStream::OpenFile( const char* fname )
{
    m_FileName = fname;
    m_hFile = fopen( fname, "rb" );

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

int FInStream::ReadStream( void* buf, int nBytes )
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

void FInStream::Close()
{
    if (m_hFile == NULL) return;
    fclose( m_hFile );
    m_hFile = NULL;
}

//****************************************************************************/
/*  FOutStream implementation
//****************************************************************************/
FOutStream::FOutStream( const char* fname )
{
    m_FileName = fname;
    m_hFile = NULL;
    OpenFile( fname );
}

bool FOutStream::OpenFile( const char* fname )
{
    m_FileName = fname;
    m_hFile = fopen( fname, "wb" );

    if (m_hFile == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void FOutStream::OnClose()
{
    FlushStream();
    if (m_hFile == NULL) return;
    fclose( m_hFile );
    m_hFile = NULL;
}

int FOutStream::Flush( const void* buf, int nBytes )
{
    if (m_hFile == NULL) return 0;
    uint32_t nWritten = fwrite( buf, nBytes, 1, m_hFile );
    return (int)nWritten;
}

