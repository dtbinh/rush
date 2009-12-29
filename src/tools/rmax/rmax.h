/***********************************************************************************/
//  File:   RushMax.h
//  Desc:   Exporter core class declaration
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __RBEXPORT_H__
#define __RBEXPORT_H__

onst Matrix3 c_FlipTM(  Point3( 0.0f, 1.0f, 0.0f ), 
                        Point3( 1.0f, 0.0f, 0.0f ), 
                        Point3( 0.0f, 0.0f, 1.0f ), 
                        Point3( 0.0f, 0.0f, 0.0f ) );

const Matrix3 c_CamFlipTM(  Point3( 1.0f, 0.0f,  0.0f ), 
                            Point3( 0.0f, 1.0f,  0.0f ), 
                            Point3( 0.0f, 0.0f, -1.0f ), 
                            Point3( 0.0f, 0.0f,  0.0f ) );

const Matrix3 c_IdentityTM(  Point3( 1.0f, 0.0f, 0.0f ), 
                             Point3( 0.0f, 1.0f, 0.0f ), 
                             Point3( 0.0f, 0.0f, 1.0f ), 
                             Point3( 0.0f, 0.0f, 0.0f ) );

const int c_MaxBonesPerSkin = 100;

typedef std::set<ExpVertex*, ExpVertex::Less> VertexSet;
typedef UniformPool<ExpVertex>                VertexPool;
typedef std::vector<ExpVertex*>               VertexPtrArray;
typedef std::vector<JMaterial*>               MaterialArray;
typedef std::vector<ExpNode>                  NodeArray;
typedef std::vector<ExpFace>                  FaceArray;
typedef std::vector<std::string>              StringArray;

class JModel;
class JModelAnim;
class JMaterial;
class ColMesh;
class ExportConfig;
/***********************************************************************************/
//  Class:  RBExport
//  Desc:   Main exporter class
/***********************************************************************************/
class RBExport : public UtilityObj 
{
public:
                            RBExport        ();
                            ~RBExport       ();

    int                     ExtCount        ();             // Number of extensions supported 
    const TCHAR*            Ext             ( int n );      // Extension #n (i.e. "ASC")
    const TCHAR*            LongDesc        ();             // Long ASCII description (i.e. "Ascii Export") 
    const TCHAR*            ShortDesc       ();             // Short ASCII description (i.e. "Ascii")
    const TCHAR*            AuthorName      ();             // ASCII Author name
    const TCHAR*            CopyrightMessage();             // ASCII Copyright message 
    const TCHAR*            OtherMessage1   ();             // Other message #1
    const TCHAR*            OtherMessage2   ();             // Other message #2
    unsigned int            Version         ();             // Version number * 100 (i.e. v3.01 = 301) 
    void                    ShowAbout       ( HWND hWnd );  // Show DLL's "About..." box


    virtual void            BeginEditParams ( Interface *ip, IUtil *iu );
    virtual void            EndEditParams   ( Interface *ip, IUtil *iu );
    virtual void            DeleteThis      () {}


    BOOL                    Process         ( INode* node );
    void                    ProcessMesh     ( INode* node );
    void                    ProcessCamera   ( INode* node );
    void                    ProcessLight    ( INode* node );

    TSTR                    GetCfgFilename  ();
    bool                    ReadConfig      ();
    void                    WriteConfig     ();

    bool                    ShowHelp        ();

protected:
    int                     Dispatch        ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    int                     DispatchLogDlg  ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    int                     DispatchOptDlg  ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    
    void                    StartExport     ();

    enum LogType
    {
        LogType_Unknown     = 0,
        LogType_Info        = 1,
        LogType_Spam        = 2,
        LogType_Warning     = 3,
        LogType_Error       = 4,
        LogType_Stats       = 5
    };

    void                    Log             ( LogType type, const char* format, va_list argList );
    void                    Msg             ( LogType type, const char* format, ... );

	void                    Info            ( const char* format, ... );
    void                    Spam            ( const char* format, ... );
    void                    Err             ( const char* format, ... );
    void                    Warn            ( const char* format, ... );

    void                    SetProgress     ( float progress );
    void                    Indent          ( bool bIndent = true );

    
private:    
    Interface*              m_pInterface;
    IUtil*                  m_pUtil;

    int                     m_NumNodesTotal;
    int                     m_CurNode;
    TimeValue               m_CurTime;
    VertexSet               m_VertexSet;
    VertexPool              m_VertexPool;
    VertexPtrArray          m_Vertices;

	//  export log options
    int                     m_IndentLevel;
    int                     m_NErrors;
    int                     m_NWarnings;

    FaceArray               m_Faces;

    bool                    m_bShowPrompts;
    bool                    m_bExportSelected;

