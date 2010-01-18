//****************************************************************************/
//  File:  Path.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "windows.h"
#include "path.h"

//****************************************************************************/
/*  Path implementation
//****************************************************************************/
Path::Path()
{
    Clear();
}

Path::Path( const char* path )
{
    Clear();
    SetPath( path );
}

bool Path::SetPath( const char* val )
{
    if (val == NULL)
    {
        return false;
    }
    char ext[_MAX_PATH];
    _splitpath( val, m_Drive, m_Directory, m_File, ext );
    //  remove '.' at the beginning of extension
    char* pExt = ext;
    if (pExt[0] == '.') pExt++;
    strcpy( m_Ext, pExt );

    //  remove ':' at the end of drive
    int lastChar = strlen( m_Drive ) - 1;
    while (lastChar > 0)
    {
        if (m_Drive[lastChar] == ':')
        {
            m_Drive[lastChar] = 0;
        }
        else
        {
            break;
        }
        lastChar--;
    }

    FixDirChars();
    CalcDirDepth();
    CreateFileExt();
    CreateDriveDir();
    return true;
} // Path::SetPath

bool Path::SetDrive( const char* val )
{
    if (!val)
    {
        m_Drive[0] = 0;
        CreateFullPath();
        return true;
    }
    strcpy( m_Drive, val );
    CreateFullPath();
    CreateDriveDir();
    return true;
} // Path::SetDrive

bool Path::SetDirectory( const char* val )
{
    if (!val || val[0] == 0)
    {
        m_Directory[0] = 0;
        CreateFullPath();
        return true;
    }

    char path  [_MAX_PATH];
    char dir   [_MAX_PATH];
    char drive [_MAX_PATH];
    char file  [_MAX_PATH];
    char ext   [_MAX_PATH];
    strcpy( path, val );
    int len = strlen( path );
    if (path[len - 1] != '\\')
    {
        strcat( path, "\\" );
    }
    _splitpath( path, drive, dir, file, ext );
    if (drive[0] != 0)
    {
        SetDrive( drive );
        val = dir;
    }

    if (val[0] != '\\')
    {
        m_Directory[0] = '\\';
        strcpy( m_Directory + 1, val );
    }
    else
    {
        strcpy( m_Directory, val );
    }
    FixDirChars();
    CalcDirDepth();
    CreateFullPath();
    CreateDriveDir();
    return true;
} // Path::SetDirectory

bool Path::SetFile( const char* val )
{
    if (!val)
    {
        m_File[0] = 0;
        CreateFullPath();
        CreateFileExt();
        return true;
    }
    strcpy( m_File, val );
    CreateFileExt();
    CreateFullPath();
    return true;
} // Path::SetFile

bool Path::SetExt( const char* val )
{
    if (!val)
    {
        m_Ext[0] = 0;
        CreateFullPath();
        CreateFileExt();
        return true;
    }
    strcpy( m_Ext, val );
    CreateFileExt();
    CreateFullPath();
    return true;
} // Path::SetExt

bool Path::SetFileExt( const char* val )
{
    if (!val)
    {
        m_Ext[0]  = 0;
        m_File[0] = 0;
        CreateFullPath();
        CreateFileExt();
        return true;
    }

    char dir  [_MAX_PATH];
    char drive [_MAX_PATH];
    char file  [_MAX_PATH];
    char ext   [_MAX_PATH];
    _splitpath( val, drive, dir, file, ext );
    strcpy( m_File, file );
    char* pExt = ext;
    if (pExt[0] == '.') pExt++;
    strcpy( m_Ext, pExt );
    CreateFileExt();
    CreateFullPath();
    return true;
} // Path::SetFileExt

void Path::Clear()
{
    m_FullPath[0]   = 0;
    m_Drive[0]      = 0;
    m_Directory[0]  = 0;
    m_File[0]       = 0;
    m_FileExt[0]    = 0;
    m_Ext[0]        = 0;
    m_DriveDir[0]   = 0;
    m_DirDepth      = 0;
} // Path::Clear

void Path::CreateFullPath()
{
    _makepath( m_FullPath, m_Drive, m_Directory, m_File, m_Ext );
} // Path::CreateFullPath

void Path::CalcDirDepth()
{
    int depth = 0;
    char* pChar = m_Directory;
    while (*pChar)
    {
        if (*pChar == '\\')
        {
            depth++;
        }
        pChar++;
    }
    m_DirDepth = depth;
} // Path::CalcDirDepth

