/***********************************************************************************/
//  File:   dialog.cpp
//  Desc:   Exporter dialog window management code
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "shellapi.h"
#include "FileHistoryDlg.h"
#include "ExportConfig.h"
#include "rmax.h"
#include "shlobj.h"

/***********************************************************************************/
/*  Constants
/***********************************************************************************/
//  number of spaces to indent into log with each level 
const int c_IndentSize = 2;
//  log auxiliary buffer size
const int c_LogBufferSize = 2048;

/***********************************************************************************/
/*  Utilities
/***********************************************************************************/
bool IsChecked( HWND hWnd )
{
    return (SendMessage( hWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);
} // IsChecked

void SetCheck( HWND hWnd, bool bChecked )
{
    SendMessage( hWnd, BM_SETCHECK, 
                 bChecked ? (WPARAM)BST_CHECKED : (WPARAM)BST_UNCHECKED, 0 );
} // SetCheck

void SelectItem( HWND hWnd, int itemIdx )
{
    SendMessage( hWnd, CB_SETCURSEL, (WPARAM)itemIdx, 0 );
} // SelectItem

int GetSelectedItem( HWND hWnd )
{
    return SendMessage( hWnd, CB_GETCURSEL, 0, 0 );
} // GetSelectedItem


int CALLBACK BrowseForFolderCallback( HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData )
{
    char szPath[MAX_PATH];
    switch (uMsg)
    {
    case BFFM_INITIALIZED:
        SendMessage( hwnd, BFFM_SETSELECTION, TRUE, pData );
        break;

    case BFFM_SELCHANGED: 
        if (SHGetPathFromIDList( (LPITEMIDLIST) lp ,szPath )) 
        {
            SendMessage( hwnd, BFFM_SETSTATUSTEXT,0,(LPARAM)szPath ); 
        }
        break;
    }
    return 0;
} // BrowseForFolderCallback

BOOL BrowseFolders( HWND hwnd, LPSTR lpszFolder, LPSTR lpszTitle )
{
    BROWSEINFO bi;
    char szPath[MAX_PATH + 1];
    LPITEMIDLIST pidl;
    BOOL bResult = FALSE;

    LPMALLOC pMalloc;

    if (SUCCEEDED(SHGetMalloc( &pMalloc ))) 
    {
        bi.hwndOwner        = hwnd;
        bi.pidlRoot         = NULL;
        bi.pszDisplayName   = NULL;
        bi.lpszTitle        = lpszTitle;
        bi.ulFlags          = BIF_STATUSTEXT | BIF_USENEWUI;
        bi.lpfn             = BrowseForFolderCallback;
        bi.lParam           = (LPARAM)lpszFolder;
        
        pidl = SHBrowseForFolder( &bi );
        if (pidl)
        {
            if (SHGetPathFromIDList( pidl,szPath )) 
            {
                bResult = TRUE;
                strcpy( lpszFolder, szPath );
                int nChar = strlen( lpszFolder );
                if (nChar > 0 && lpszFolder[nChar - 1] != '\\' && lpszFolder[nChar - 1] != '/')
                {
                    strcat( lpszFolder, "\\" );
                }
            }
            pMalloc->Free( pidl );
            pMalloc->Release();
                        
        }
    }
    return bResult;
} // BrowseFolders


/***********************************************************************************/
/*  RBExport implementation
/***********************************************************************************/
INT_PTR CALLBACK RBExport::AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) 
    {
    case WM_INITDIALOG:
        CenterWindow(hWnd, GetParent(hWnd)); 
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        {
        case IDOK:
            EndDialog(hWnd, 1);
            break;
        }
        break;
        default:
            return FALSE;
    }
    return TRUE;
} // RBExport::AboutDlgProc

void RBExport::ShowAbout( HWND hWnd )
{
    DialogBoxParam( g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc, 0 );
} // RBExport::ShowAbout

