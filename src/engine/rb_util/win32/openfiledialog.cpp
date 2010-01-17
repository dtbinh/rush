/***********************************************************************************/
//  File:   OpenFileDialog.cpp
//  Date:   27.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "openfiledialog.h"

#include "direct.h"

/***********************************************************************************/
/*  OpenFileDialog implementation
/***********************************************************************************/
OpenFileDialog::OpenFileDialog()
{
    memset( &m_Ofs, 0, sizeof(m_Ofs) );
    m_Ofs.lStructSize    = sizeof( m_Ofs );
    m_Ofs.lpstrFile      = m_FileName;
    m_Ofs.nMaxFile       = _MAX_PATH;
    m_Ofs.lpstrFilter    = m_Filter;

    m_nFilterChars       = 0;
    m_Filter[0]          = 0;
    m_FileName[0]        = 0;
}

void OpenFileDialog::AddFilter( const char* descr, const char* filter )
{
    strcpy( &m_Filter[m_nFilterChars], descr );
    m_nFilterChars += strlen( descr ) + 1;
    strcpy( &m_Filter[m_nFilterChars], filter );
    m_nFilterChars += strlen( filter ) + 1;
    m_Filter[m_nFilterChars] = 0;
} // OpenFileDialog::AddFilter

bool OpenFileDialog::Show()
{
    char cdir[_MAX_PATH];
    _getcwd( cdir, _MAX_PATH );
    m_Ofs.lpstrInitialDir = cdir;
    BOOL res = GetOpenFileName( &m_Ofs );
    _chdir( cdir );
    return res != FALSE;
    return false;
} // OpenFileDialog::Show
