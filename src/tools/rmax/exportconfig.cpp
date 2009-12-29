/***********************************************************************************/
//  File:   ExportConfig.cpp
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "ExportConfig.h"

/***********************************************************************************/
/*  ExportConfig implementation
/***********************************************************************************/
decl_class( ExportConfig );

ExportConfig::ExportConfig()
{
    m_bExportModel          = true;
    m_bExportAnim           = true;
    m_bCopyTextures         = true;
    m_bOverwriteTextures    = false;

    m_bPreview              = false;

    m_bExportNormals        = true;
    m_bExportVertexColors   = true;

    m_bVerboseLog           = false;
    m_bShowLog              = true;

    m_Format                = ExportFormat_Binary;

    m_bExportCameras        = true;
    m_bExportAnimSet        = true;
} // ExportConfig::ExportConfig