const char* c_MessageTypeColor[] = 
{
"{\\colortbl;\\red0\\green0\\blue0;}",
"{\\colortbl;\\red0\\green0\\blue255;}",
"{\\colortbl;\\red0\\green0\\blue0;}",
"{\\colortbl;\\red255\\green150\\blue128;}",
"{\\colortbl;\\red255\\green0\\blue0;}",
"{\\colortbl;\\red0\\green128\\blue0;}",
};

const char* c_MessageTypeStr[] = 
{
    "",
    "",
    "",
    "WARNING: ",
    "ERROR: ",
    ""
};

void RBExport::Log( LogType type, const char* format, va_list argList )
{
    char buf[c_LogBufferSize];
    //  print message type
    sprintf( buf, c_MessageTypeStr[type] );
    int cChar = strlen( buf );

    //  insert indents
    if (cChar == 0) 
    {
        int nSpaces = c_IndentSize*m_IndentLevel;
        memset( buf + cChar, ' ', nSpaces );
        cChar += nSpaces;
    }

    //  print message into buffer
    vsprintf( buf + cChar, format, argList );

    std::string str = "{\\rtf ";
    str += c_MessageTypeColor[type]; 
    str += "{\\par\\pard \\cf1";
    str += buf;
    str += "}}\n";

    SendMessage( m_hLog, EM_SETSEL, -1, -1 );
    SendMessage( m_hLog, EM_REPLACESEL, 0, (LPARAM)str.c_str() );
    SendMessage( m_hLog, EM_SCROLL, SB_PAGEDOWN, 0 );

    UpdateWindow( m_hLogWnd );
}

void RBExport::Msg( LogType type, const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    Log( type, format, argList );
    va_end( argList );
} // RBExport::Msg

void RBExport::Spam( const char* format, ... )
{
	if (m_pConfig->m_bVerboseLog == false) return;
    va_list argList;
    va_start( argList, format );
    Log( LogType_Spam, format, argList );
    va_end( argList );
} // RBExport::Msg

void RBExport::Info( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    Log( LogType_Info, format, argList );
    va_end( argList );
} // RBExport::Msg

void RBExport::Err( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    Log( LogType_Error, format, argList );
    va_end( argList );
    m_NErrors++;
} // RBExport::Err

void RBExport::Warn( const char* format, ... )
{
    va_list argList;
    va_start( argList, format );
    Log( LogType_Warning, format, argList );
    va_end( argList );
    m_NWarnings++;
} // RBExport::Warn

void RBExport::GetOptionsFromDialog()
{
    m_pConfig->m_bExportModel          = IsChecked( m_hExportModel );
    m_pConfig->m_bExportAnim           = IsChecked( m_hExportAnim  );
    m_pConfig->m_bPreview              = IsChecked( m_hPreview     );
    m_pConfig->m_bCopyTextures         = IsChecked( m_hCopyTextures );
    m_pConfig->m_bOverwriteTextures    = IsChecked( m_hOverwriteTextures );
    m_pConfig->m_bExportCameras        = IsChecked( m_hExportCameras );
    m_pConfig->m_bExportAnimSet        = IsChecked( m_hExportAnimSet );

    m_pConfig->m_Format = (ExportFormat)GetSelectedItem( m_hFormat );
    
    char path[_MAX_PATH];

    GetWindowText( m_hFolderPath, path, _MAX_PATH );
    m_ExportFolder = path;

    GetWindowText( m_hModelPath, path, _MAX_PATH );
    Path modelPath( path );
    Path modelFolder;
    modelFolder.SetDirectory( modelPath.GetDriveDir() );
    if (m_ExportFolder.SameFolder( modelPath ) || !modelFolder.Exists())
    {
        m_ModelPath.SetFile( modelPath.GetFile() );
    }
    else
    {
        m_ModelPath = modelPath;
    }
    
    GetWindowText( m_hAnimPath, path, _MAX_PATH );
    Path animPath( path );
    Path animFolder;
    animFolder.SetDirectory( animPath.GetDriveDir() );
    if (m_ExportFolder.SameFolder( animPath ) || !animFolder.Exists())
    {
        m_AnimPath.SetFile( animPath.GetFile() );
    }
    else
    {
        m_AnimPath = animPath;
    }

} // RBExport::GetOptionsFromDialog

