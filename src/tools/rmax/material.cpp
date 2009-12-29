/***********************************************************************************/
//  File:   material.cpp
//  Desc:   Material-related exporter part
//  Date:   26.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JModel.h"
#include "Path.h"
#include "shlwapi.h"
#include "ExportConfig.h"
#include "IFileServer.h"
#include "rmax.h"

bool RBExport::AddTexturePath( const char* path )
{
    int nTex = m_Textures.size();
    for (int i = 0; i < nTex; i++)
    {
        if (!stricmp( path, m_Textures[i].c_str() ))
        {
            return true;
        }
    }   
    m_Textures.push_back( path );
    return false;
} // AddTexturePath

bool RBExport::ExtractDiffuseMap( Mtl* pMtl, JMaterial* pMaterial )
{
    if (!pMtl || !pMaterial) return false;
    float amount = 1.0;
    bool bHasDifmap = false;
    StdMat* pStdMat     = (StdMat*)pMtl;

    Texmap* pDiffuseMap = pMtl->GetSubTexmap( ID_DI );
    Path diffPath;
    if (pDiffuseMap && pStdMat->MapEnabled( ID_DI ))
    {
        amount = pStdMat->GetTexmapAmt( ID_DI, 0 );
        if (pDiffuseMap->ClassID() == Class_ID( BMTEX_CLASS_ID, 0 ))
        {
            BitmapTex* pBitmapTex = (BitmapTex*)pDiffuseMap;
            diffPath.SetPath( pBitmapTex->GetMapName() );
            AddTexturePath( diffPath.GetFullPath() );
            pMaterial->SetDiffuseMap( diffPath.GetFileExt() );
            bHasDifmap = true;
        }
    }

    //  make some sanity checks
    Texmap* pAlphaMap = pMtl->GetSubTexmap( ID_OP );
    if (pAlphaMap && pStdMat->MapEnabled( ID_OP ) && 
        pAlphaMap->ClassID() == Class_ID( BMTEX_CLASS_ID, 0 ))
    {
        BitmapTex* pAlphaTex = (BitmapTex*)pAlphaMap;
        Path alphaPath( pAlphaTex->GetMapName() );
        if (stricmp( alphaPath.GetFileExt(), diffPath.GetFileExt() ))
        {
            Warn( "Material '%s' has opacity map in a different file than diffuse map, not in diffuse alpha channel.",
                pMtl->GetName() );
        }
    }

    return bHasDifmap;
} // RBExport::ExtractDiffuseMap

bool RBExport::ExtractEnvMap( Mtl* pMtl, JMaterial* pMaterial )
{
    if (!pMtl || !pMaterial) return false;
    bool bHasEnvMap = false;
    StdMat* pStdMat = (StdMat*)pMtl;
    Texmap* pEnvMap = pMtl->GetSubTexmap( ID_RL );
    if (pEnvMap && pStdMat->MapEnabled( ID_RL ))
    {
        float amount = pStdMat->GetTexmapAmt( ID_RL, 0 );
        if (pEnvMap->ClassID() == Class_ID( BMTEX_CLASS_ID, 0 ))
        {
            BitmapTex* pBitmapTex = (BitmapTex*)pEnvMap;
            Path envPath( pBitmapTex->GetMapName() );
            AddTexturePath( envPath.GetFullPath() );
            pMaterial->SetEnvMap( envPath.GetFileExt() );
            bHasEnvMap = true;
        }
    }

    if (bHasEnvMap)
    {
        pMaterial->SetShaderName( "reflective_model" );
    }
    return bHasEnvMap;
} // RBExport::ExtractEnvMap

JMaterial* RBExport::ExtractStdMaterial( Mtl* pMtl )
{
    StdUVGen* pUVGen = NULL;
    StdMat*   pStdMat = (StdMat*)pMtl;
    if (!pMtl) return NULL;

    bool bTwoSided = (pStdMat->GetTwoSided() != 0);

    JMaterial* pResMaterial = new JMaterial();
    pResMaterial->SetShaderName( "base_model" );
    
    pResMaterial->SetDiffuse ( ColorToDWORD( pStdMat->GetDiffuse ( m_CurTime ) ) );
    pResMaterial->SetSpecular( ColorToDWORD( pStdMat->GetSpecular( m_CurTime ) ) );

    ExtractDiffuseMap( pMtl, pResMaterial );
    ExtractEnvMap( pMtl, pResMaterial );
    return pResMaterial;
} // RBExport::ExtractStdMaterial

