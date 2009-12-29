//****************************************************************************/
//  File:  FileSystem.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "Path.h"
#include "FileSystem.h"

//****************************************************************************/
//  FileSystem implementation
//****************************************************************************/
FileSystem::FileSystem()
{
}

bool FileSystem::Mount( const char* path )
{
    Path cpath( path );
    if (cpath.IsDirectory())
    {
        //  mount entry is directory
    }
    else if (!stricmp( cpath.GetExt(), "rbp" ))
    {
        //  mount entry is .rbp archive
    }
    else if (!cpath.IsDirectory())
    {
        //  mount entry is single file
        JString key( cpath.GetFileExt() );
        key.to_lower();
        if (m_MountMap.find( key ) != m_MountMap.end())
        {
            return true;
        }
        MountEntry* pEntry = m_Pool.Allocate();
        pEntry->m_Type    = MountType_File;
        pEntry->m_pParent = NULL;
        pEntry->m_Path    = cpath.GetFullPath();
        m_MountMap[key]   = pEntry;
        return true;
    }
    return false;
} // FileSystem::Mount

bool FileSystem::Unmount( const char* path )
{
    return false;
} // FileSystem::Unmount

InputStream FileSystem::OpenFile( const char* file, const char* ext )
{
    MountEntry* pEntry = FindEntry( file, ext );
    InputStream is;
    if (!pEntry)
    {
        return is;
    }

    return is;
} // FileSystem::OpenFile

const char* FileSystem::FindFile( const char* file, const char* ext )
{
    MountEntry* pEntry = FindEntry( file, ext );
    return pEntry? pEntry->m_Path.c_str() : NULL;
} // FileSystem::FindFile

MountEntry* FileSystem::FindEntry( const char* file, const char* ext )
{
    char fileExt[_MAX_PATH];
    strcpy( fileExt, file );
    strcat( fileExt, "." );
    strcat( fileExt, ext );
    JString key( fileExt );
    MountMap::iterator it = m_MountMap.find( key );
    if (it == m_MountMap.end()) 
    {
        return NULL;
    }
    return (*it).second;
} // FileSystem::FindEntry

void FileSystem::Reset()
{
    
} // FileSystem::Reset

void FileSystem::Update()
{
    
} // FileSystem::Update