void Path::FixDirChars()
{
    //  replace slashes with backslashes
    char* pChar = m_Directory;
    while (*pChar)
    {
        if (*pChar == '/') *pChar = '\\';
        pChar++;
    }

    //  remove trailing backslashes in directory name
    int lastChar = strlen( m_Directory ) - 1;
    while (lastChar > 0)
    {
        if (m_Directory[lastChar] == '\\')
        {
            m_Directory[lastChar] = 0;
        }
        else
        {
            break;
        }
        lastChar--;
    }

    CreateFullPath();
}

void Path::CreateFileExt()
{
    if (m_Ext[0] != 0)
    {
        sprintf( m_FileExt, "%s.%s", m_File, m_Ext );
    }
    else
    {
        strcpy( m_FileExt, m_File );
    }
}

void Path::CreateDriveDir()
{
    _makepath( m_DriveDir, m_Drive, m_Directory, NULL, NULL );
}

bool Path::Exists() const
{
    if (::GetFileAttributesA( GetFullPath() ) == 0xFFFFFFFF)
    {
       UINT err = ::GetLastError();
       if((err == ERROR_FILE_NOT_FOUND)
         || (err == ERROR_INVALID_PARAMETER)
         || (err == ERROR_NOT_READY)
         || (err == ERROR_PATH_NOT_FOUND)
         || (err == ERROR_INVALID_NAME)
         || (err == ERROR_BAD_NETPATH ))
       {
          return false;
       }
       return true;
    }
    return true;
}

bool Path::IsDirectory() const
{
    uint32_t attributes = ::GetFileAttributesA( GetFullPath() );
    return attributes != 0xFFFFFFFF && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

int Path::GetFileSize() const
{
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!::GetFileAttributesExA( GetFullPath(),
        ::GetFileExInfoStandard, &fad ) )
    {
        return 0;
    }

    if ((fad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=0)
    {
        return 0;
    }
    return fad.nFileSizeLow;
}

bool Path::SameFolder( const Path& path ) const
{
    return (!stricmp( GetDriveDir(), path.GetDriveDir() ));
}

int Path::GetDirFiles( std::vector<Path>& files, const char* ext )
{
    WIN32_FIND_DATA fData;

    Path searchPath( GetFullPath() );
    searchPath.SetFile( "*" );
    if (ext)
    {
        searchPath.SetExt( ext );
    }
    else
    {
        searchPath.SetExt( "*" );
    }

    HANDLE h = FindFirstFile( searchPath.GetFullPath(), &fData );
    uint32_t err = GetLastError();
    if (h == INVALID_HANDLE_VALUE) return 0;

    int nFiles = 0;
    if (fData.cFileName[0] != '.')
    {
        files.push_back( fData.cFileName );
        nFiles++;
    }
    while (FindNextFile( h, &fData ))
    {
        if (fData.cFileName[0] != '.')
        {
            files.push_back( fData.cFileName );
            nFiles++;
        }
    }
    FindClose( h );
    return nFiles;
}

bool Path::DirUp()
{
    int nCh = strlen( m_Directory );
    int chPos = -1;
    for (int i = nCh; i >= 0; i--)
    {
        if (m_Directory[i] == '\\' || m_Directory[i] == '/')
        {
            chPos = i;
            break;
        }
    }
    if (chPos == -1)
    {
        if (m_Directory[0] != 0)
        {
            //  top-level directory
            chPos = 0;
        }
        else
        {
            return false;
        }
    }
    m_Directory[chPos] = 0;
    FixDirChars();
    CreateFullPath();
    return true;
}

void Path::DirAppend( const char* dirname )
{
    FixDirChars();
    strcat( m_Directory, "\\" );
    const char* pDirname = dirname;
    while (*pDirname == '\\' || *pDirname == '/')
    {
        pDirname++;
    }
    strcat( m_Directory, pDirname );
    FixDirChars();
}

void Path::Normalize()
{
    FixDirChars();

    char newPath[_MAX_PATH];
    const char* pSrc = m_FullPath;
    char* pDst = newPath;
    while (*pSrc != 0)
    {
        if (*pSrc == '\\')
        {
            if (*(pSrc + 1) == '.')
            {
                if (*(pSrc + 2) == '.')
                {
                    //  "parent directory"
                    pSrc += 3;
                    while (pDst > newPath && *(pDst - 1) == '\\')
                    {
                        pDst--;
                    }
                    while (pDst > newPath && *(pDst - 1) != '\\')
                    {
                        pDst--;
                    }
                    while (pDst > newPath && *(pDst - 1) == '\\')
                    {
                        pDst--;
                    }
                    continue;
                }
                else
                {
                    //  "current directory"
                    pSrc += 2;
                }
            }
            else if (*(pSrc + 1) == '\\')
            {
                //  "double slash"
                pSrc++;
                continue;
            }
        }
        *pDst = *pSrc;
        pSrc++;
        pDst++;
    }
    *pDst = 0;
    SetPath( newPath );
}


