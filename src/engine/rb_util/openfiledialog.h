/***********************************************************************************/
//  File:   OpenFileDialog.h
//  Date:   27.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __OPENFILEDIALOG_H__
#define __OPENFILEDIALOG_H__

#include "windows.h"
#include "commdlg.h"

/***********************************************************************************/
//  Class:  OpenFileDialog
//  Desc:   
/***********************************************************************************/
class OpenFileDialog
{
public:
                            OpenFileDialog  ();
    bool                    Show            ();
    void                    AddFilter       ( const char* descr, const char* filter );

    const char*             GetFilePath     () { return m_FileName; }
    void                    SetDefaultExt   ( const char* ext )
    {
        strcpy( m_DefExt, ext );
        m_Ofs.lpstrDefExt = m_DefExt;
    }

protected:
    char                    m_FileName  [_MAX_PATH];
    char                    m_DefExt    [_MAX_EXT ];

    char                    m_Filter    [_MAX_PATH];
    int                     m_nFilterChars;
    OPENFILENAME            m_Ofs;
}; // class OpenFileDialog

#endif //__OPENFILEDIALOG_H__