void RBExport::SetOptionsToDialog()
{
    SetCheck( m_hExportModel,       m_pConfig->m_bExportModel          );
    SetCheck( m_hExportAnim,        m_pConfig->m_bExportAnim           );
    SetCheck( m_hPreview,           m_pConfig->m_bPreview              );
    SetCheck( m_hOverwriteTextures, m_pConfig->m_bOverwriteTextures    );
    SetCheck( m_hExportCameras,     m_pConfig->m_bExportCameras        );
    SetCheck( m_hExportAnimSet,     m_pConfig->m_bExportAnimSet        );

    SelectItem( m_hFormat, (int)m_pConfig->m_Format );

    SetWindowText( m_hFolderPath, m_ExportFolder.GetFullPath() );

    if (m_ExportFolder.SameFolder( m_ModelPath ))
    {
        SetWindowText( m_hModelPath, m_ModelPath.GetFileExt() );
    }
    else
    {
        SetWindowText( m_hModelPath, m_ModelPath.GetFullPath() );
    }

    if (m_ExportFolder.SameFolder( m_AnimPath ))
    {
        SetWindowText( m_hAnimPath, m_AnimPath.GetFileExt() );
    }
    else
    {
        SetWindowText( m_hAnimPath, m_AnimPath.GetFullPath() );
    }   

    EnableWindow( m_hBrowseAnim,        m_pConfig->m_bExportAnim   ? TRUE : FALSE );
    EnableWindow( m_hOverwriteTextures, m_pConfig->m_bCopyTextures ? TRUE : FALSE );
    EnableWindow( m_hReduceKeys,        m_pConfig->m_bExportAnim   ? TRUE : FALSE );

    if (!m_pConfig->m_bExportAnim && !m_pConfig->m_bExportModel)
    {
        EnableWindow( m_hBtnExport, FALSE );
    }
    else if (!m_pConfig->m_bExportModel && !FileExists( m_ModelPath.GetFullPath() ))
    {
        EnableWindow( m_hBtnExport, FALSE );
    }
    else 
    {
        EnableWindow( m_hBtnExport, TRUE );
    }
} // RBExport::SetOptionsToDialog

void RBExport::OpenLogWindow()
{
    m_hLogWnd     = NULL;
    DialogBoxParam( g_hInstance, MAKEINTRESOURCE( IDD_LOG ),
                        m_pInterface->GetMAXHWnd(), LogDlgProc, (LPARAM)this );
} // RBExport::OpenLogWindow

INT_PTR CALLBACK RBExport::OptDlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    RBExport* pExporter = NULL;
    if (msg == WM_INITDIALOG)
    {
        pExporter = (RBExport*)lParam;
        SetWindowLongPtr( hWnd, GWLP_USERDATA, LONG( lParam ) );
    }
    else
    {
        pExporter = (RBExport*)GetWindowLongPtr( hWnd, GWLP_USERDATA ); 
    }

    if (pExporter)
    {
        return pExporter->DispatchOptDlg( hWnd, msg, wParam, lParam );
    }
    return TRUE;
} // RBExport::OptDlgProc

