/***********************************************************************************/
//  File:   ExportConfig.h
//  Desc:   
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __EXPORTCONFIG_H__
#define __EXPORTCONFIG_H__

enum ExportFormat
{
    ExportFormat_Text   = 0,
    ExportFormat_Binary = 1,
    ExportFormat_Hybrid = 2
}; // enum ExportFormat

enum_beg(ExportFormat)
    enum_val( "Text  ", ExportFormat_Text   ),
    enum_val( "Binary", ExportFormat_Binary ),
    enum_val( "Hybrid", ExportFormat_Hybrid )
enum_end(ExportFormat)

//****************************************************************************/
//  Class:  ExportConfig
//  Desc:   Stores export settings
//****************************************************************************/
class ExportConfig : public JObject
{
    bool            m_bExportModel;
    bool            m_bExportAnim;
    bool            m_bCopyTextures;
    bool            m_bOverwriteTextures;
    bool            m_bExportNormals;     
    bool            m_bExportVertexColors;
    bool            m_bPreview;
	bool			m_bVerboseLog;
	bool			m_bShowLog;
    bool            m_bExportCameras;
    bool            m_bExportAnimSet;
    
    ExportFormat    m_Format;

    JString         m_PrevModelPath;
    JString         m_PrevAnimPath;
    JString         m_PrevFolder;

    friend class    RBExport;
public:
    ExportConfig();

    expose(ExportConfig)
    {
        parent(JObject);
        field( "ExportModel",           m_bExportModel          );
        field( "PrevModelPath",         m_PrevModelPath         );
        field( "ExportAnim",            m_bExportAnim           );
        field( "PrevAnimPath",          m_PrevAnimPath          );
        field( "CopyTextures",          m_bCopyTextures         );
        field( "OverwriteTextures",     m_bOverwriteTextures    );
        field( "ExportNormals",         m_bExportNormals        );
        field( "ExportVertexColors",    m_bExportVertexColors   );
        field( "Preview",               m_bPreview              );
        field( "Format",                m_Format                );
        field( "PrevFolder",            m_PrevFolder            );
        field( "VerboseLog",			m_bVerboseLog           );
        field( "ShowLog",				m_bShowLog				);
        field( "ExportCameras",         m_bExportCameras        );
        field( "ExportAnimSet",         m_bExportAnimSet        );
    }                   
}; // struct ExportConfig

#endif // __ExportConfig_H__