bool RBExport::ExtractMaterial( Mtl* pMtl )
{
    if (!pMtl) 
    {
        return false;
    }

    Class_ID mtlClassID = pMtl->ClassID();
    JMaterial* pMaterial = NULL;

    //  standard material
    if (mtlClassID == Class_ID( DMTL_CLASS_ID, 0 ))
    {
        pMaterial = ExtractStdMaterial( pMtl );
    }
    
    //  complex material
    if (mtlClassID == Class_ID( MULTI_CLASS_ID, 0 ))
    {
        int nSubMtl = pMtl->NumSubMtls();
        bool bRes = false;
        for (int i = 0; i < nSubMtl; i++)
        {
            Mtl* pSubMtl = pMtl->GetSubMtl( i );
            bRes |= ExtractMaterial( pSubMtl );
        }
        return true;
    }

    // add material to the material list
    if (pMaterial != NULL)
    {   
        int mtlIdx = -1;
        for (int i = 0; i < m_Materials.size(); i++)
        {
            if (pMaterial->IsEqual( m_Materials[i] ))
            {
                mtlIdx = i;
                break;
            }
        }
        if (mtlIdx == -1)
        {
            mtlIdx = m_Materials.size();
            char buf[_MAX_PATH];
            sprintf( buf, "material%02d", mtlIdx );
            pMaterial->SetName( buf );
            m_Materials.push_back( pMaterial );
        }
        else
        {
            delete pMaterial;
        }
        m_pCurExpNode->m_MtlIdx.push_back( mtlIdx );
        return true;
    }

    return false;
} // RBExport::ExtractMaterial

void RBExport::ExtractMaterials( INode* node )
{
    if (!node) return;

    Mtl* pMtl = node->GetMtl();
    if (!pMtl) 
    {
        return;
    }
    
    bool bHasMaterial = ExtractMaterial( pMtl );
    
    if (!bHasMaterial)
    {
        Warn( "No material at the node %s.", node->GetName() );
    }
} // RBExport::ProcessMaterial

void RBExport::CreateMaterialTable()
{
    int nMtl = m_Materials.size();
    for (int i = 0; i < nMtl; i++)
    {
        m_pModel->AddChild( m_Materials[i] );
    }
} // RBExport::CreateMaterialTable

void RBExport::CopyTextures()
{
    int nTex = m_Textures.size();
    int nTexBytes = 0;
    int nTexCopied = 0;
    Path dstPath( m_ModelPath );
    for (int i = 0; i < nTex; i++)
    {
        Path srcPath = m_Textures[i].c_str();
        if (!PathFileExists( srcPath.GetFullPath() ))
        {
            //  try to pick texture from the same directory where .max file is
            srcPath.SetDrive( m_SourcePath.GetDrive() );
            srcPath.SetDirectory( m_SourcePath.GetDirectory() );
        }

        if (!PathFileExists( srcPath.GetFullPath() ))
        {
            Warn( "Texture file %s was not found!", srcPath.GetFileExt() );
            continue;
        }
        dstPath.SetFileExt( srcPath.GetFileExt() );

		//  check whether texture is in .dds format and warn
		if (stricmp( srcPath.GetExt(), "dds" ) && 
            stricmp( srcPath.GetExt(), ".dds" ))
		{
			Warn( "Texture %s is not in DDS format. Consider converting it to DDS, "
					"otherwise its in-game usage will be inefficient.", srcPath.GetFileExt() );
		}

        if (!stricmp( srcPath.GetFullPath(), dstPath.GetFullPath() ))
        {
            continue;
        }
        BOOL bRes = CopyFile( srcPath.GetFullPath(), dstPath.GetFullPath(), 
            m_pConfig->m_bOverwriteTextures ? FALSE : TRUE );
        if (bRes == TRUE)
        {
            Info( "Texture %s copied to %s", srcPath.GetFileExt(), dstPath.GetFullPath() );
            g_pFileServer->AddMediaPath( dstPath.GetDriveDir() );
            nTexBytes += srcPath.GetFileSize();
            nTexCopied++;
        }
        else
        {
            if (!m_pConfig->m_bOverwriteTextures && dstPath.Exists())
            {
                Info( "Texture %s is skipped, because it already exists at destination path.", srcPath.GetFileExt() );
            }
            else
            {
                Warn( "Could not copy texture %s", srcPath.GetFileExt() );
            }
        }
    }

    if (nTexCopied > 0)
    {
        Msg( LogType_Stats, "Copied %d textures, total size is %d Kb.", nTexCopied, nTexBytes/1024 );
    }
} // RBExport::CopyTextures

