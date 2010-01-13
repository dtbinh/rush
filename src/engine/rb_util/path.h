//****************************************************************************/
/*    File:    Path.h
/*    Author:  Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PATH_H__
#define __PATH_H__

//****************************************************************************/
//  Class:   Path
//  Desc:    Class, which incapsulates working with file path
//****************************************************************************/
class Path
{
public:
                Path            ();
                Path            ( const char* path );

    const char* GetFullPath     () const { return m_FullPath; }
    const char* GetDrive        () const { return m_Drive; }
    const char* GetDirectory    () const { return m_Directory; }
    const char* GetFile         () const { return m_File; }
    const char* GetExt          () const { return m_Ext; }
    const char* GetFileExt      () const { return m_FileExt; }
    const char* GetDriveDir     () const { return m_DriveDir; }

    operator const char*        () const { return m_FullPath; }

    bool        SetPath         ( const char* val );
    bool        SetDrive        ( const char* val );
    bool        SetDirectory    ( const char* val );
    bool        SetFile         ( const char* val );
    bool        SetExt          ( const char* val );
    bool        SetFileExt      ( const char* val );

    bool        SameFolder      ( const Path& path ) const;

    bool        IsDirectory     () const;
    int         GetDirDepth     () const { return m_DirDepth; }

    void        Clear           ();
    bool        Exists          () const;
    int         GetFileSize     () const;

    //! converts path to the normalized name (replacing "..", ".", "\\")
    void        Normalize       ();

    //! moves current path one directory up in the directory tree
    bool        DirUp           ();

    //! appends current directory with given directory
    void        DirAppend       ( const char* dirname );

    int         GetDirFiles     ( std::vector<Path>& files, const char* ext = NULL );

private:
    char        m_FullPath  [_MAX_PATH];
    char        m_Drive     [_MAX_PATH];
    char        m_Directory [_MAX_PATH];
    char        m_DriveDir  [_MAX_PATH];
    char        m_File      [_MAX_PATH];
    char        m_FileExt   [_MAX_PATH];
    char        m_Ext       [_MAX_PATH];
    int         m_DirDepth;

    void        CreateFullPath();
    void        CalcDirDepth  ();
    void        CreateFileExt ();
    void        FixDirChars   ();
    void        CreateDriveDir();
}; // class Path

#endif // __PATH_H__