int RBExport::DispatchOptDlg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    //  check whether we need to update default paths
    if (strcmp( m_SourcePath.GetFullPath(), (const char*)m_pInterface->GetCurFilePath() ))
    {
        m_SourcePath  = (const char*)m_pInterface->GetCurFilePath();
        SetDefaultPaths();
        SetOptionsToDialog();
    }

    switch (msg) 
    {
    case WM_INITDIALOG:

        //  get handles to the dialog control windows
        m_hOptionsWnd           = hWnd;
        m_hExportModel          = GetDlgItem( hWnd, IDC_EXPORTMODEL         );
        m_hModelPath            = GetDlgItem( hWnd, IDC_MODELPATH           );
        m_hBrowseModel          = GetDlgItem( hWnd, IDC_MODELBROWSE         );
        m_hExportAnim           = GetDlgItem( hWnd, IDC_EXPORTANIM          );
        m_hAnimPath             = GetDlgItem( hWnd, IDC_ANIMPATH            );
        m_hBrowseAnim           = GetDlgItem( hWnd, IDC_ANIMBROWSE          );
        m_hReduceKeys           = GetDlgItem( hWnd, IDC_REDUCEKEYS          );
        m_hCopyTextures         = GetDlgItem( hWnd, IDC_COPYTEXTURES        );
        m_hOverwriteTextures    = GetDlgItem( hWnd, IDC_OVERWRITETEXTURES   );
        m_hExportCameras        = GetDlgItem( hWnd, IDC_EXPORTCAMERAS       );
        m_hExportAnimSet        = GetDlgItem( hWnd, IDC_EXPORTANIMSET       );
    
        m_hPreview              = GetDlgItem( hWnd, IDC_PREVIEW             );
        m_hBtnExport            = GetDlgItem( hWnd, IDC_EXPORT              );
        m_hBtnHelp              = GetDlgItem( hWnd, IDC_HELP                );
        m_hFormat               = GetDlgItem( hWnd, IDC_FORMAT              );
        m_hFolderPath           = GetDlgItem( hWnd, IDC_FOLDERPATH          ); 
        m_hBrowseFolder         = GetDlgItem( hWnd, IDC_FOLDERBROWSE        ); 

        ::SendMessage( m_hFormat, CB_ADDSTRING, 0, (LPARAM)"Binary" );
        ::SendMessage( m_hFormat, CB_ADDSTRING, 0, (LPARAM)"Hybrid" );
        ::SendMessage( m_hFormat, CB_ADDSTRING, 0, (LPARAM)"Text"   );

        SetOptionsToDialog();
                
        break;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam)) 
        {
        case IDOK:
            EndDialog( hWnd, 1 );
            break;

        case IDC_EXPORT:
            GetOptionsFromDialog();
            OpenLogWindow();             
            break;
        
        case IDC_FOLDERBROWSE:
            {
                GetOptionsFromDialog();
                //  get the name of the folder
                TCHAR path[MAX_PATH];
                strcpy( path, m_ExportFolder.GetFullPath() );
                if (BrowseFolders( m_hOptionsWnd, path, "Select Folder to Contain Exported Files" ))
                {
                    m_ExportFolder = path;
                    if (m_ExportFolder.Exists())
                    {
                        m_ModelPath.SetDirectory( m_ExportFolder.GetDriveDir() );
                        m_AnimPath.SetDirectory( m_ExportFolder.GetDriveDir() );
                    }
                }
                SetOptionsToDialog();
            }
            break;

        case IDC_MODELBROWSE:
            {
                GetOptionsFromDialog();
                FileHistoryDlg dlg( hWnd, g_hInstance );
                //  set history
                for (int i = 0; i < m_ModelHistory.size(); i++)
                {
                    dlg.AddHistoryItem( m_ModelHistory[i].c_str() );
                }
                
                dlg.EnableHistory( false );
                dlg.AddFilter( "Model Files", "rmdl" );
                dlg.AddFilter( "All", "*" );
                dlg.SetFileName( m_ModelPath.GetFullPath() );
                if (dlg.Show( m_pConfig->m_bExportModel ))
                {
                    m_ModelPath = dlg.GetFileName();
                    m_ModelPath.SetExt( "rmdl" );
                }

                //  get history
                m_ModelHistory.clear();
                int nItems = dlg.GetNumHistoryItems();
                for (int i = 0; i < nItems; i++)
                {
                    m_ModelHistory.push_back( dlg.GetHistoryItem( i ) );
                }
                SetOptionsToDialog();
            }
            break;

        case IDC_ANIMBROWSE:
            {
                GetOptionsFromDialog();
                FileHistoryDlg dlg( hWnd, g_hInstance );
                //  set history
                for (int i = 0; i < m_AnimHistory.size(); i++)
                {
                    dlg.AddHistoryItem( m_AnimHistory[i].c_str() );
                }
                
                dlg.EnableHistory( false );
                dlg.AddFilter( "Animation Files", "ranm" );
                dlg.AddFilter( "All", "*" );
                dlg.SetFileName( m_AnimPath.GetFullPath() );
                
                if (m_pConfig->m_bExportModel)
                {
                    Path defPath( m_ModelPath );
                    defPath.SetFileExt( m_AnimPath.GetFileExt() );
                }
                if (dlg.Show( m_pConfig->m_bExportAnim ))
                {
                    m_AnimPath = dlg.GetFileName();
                    m_AnimPath.SetExt( "ranm" );
                }

                //  get history
                m_AnimHistory.clear();
                int nItems = dlg.GetNumHistoryItems();
                for (int i = 0; i < nItems; i++)
                {
                    m_AnimHistory.push_back( dlg.GetHistoryItem( i ) );
                }
                SetOptionsToDialog();
            }
            break;

        case IDC_HELP:
            ShowHelp();
            break;

        case IDC_EXPORTMODEL:
        case IDC_EXPORTANIM:
        case IDC_REDUCEKEYS:
        case IDC_COPYTEXTURES:
            GetOptionsFromDialog();
            SetOptionsToDialog();
            break;
        case IDCLOSE:
        case IDCANCEL:
            EndDialog( hWnd, 1 );
            return TRUE;
        case IDC_FOLDERPATH:
        case IDC_ANIMPATH:
        case IDC_MODELPATH:
            {
                //  because our utility dialog is modeless, we must disable MAX accelerators in order
                //  to recieve input from our edit boxes
                WORD notifyID = HIWORD( wParam );
                if (notifyID == EN_SETFOCUS)
                {
                    DisableAccelerators();
                }
                else if (notifyID == EN_KILLFOCUS)
                {
                    EnableAccelerators();
                }
            }
            break;
        }
    }break;
    case WM_QUIT:
        EndDialog( hWnd, 1 );
        break;
    case WM_KEYDOWN:
        if (m_bHasKeyboardInput)
        {
            return TRUE;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
} // RBExport::DispatchOptDlg