    JModel*                 m_pModel;
    JModelAnim*             m_pAnimation;
    JObject*                m_pPhysicsGroup;

    Path                    m_SourcePath;

    Path                    m_ModelPath;
    Path                    m_AnimPath;
    Path                    m_ExportFolder;

    StringArray             m_Textures;

    Interval                m_AnimRange;

    float                   m_SamplingInterval;
    float                   m_WorldScale;

    //  export options
    ExportConfig*           m_pConfig;

    JStringList             m_ModelHistory;
    JStringList             m_AnimHistory;

    bool                    m_bCanceled;
    bool                    m_bFinished;

    static INT_PTR CALLBACK OptDlgProc   ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    static INT_PTR CALLBACK LogDlgProc   ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    static INT_PTR CALLBACK AboutDlgProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

    //  exporter dialog controls
    HWND                    m_hOptionsWnd;   
    HWND                    m_hLogWnd;    
    HWND                    m_hLog;              
    HWND                    m_hProgress;         
    HWND                    m_hExportModel;      
    HWND                    m_hModelPath;       
    HWND                    m_hBrowseModel;      
    HWND                    m_hExportAnim;       
    HWND                    m_hAnimPath;         
    HWND                    m_hBrowseAnim;       
    HWND                    m_hReduceKeys;       
    HWND                    m_hCopyTextures;     
    HWND                    m_hOverwriteTextures;
    HWND                    m_hExportCameras;
    HWND                    m_hExportAnimSet;
    HWND                    m_hPreview;          
    HWND                    m_hBtnExport;        
    HWND                    m_hBtnHelp;          
    HWND                    m_hBtnAbout;    
    HWND                    m_hFormat;
    HWND                    m_hFolderPath;         
    HWND                    m_hBrowseFolder;   

    bool                    m_bHasKeyboardInput;

    MaterialArray           m_Materials;
    NodeArray               m_Nodes;

    void                    GetOptionsFromDialog();
    void                    SetOptionsToDialog();
    void                    OpenLogWindow();
    
    //  skinning data
    IPhyContextExport*      m_pPhysique;
    ISkin*                  m_pSkin;
    ISkinContextData*       m_pSkinData;

    bool                    m_bBreakOnError;

    ExpNode*                m_pCurExpNode;
    ExpNode*                GetExportedNode     ( INode* node );

    void                    PreProcessNode      ( INode* node );
    void                    PreProcessScene     ();
    Modifier*               FindModifier        ( INode* node, Class_ID modID );
    void                    Cleanup             ();
    bool                    RunPreviewer        ();

    bool                    GetToolsInstallDir  ( std::string& path );

    //  materials 
    void                    ExtractMaterials    ( INode* node );
    bool                    ExtractMaterial     ( Mtl* pMtl );
    JMaterial*              ExtractStdMaterial  ( Mtl* pMtl );
    bool                    ExtractDiffuseMap   ( Mtl* pMtl, JMaterial* pMaterial );
    bool                    ExtractEnvMap       ( Mtl* pMtl, JMaterial* pMaterial );
    void                    CreateMaterialTable ();
    void                    CopyTextures        ();
    bool                    AddTexturePath      ( const char* path );

    int                     ExtractAnimSet      ();

    //  geometry
    ExpVertex*              AddVertex           ( ExpVertex& v );
    int                     AddPolygon          ( ExpVertex* pV0, ExpVertex* pV1, ExpVertex* pV2 );
    bool                    ProcessSkin         ( INode* node );
    void                    GetSkinInfo         ( ExpVertex& v );
    void                    GetPhysiqueBinding  ( ExpVertex& v );
    void                    GetSkinBinding      ( ExpVertex& v );
    void                    PreprocessVertices  ();

    void                    CreateMeshes        ();
    void                    SmoothNormals       ( VertexPtrArray& vertexEntries );
    bool                    InSameMesh          ( const ExpVertex* pVA, const ExpVertex* pVB );
    void                    AddMesh             ( int meshID, int startVByte, int startVert, int nVert, 
                                                    int startFace, int nFaces );
    VertexDeclaration       GetVDecl            ( int startVert, int nVert );
    void                    FillVertexBuffer    ( BYTE* pVert, const VertexDeclaration& vertexDecl, 
                                                  int startVert, int nVert );
    bool                    MeshModStackIsValid ( INode* node );
    bool                    IsCanceled          () const;

    void                    SetDefaultPaths     ();

    //  animation
    void                    ProcessAnimation    ( INode* node );
    Matrix3                 GetLocalTM          ( INode* node, TimeValue t );
    bool                    CheckAnimation      ();

    JObject*                ProcessPhysicsObject( INode* node );
    ColMesh*                GetColMesh          ( INode* node );

}; // class RBExporter

#endif // __RUSHMAX_H__

