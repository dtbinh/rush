/***********************************************************************************/
//  File:   FileHistoryDlg.h
//  Desc:   Exporter core class declaration
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __FILEHISTORYDLG_H__
#define __FILEHISTORYDLG_H__

#include <vector>
#include <string>

/***********************************************************************************/
//  Class:  FileHistoryDlg
//  Desc:    
/***********************************************************************************/
class FileHistoryDlg  
{
public:
                FileHistoryDlg      ( HWND hWnd = NULL, HINSTANCE hInstance= NULL );
                ~FileHistoryDlg     ();

    bool        Show                ( bool bSaveDlg = false );
    void        AddHistoryItem      ( const char* item ) { m_History.push_back( item ); }
    int         GetNumHistoryItems  () const { return m_History.size(); }
    const char* GetHistoryItem      ( int idx ) const { return m_History[idx].c_str(); }

    void        AddFilter           ( const char* descr, const char* ext );
    const char* GetFileName         () const  { return m_FileName.GetFullPath(); }
    void        SetFileName         ( const char* path ) { m_FileName = path; }
    void        EnableHistory       ( bool bEnable = true ) { m_bEnableHistory = bEnable; }

    UINT_PTR CALLBACK HookProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    static UINT_PTR CALLBACK HookProcCB( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

private:
    typedef std::vector<std::string> StringList;
    HWND            m_hWnd;
    HINSTANCE       m_hInstance;
    StringList      m_History;
    Path            m_FileName;

    char            m_Filter[_MAX_PATH];

    HWND            m_hHistory;
    bool            m_bEnableHistory;
}; // class FileHistoryDlger

#endif // __FileHistoryDlg_H__

