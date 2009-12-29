/***********************************************************************************/
//  File:   FileHistoryDlg.cpp
//  Desc:   Exporter dialog window management code
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "dlgs.h"
#include "Path.h"
#include "FileHistoryDlg.h"


/***********************************************************************************/
//  FileHistoryDlg implementation
/***********************************************************************************/
FileHistoryDlg::FileHistoryDlg( HWND hWnd, HINSTANCE hInstance )
{
    m_hWnd              = hWnd;
    m_hHistory          = NULL;
    m_hInstance         = hInstance;
    m_bEnableHistory    = true;

    m_Filter[0] = 0;
    m_Filter[1] = 0;
}

FileHistoryDlg::~FileHistoryDlg()
{

}

void FileHistoryDlg::AddFilter( const char* descr, const char* ext )
{
    //  find last filter entry
    int cChar = 0;
    while ((m_Filter[cChar] != 0 || m_Filter[cChar + 1] != 0) && (cChar < _MAX_PATH))
    {
        cChar++;
    }
    if (cChar == _MAX_PATH) return;
    if (cChar != 0) cChar++;
    strcpy( m_Filter + cChar, descr );
    cChar += strlen( descr ) + 1;
    sprintf( m_Filter + cChar, "*.%s", ext );
    cChar += strlen( m_Filter + cChar ) + 1;
    m_Filter[cChar] = 0;
}

UINT_PTR CALLBACK FileHistoryDlg::HookProcCB( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  FileHistoryDlg* pDlg = NULL;
  if (msg == WM_INITDIALOG)
  {
      pDlg = (FileHistoryDlg*)lParam;
      SetWindowLongPtr( hWnd, GWLP_USERDATA, LONG( lParam ) );
  }
  else
  {
      pDlg = (FileHistoryDlg*)GetWindowLongPtr( hWnd, GWLP_USERDATA ); 
  }

  if (pDlg)
  {
      return pDlg->HookProc( hWnd, msg, wParam, lParam );
  }
  return TRUE;
} // FileHistoryDlg::HookProcCB

UINT_PTR CALLBACK FileHistoryDlg::HookProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch (msg) 
    {
    case WM_INITDIALOG:
        m_hWnd      = hWnd;
        m_hHistory  = GetDlgItem( hWnd, IDC_HISTORY );

        SendMessage( m_hHistory, CB_INSERTSTRING, -1, (LPARAM)"C:\\Program Files" );
        SendMessage( m_hHistory, CB_INSERTSTRING, -1, (LPARAM)"C:\\home" );
        SendMessage( m_hHistory, CB_INSERTSTRING, -1, (LPARAM)"C:\\downloads" );
        SendMessage( m_hHistory, CB_SETCURSEL, 0, 0 );
        break;
    case WM_COMMAND:
        {
            if (LOWORD(wParam) == IDC_HISTORY && HIWORD(wParam) == CBN_SELCHANGE) 
            {
                HWND hFile = GetDlgItem( hWnd, cmb2 );
                SendMessage( hWnd, CDM_SETCONTROLTEXT, cmb2, (LPARAM)"C:\\home" );
            }
            return TRUE;
        }
        break;
    case WM_PAINT:
        {
            UpdateWindow( hWnd );
            return TRUE;
        }
        break;
    }
    return FALSE;
} // FileHistoryDlg::HookProc

bool FileHistoryDlg::Show( bool bSaveDlg )
{
  OPENFILENAME ofn;
  memset( &ofn, 0, sizeof( ofn ) );
  char fileName[_MAX_PATH];
  strcpy( fileName, m_FileName.GetFileExt() );

  ofn.lpstrFilter       = m_Filter;
  ofn.lpstrTitle        = bSaveDlg ? "Select File To Save" : "Select File To Open";
  ofn.Flags             = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLEHOOK;
  ofn.FlagsEx           = 0;
  ofn.lStructSize       = sizeof( OPENFILENAME );   
  ofn.hwndOwner         = m_hWnd;
  ofn.hInstance         = m_hInstance;
  ofn.lpstrFile         = fileName;
  ofn.nMaxFile          = MAX_PATH;
  ofn.lpstrInitialDir   = m_FileName.GetDriveDir();
  ofn.lCustData         = (LPARAM)this;
  ofn.nFilterIndex      = 1;

  if (m_bEnableHistory)
  {
    ofn.lpTemplateName    = MAKEINTRESOURCE( IDD_FILEOPEN );
    ofn.lpfnHook          = (LPOFNHOOKPROC)HookProcCB;
    ofn.Flags |= OFN_ENABLETEMPLATE;
  }
  
  bool bRes = false;
  if (bSaveDlg)
  {
    if (GetSaveFileName( &ofn )) 
    {
        bRes = true; 
        m_FileName = ofn.lpstrFile;
    }
  }
  else
  {
    if (GetOpenFileName( &ofn )) 
    {
        bRes = true; 
        m_FileName = ofn.lpstrFile;
    }
  }
  DWORD err = GetLastError();
  return bRes;
} // FileHistoryDlg::Show