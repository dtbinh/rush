//****************************************************************************/
//  File:  FStream.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "FStream.h"
#include "direct.h"

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
    m_hFile		= INVALID_HANDLE_VALUE;
    OpenFile( fname );
}

bool FInStream::OpenFile( const char* fname )
{
    m_FileName = fname;
    char cwd[_MAX_PATH];
    getcwd( cwd, _MAX_PATH );
    m_hFile = CreateFile(   fname, 
                            GENERIC_READ, 
                            FILE_SHARE_READ, 0, 
                            OPEN_EXISTING, 
                            FILE_ATTRIBUTE_NORMAL, 
                            NULL );

    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        DWORD err = GetLastError();
        m_TotalSize = 0;
        return false;
    }
    else
    {
        m_TotalSize = ::GetFileSize( m_hFile, NULL );
        return true;
    }
} // FInStream::OpenFile

int FInStream::ReadStream( void* buf, int nBytes )
{
    if (m_hFile == INVALID_HANDLE_VALUE) return false;
    DWORD readBytes;

    if (buf == 0)
    {
        SetFilePointer( m_hFile, nBytes, NULL, FILE_CURRENT );
        return nBytes;
    }

    BOOL res = ReadFile( m_hFile, buf, nBytes, &readBytes, NULL );
    if (res == FALSE)
    {
        DWORD err = GetLastError();
        return 0;
    }
    return readBytes;
} // FInStream::ReadStream

void FInStream::Close()
{
    if (m_hFile == INVALID_HANDLE_VALUE) return;
    BOOL res = CloseHandle( m_hFile );
    if (res == FALSE) return;
    m_hFile = INVALID_HANDLE_VALUE;
} // FInStream::Close

//****************************************************************************/
/*  FOutStream implementation
//****************************************************************************/
FOutStream::FOutStream( const char* fname )
{
    m_FileName = fname;
    m_hFile = INVALID_HANDLE_VALUE;
    OpenFile( fname );
} // FOutStream::FOutStream

bool FOutStream::OpenFile( const char* fname )
{
    m_hFile = CreateFile(   fname, 
                            FILE_WRITE_DATA, 
                            FILE_SHARE_READ, 
                            NULL,
                            CREATE_ALWAYS, 
                            FILE_ATTRIBUTE_NORMAL, 
                            NULL );
    DWORD err = GetLastError();

    return (m_hFile != INVALID_HANDLE_VALUE);
} // FOutStream::OpenFile

void FOutStream::OnClose()
{
    FlushStream();
    if (m_hFile == INVALID_HANDLE_VALUE) return;
    BOOL res = CloseHandle( m_hFile );
    if (res == FALSE) return;
    m_hFile = INVALID_HANDLE_VALUE;
} // FOutStream::CloseFile

int FOutStream::Flush( const void* buf, int nBytes )
{
    if (m_hFile == INVALID_HANDLE_VALUE) return 0;
    DWORD wBytes;
    BOOL res = WriteFile( m_hFile, buf, nBytes, &wBytes, NULL );
    if (res == FALSE)
    {
        DWORD err = GetLastError();
        return 0;
    }
    return (int)wBytes;
} // FOutStream::Flush

