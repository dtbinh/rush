/***********************************************************************************/
//  File:   RushMax.cpp
//  Desc:   MAX setup related exporter routines
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ISoundServer.h"
#include "IScriptServer.h"
#include "IParticleServer.h"
#include "JCore.h"
#include "JModel.h"
#include "JModelAnim.h"
#include "IPersistServer.h"
#include "JWindowServer.h"
#include "ExportConfig.h"
#include "rmax.h"

HINSTANCE g_hInstance;
bool      g_bInited = false;

#define XMAX_CLASS_ID   Class_ID(0x5eac182b, 0x3d793463)

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) 
{
    g_hInstance = hinstDLL;

    if (!g_bInited) 
    {
        g_bInited = true;
        InitCustomControls( g_hInstance );
        InitCommonControls();

        char moduleFile[_MAX_PATH];
        GetModuleFileName( hinstDLL, moduleFile, _MAX_PATH );
        Path path( moduleFile );
        path.SetFile( NULL );
        path.SetExt( NULL );
    }
    
    return (TRUE);
} // DllMain


__declspec( dllexport ) const TCHAR* LibDescription() 
{
    return ".rmdl exporter";
}

/// MUST CHANGE THIS NUMBER WHEN ADD NEW CLASS 
__declspec( dllexport ) int LibNumberClasses() 
{
    return 1;
}


class RBExportClassDesc : public ClassDesc 
{
public:
    int             IsPublic    () { return 1; }
    void*           Create      ( BOOL loading = FALSE ) { return new RBExport; } 
    const TCHAR*    ClassName   () { return "RushMax"; }
    SClass_ID       SuperClassID() { return UTILITY_CLASS_ID; } 
    Class_ID        ClassID     () { return XMAX_CLASS_ID; }
    const TCHAR*    Category    () { return "Standard"; }
};

static RBExportClassDesc s_XMaxDesc;
__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
    switch(i) 
    {
    case 0: return &s_XMaxDesc;
    default: return 0;
    }
}

__declspec( dllexport ) ULONG LibVersion() 
{
    return VERSION_3DSMAX;
}

TCHAR *GetString(int id)
{
    static TCHAR buf[256];

    if (g_hInstance)
        return LoadString(g_hInstance, id, buf, sizeof(buf)) ? buf : NULL;

    return NULL;
}

/***********************************************************************************/
/*  RBExport implementation
/***********************************************************************************/
RBExport::RBExport()
{
    m_pModel                = NULL;
    m_pAnimation            = NULL; 
    m_pPhysicsGroup         = NULL;

    m_hOptionsWnd           = NULL;
    m_hLogWnd               = NULL;
    m_hLog                  = NULL;
    m_hProgress             = NULL;
    m_hExportModel          = NULL;
    m_hModelPath            = NULL;
    m_hBrowseModel          = NULL;
    m_hExportAnim           = NULL;
    m_hAnimPath             = NULL;
    m_hBrowseAnim           = NULL;
    m_hReduceKeys           = NULL;
    m_hCopyTextures         = NULL;
    m_hOverwriteTextures    = NULL;
    m_hPreview              = NULL;
    m_hBtnExport            = NULL;
    m_hBtnHelp              = NULL;
    m_hBtnAbout             = NULL;

    m_WorldScale            = 1.0f;
    m_bBreakOnError         = false;

    m_bCanceled             = false;
    m_NErrors               = 0;
    m_NWarnings             = 0;

    m_pUtil                 = NULL;
    m_pInterface            = NULL;

    m_bHasKeyboardInput     = false;

    import(rb_core);
    import(rb_draw);
    import(rb_physics);
    import(rb_scene);

    link_class( ExportConfig );

    JCore* pCore = new JCore();
    pCore->Init();

    m_pConfig = new ExportConfig();
} // RBExport::RBExport

RBExport::~RBExport()
{
    m_pConfig->Release();
}

int RBExport::ExtCount()
{
    return 2;
}