INT_PTR CALLBACK RBExport::LogDlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    RBExport* pExporter = NULL;
    if (msg == WM_INITDIALOG)
    {
        pExporter = (RBExport*)lParam;
        SetWindowLongPtr( hWnd, GWLP_USERDATA, LONG( lParam ) );
        ShowWindow( hWnd, SW_SHOW );
    }
    else
    {
        pExporter = (RBExport*)GetWindowLongPtr( hWnd, GWLP_USERDATA ); 
    }

    if (pExporter)
    {
        return pExporter->DispatchLogDlg( hWnd, msg, wParam, lParam );
    }
    return TRUE;
} // RBExport::LogDlgProc

int RBExport::DispatchLogDlg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch (msg) 
    {
    case WM_INITDIALOG:

        //  get handles to the dialog control windows
        m_hLogWnd       = hWnd;
        m_hLog          = GetDlgItem( hWnd, IDC_LOG         );
        m_hProgress     = GetDlgItem( hWnd, IDC_PROGRESS    );
                
        //  set horizontal extent of the scrollable area of the log list box
        SendMessage( m_hLog,  LB_SETHORIZONTALEXTENT, 1024, 0 );
        
        StartExport();

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        {
        case IDCLOSE:
            m_bCanceled = true;
            EndDialog( hWnd, 1 );
            break;
        }
        break;
    case WM_QUIT:
    case WM_CLOSE:
        EndDialog( hWnd, 1 );
        break;

    default:
        return FALSE;
    }

    return TRUE;
} // RBExport::DispatchLogDlg

void RBExport::SetProgress( float progress )
{
    int pos = (int)(progress*100.0f);
    SendMessage( m_hProgress, PBM_SETPOS, (WPARAM)pos, 0 );
    UpdateWindow( m_hLogWnd );

    MSG  msg;
    while (PeekMessage( &msg, m_hLogWnd, 0U, 0U, PM_REMOVE ) != 0)
    {
       TranslateMessage( &msg );
       DispatchMessage( &msg );
    }
} // RBExport::SetProgress

