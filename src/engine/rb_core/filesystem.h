//****************************************************************************/
/*    File:    FileSystem.h
/*    Author:  Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "InputStream.h"
#include "UniformPool.h"
#include "JString.h"

enum MountType
{
    MountType_Unknown   = 0,
    MountType_Directory = 1,
    MountType_File      = 2,
    MountType_Package   = 3
}; // enum MountType

struct MountEntry
{
    std::string     m_Path;
    MountType       m_Type;
    MountEntry*     m_pParent;

    MountEntry() : m_Type( MountType_Unknown ), m_pParent( NULL )
    {}
}; // struct MountEntry

typedef stdext::hash_map<JString, MountEntry*>  MountMap;
typedef UniformPool<MountEntry>                 MountEntryPool;

//****************************************************************************/
//  Class:   FileSystem
//  Desc:    Asset virtual file system, which supports mounting of the
/*              lump package .rbp files, directories, and file paths to serve
/*              as data source
//****************************************************************************/
class FileSystem
{
public:
                    FileSystem  ();

    bool            Mount       ( const char* path );
    bool            Unmount     ( const char* path );

    InputStream     OpenFile    ( const char* file, const char* ext );
    const char*     FindFile    ( const char* file, const char* ext );

    void            Reset       ();
    void            Update      ();

private:
    MountMap        m_MountMap;
    MountEntryPool  m_Pool;

    MountEntry*     FindEntry   ( const char* file, const char* ext );

}; // class FileSystem

#endif // __FILESYSTEM_H__