const TCHAR * RBExport::Ext(int n)
{
    switch(n) {
    case 0:
        return _T( "rmdl" );
    case 1:
        return _T( "ranm" );
    }
    return _T("");
}

const TCHAR * RBExport::LongDesc()
{
    return "Rush Engine model file exporter";
}

const TCHAR * RBExport::ShortDesc()
{
    return ".rmdl exporter";
}

const TCHAR * RBExport::AuthorName() 
{
    return _T("Ruslan Shestopalyuk");
}

const TCHAR * RBExport::CopyrightMessage() 
{
    return "Copyright (C)2004-2007 Rush";
}

const TCHAR * RBExport::OtherMessage1() 
{
    return _T("");
}

const TCHAR * RBExport::OtherMessage2() 
{
    return _T("");
}

unsigned int RBExport::Version()
{
    return 100;
}

void RBExport::StartExport()
{
    m_bShowPrompts    = false;
    m_bExportSelected = false;

    m_SourcePath = (const char*)m_pInterface->GetCurFilePath();

    m_NumNodesTotal = 0;
    m_CurNode       = 0;
    m_IndentLevel   = 0;

    //  make first pass traversing scene, gather materials, number of nodes etc.
    Msg( LogType_Info, "Exporting model..." );
    PreProcessScene();
    
    //  create new model instance
    m_pModel = new JModel();
    m_pModel->SetVisible( false );
    m_pModel->SetName( m_ModelPath.GetFile() );

    //  create new animation instance
    m_pAnimation = new JModelAnim();
    m_pAnimation->SetVisible( false );
    m_pAnimation->SetName( m_AnimPath.GetFile() );

    m_AnimRange = m_pInterface->GetAnimRange();

    //  extract animation set metadata
    ExtractAnimSet();

    m_pPhysicsGroup = NULL;
    
    float anmDuration = TimeToMs( m_AnimRange.End() - m_AnimRange.Start() )*0.001f;
    m_pAnimation->SetDuration( anmDuration );
    
    int numChildren = m_pInterface->GetRootNode()->NumberOfChildren();
    
    for (int idx = 0; idx < numChildren; idx++) 
    {
        if (m_pInterface->GetCancel())
        {
            break;
        }
        Process( m_pInterface->GetRootNode()->GetChildNode( idx ) );
    }

    if (IsCanceled())
    {
        m_IndentLevel = 0;
        Info( "MESH EXPORT WAS CANCELED!!!" );
        return;
    }
    
    //  create material nodes for model
    CreateMaterialTable();

    //  create mesh nodes for model
    CreateMeshes();

    //  add physics group, if exists
    if (m_pPhysicsGroup)
    {
        m_pModel->AddChild( m_pPhysicsGroup );
    }
    
    if (strlen( m_ModelPath.GetDriveDir() ) == 0)
    {
        m_ModelPath.SetDirectory( m_ExportFolder.GetDriveDir() );
    }
    Path destPath = m_ModelPath;
    Path logPath;
    //  save model
    if (m_pConfig->m_bExportModel)
    {
        destPath.SetExt( "rmdl" );

        logPath = destPath;
        logPath.Normalize();
        Info( "Writing model file: %s", logPath.GetFullPath() );

        FOutStream os( destPath.GetFullPath() );
        g_pPersistServer->Save( m_pModel, os, PersistFmt_JML );
        os.Close();

        int nMdlBytes = destPath.GetFileSize();

        m_pModel->GatherChildren();
        Msg( LogType_Stats, "Final model has %d faces, %d vertices, %d transforms, %d meshes and %d materials.",
            m_pModel->GetNFaces(), m_pModel->GetNVertices(), m_pModel->GetNBones(), m_pModel->GetNMeshes(),
            m_pModel->GetNMaterials() );
        Msg( LogType_Stats, "Model size is %d Kb.", nMdlBytes/1024 );
    }
    else
    {
        m_pModel->Release();
        m_pModel = NULL;
        //  open model from given location
        if (!FileExists( m_ModelPath.GetFullPath() ))
        {
            Err( "Supplied model file does not exist: %s", m_ModelPath.GetFullPath() );
        }
        else
        {
            m_pModel = obj_cast<JModel>( 
                g_pPersistServer->LoadObject( m_ModelPath.GetFullPath() ) );
            if (m_pModel == NULL)
            {
                Err( "Invalid model file: %s", m_ModelPath.GetFullPath() );
            }
        }
        if (m_pModel != NULL)
        {
            logPath = m_ModelPath;
            logPath.Normalize();
            Info( "Using model file: %s", logPath.GetFullPath() );

            //  check whether skeleton can be attached for this animation
            CheckAnimation();
        }
    }

    //  save animation
    if (strlen( m_AnimPath.GetDriveDir() ) == 0)
    {
        m_AnimPath.SetDirectory( m_ExportFolder.GetDriveDir() );
    }
    if (m_pConfig->m_bExportAnim)
    {
        destPath = m_AnimPath;
        destPath.SetExt( "ranm" );
        logPath = destPath;
        logPath.Normalize();
        Info( "Writing animation file: %s", logPath.GetFullPath() );

        m_pAnimation->SetModelName( m_ModelPath.GetFile() );
        FOutStream os( destPath.GetFullPath() );
        g_pPersistServer->Save( m_pAnimation, os, PersistFmt_JML );
        os.Close();
        Msg( LogType_Stats, "Animation size is %d Kb.", destPath.GetFileSize()/1024 );
    }

    //  copy texture files to exported model's location
    if (m_pConfig->m_bCopyTextures)
    {
        CopyTextures();
    }

    //  write the current options to be used next time around.
    WriteConfig();

    //  finish progress bar
    SetProgress( 1.0f );

    Msg( LogType_Info, "Export done: %d error(s), %d warning(s).", m_NErrors, m_NWarnings );

    if (m_pConfig->m_bPreview)
    {
        if (RunPreviewer())
        {
            Info( "Running previewer..." );
        }
        else
        {
            Err( "Could not run previewer. Please, try to reinstall RushEngine tools."
                 "If it does not help, please report as error." );
        }
    }
    
    //  clean data structures, used during export
    Cleanup();
} // RBExport::StartExport

