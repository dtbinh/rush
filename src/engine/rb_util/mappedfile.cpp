//****************************************************************************/
//  File:  MappedFile.cpp
//  Desc:
//****************************************************************************/
#include "stdafx.h"
#include "MappedFile.h"

//****************************************************************************/
/*  MappedFile implementation
//****************************************************************************/
MappedFile::MappedFile() :
    m_pBuffer           ( NULL ),
    m_MappedSize        ( 0 ),
    m_FileSize          ( 0 ),
    m_FirstMappedPage   ( 0 ),
    m_hFile             ( INVALID_HANDLE_VALUE ),
    m_hMapping          ( INVALID_HANDLE_VALUE )
{
}

MappedFile::~MappedFile()
{
    if (IsMapped())
    {
        Unmap();
    }
} // MappedFile::~MappedFile

uint32_t MappedFile::GetPageSize() const
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo( &sysInfo );
    return sysInfo.dwAllocationGranularity;
} // MappedFile::GetPageSize

bool MappedFile::Map( const char* fileName, int startPage, int nBytes )
{
    if (m_hMapping != INVALID_HANDLE_VALUE)
    {
        Unmap();
    }

	m_hFile = CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ, 0,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
        rlog.err( "Could not open file for mapping: %s", fileName );
        return false;
	}

	m_FileSize = ::GetFileSize( m_hFile, NULL );

    //  create mapping name
	char mapName[_MAX_PATH];
	strcpy( mapName, fileName );
    char* pName = mapName;
    while (*pName)
	{
		if (*pName == '\\' || *pName == '/') *pName = '_';
		pName++;
	}

	m_hMapping = CreateFileMapping( m_hFile, NULL, PAGE_READONLY, 0, 0, mapName );
	uint32_t err = GetLastError();
	if (m_hMapping == NULL)
    {
        rlog.err( "Could not memory-map file: %s", fileName );
        m_hMapping = INVALID_HANDLE_VALUE;
        return false;
    }

	m_pBuffer = (uint8_t*)MapViewOfFile( m_hMapping, FILE_MAP_READ, 0, startPage*GetPageSize(), nBytes );
    if (m_pBuffer == NULL)
    {
        uint32_t errCode = GetLastError();
        rlog.err( "Could not memory-map view of file: %s. Error code: %X", fileName );
        return false;
    }
	CloseHandle( m_hFile );
	m_hFile = INVALID_HANDLE_VALUE;

    m_MappedSize        = nBytes;
    m_FirstMappedPage   = startPage;

    return true;
} // MappedFile::Map

bool MappedFile::Unmap()
{
    if (m_hMapping == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    BOOL bRes = UnmapViewOfFile( m_pBuffer );
    if (bRes == FALSE)
    {
        rlog.err( "Could not memory-unmap file." );
        return false;
    }
    m_pBuffer = NULL;
    CloseHandle( m_hMapping );
    m_hMapping  = INVALID_HANDLE_VALUE;

    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle( m_hFile );
        m_hFile = INVALID_HANDLE_VALUE;
    }
    return true;
} // MappedFile::Unmap

const int c_WarmupStep = 4096;
uint8_t MappedFile::Warmup( int startPage, int nBytes )
{
    if (nBytes == 0)
    {
        nBytes = m_MappedSize;
    }
	const uint8_t* pData = m_pBuffer + startPage*GetPageSize();
	uint8_t res = 0;
	int nTouched = 0;
	while (nTouched < nBytes)
	{
		res      += m_pBuffer[nTouched];
		nTouched += c_WarmupStep;
	}

	return res;
} // MappedFile::Warmup