void RBExport::Indent( bool bIndent )
{
    m_IndentLevel += bIndent ? c_IndentSize : -c_IndentSize;
    if (m_IndentLevel < 0)
    {
        m_IndentLevel = 0;
    }
} // RBExport::Indent

bool RBExport::GetToolsInstallDir( std::string& path )
{
    char strPath[_MAX_PATH];
    DWORD nDataSize = _MAX_PATH;
    static const char* c_RegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\RushTools";
    HKEY hKey = NULL;
    LONG res = RegOpenKeyEx( HKEY_LOCAL_MACHINE, c_RegPath, 0, KEY_READ, &hKey ); 
    if (hKey == NULL)
    {
        return false;
    }

    res = RegQueryValueEx( hKey, "InstallDir", NULL, NULL, (BYTE*)strPath, &nDataSize );
    RegCloseKey( hKey );
    if (res != ERROR_SUCCESS)
    {
        return false;
    }

    path = strPath;
    return true;
} // RBExport::GetToolsInstallDir

bool RBExport::RunPreviewer()
{
    std::string strParam;
    strParam += "--model=\"";
    strParam += m_ModelPath.GetFullPath();
    strParam += "\" --anim=\"";
    strParam += m_AnimPath.GetFullPath();
    strParam += "\"";

    std::string path;
    if (!GetToolsInstallDir( path ))
    {
        return false;
    }
    
    path += "\\rboot.exe";
    ShellExecute( NULL, "open", path.c_str(), strParam.c_str(), m_ModelPath.GetDirectory(), SW_SHOWNORMAL );
    return true;
} // RBExport::RunPreviewer

bool RBExport::ShowHelp()
{
    std::string strParam;
    strParam = m_ModelPath.GetFullPath();
    strParam += " ";
    strParam = m_AnimPath.GetFullPath();

    std::string path;
    if (!GetToolsInstallDir( path ))
    {
        return false;
    }
    
    path += "\\doc\\RushTools.chm";
    ShellExecute( NULL, "open", path.c_str(), strParam.c_str(), NULL, SW_SHOWNORMAL );
    return true;
} // RBExport::ShowHelp

void RBExport::SetDefaultPaths()
{
    if (m_pConfig->m_bExportModel)
    {
        m_ModelPath.SetFile( m_SourcePath.GetFile() );
        if (strlen( m_ModelPath.GetFile() ) == 0) 
        {
            m_ModelPath = m_pConfig->m_PrevModelPath.c_str();
        }
        if (strlen( m_ModelPath.GetFile() ) == 0) 
        {
            m_ModelPath.SetFile( "default" );
        }
        m_ModelPath.SetExt( "rmdl" );
    }
    else
    {
        m_ModelPath = m_pConfig->m_PrevModelPath.c_str();
    }

    m_AnimPath.SetFile( m_SourcePath.GetFile() );
    if (strlen( m_AnimPath.GetFile() ) == 0) 
    {
        m_AnimPath = m_pConfig->m_PrevAnimPath.c_str();
    }    
    if (strlen( m_AnimPath.GetFile() ) == 0) 
    {
        m_AnimPath.SetFile( "default" );
    }
    m_AnimPath.SetExt( "ranm" );
    
    //  setup default export folder
    if (!m_ExportFolder.Exists())
    {
        m_ExportFolder = m_pConfig->m_PrevFolder.c_str();
    }
    if (!m_ExportFolder.Exists())
    {
        m_ExportFolder.SetDirectory( m_ModelPath.GetDriveDir() );
    }
    if (!m_ExportFolder.Exists())
    {
        m_ExportFolder.SetDirectory( m_SourcePath.GetDriveDir() );
    }

    if (m_ExportFolder.Exists())
    {
        m_ModelPath.SetDirectory( m_ExportFolder.GetDriveDir() );
        m_AnimPath.SetDirectory( m_ExportFolder.GetDriveDir() );
    }
} // RBExport::SetDefaultPaths