const char* c_CfgFileName = _T( "RushMax.rjml" );

TSTR RBExport::GetCfgFilename()
{
    TSTR filename;
    
    filename += m_pInterface->GetDir( APP_PLUGCFG_DIR );
    filename += "\\";
    filename += c_CfgFileName;

    return filename;
} // RBExport::GetCfgFilename

bool RBExport::ReadConfig()
{
    TSTR fileName = GetCfgFilename();
    ExportConfig* pConfig = obj_cast<ExportConfig>( 
        g_pPersistServer->LoadObject( fileName, m_pConfig ) );
    return (pConfig != NULL);
} // RBExport::ReadConfig

void RBExport::WriteConfig()
{
    m_pConfig->m_PrevModelPath = m_ModelPath.GetFullPath();
    m_pConfig->m_PrevAnimPath  = m_AnimPath.GetFullPath();
    m_pConfig->m_PrevFolder    = m_ExportFolder.GetFullPath();

    TSTR filename = GetCfgFilename();
    FOutStream hos( filename );
    g_pPersistServer->Save( m_pConfig, hos, PersistFmt_JML );  
} // RBExport::WriteConfig

void RBExport::BeginEditParams( Interface *ip, IUtil *iu ) 
{
    m_pUtil       = iu;
    m_pInterface  = ip;
    m_SourcePath  = (const char*)m_pInterface->GetCurFilePath();
    
    //  get last options
    ReadConfig();
    SetDefaultPaths();

    m_hOptionsWnd = ip->AddRollupPage( g_hInstance, MAKEINTRESOURCE( IDD_OPTIONS ),
        OptDlgProc, "Parameters", (LPARAM)this );
}
        
void RBExport::EndEditParams( Interface *ip, IUtil *iu ) 
{
    m_pUtil = NULL;
    ip->DeleteRollupPage( m_hOptionsWnd );
    m_hOptionsWnd = NULL;
}

