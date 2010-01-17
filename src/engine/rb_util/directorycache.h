/***********************************************************************************/
//  File:   DirectoryCache.h
//  Date:   15.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __DIRECTORYCACHE_H__
#define __DIRECTORYCACHE_H__

#include <vector>
#include <string>

/***********************************************************************************/
//  Class:  Directory
//  Desc:
/***********************************************************************************/
struct Directory
{
    std::string                 m_Name;
    std::string                 m_Path;
    std::vector<Directory>      m_Subdirs;
    std::vector<std::string>    m_Files;

            Directory   (){}
            Directory   ( const char* name, const char* path = "" )
            {
                 m_Name = name;
                 m_Path = path;
                 if (m_Path.size() > 0 && m_Path[m_Path.size() - 1] != '\\')
                 {
                    m_Path += "\\";
                 }
            }
    bool    FindFile    ( const char* name, const char* ext, char* path );
    void    Update      ();
}; // struct Directory

/***********************************************************************************/
//  Class:  DirectoryCache
//  Desc:
/***********************************************************************************/
class DirectoryCache
{
    std::vector<Directory>   m_Roots;

public:
                DirectoryCache  () {}
                DirectoryCache  ( const char* root );
    bool        FindFile        ( const char* name, const char* ext, char* path );
    void        AddRoot         ( const char* path );
    int         GetNRoots       () const { return m_Roots.size(); }
    const char* GetRoot         ( int idx ) const { return m_Roots[idx].m_Name.c_str(); }

protected:
    void        UpdateCache();

}; // class DirectoryCache

#endif //__DIRECTORYCACHE_H__
