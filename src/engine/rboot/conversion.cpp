/***********************************************************************************/
//  File:   Conversion.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "IPersistServer.h"
#include "IModelServer.h"
#include "JModelAnim.h"
#include "JModel.h"
#include "CommandLine.h"
#include "Path.h"
#include "FStream.h"
      
void ConvertToRMDL( const CommandLine& cmd )
{
    const char* mode = cmd.GetValue( "convert" );

    Path modelPath( cmd.GetValue( "model" ) );
    Path animPath( cmd.GetValue( "anim" ) );

    if (modelPath.Exists())
    {
        modelPath.SetExt( "rbm" );
        JModel* pMdl = obj_cast<JModel>( g_pPersistServer->LoadObject( modelPath.GetFullPath() ) );
        if (pMdl)
        {
            modelPath.SetExt( "rbg" );
            FInStream is( modelPath.GetFullPath() );
            pMdl->Unserialize( is );
            is.Close();
            modelPath.SetExt( "rmdl" );
            FOutStream os( modelPath.GetFullPath() );
            g_pPersistServer->Save( pMdl, os, PersistFmt_JML );
            os.Close();
            printf( "Converted model." );
            pMdl->Release();
        }
    }

    if (animPath.Exists())
    {
        JModelAnim* pAnm = new JModelAnim();
        pAnm->SetName( animPath.GetFile() );
        FInStream is( animPath.GetFullPath() );
        pAnm->Unserialize( is );
        is.Close();
        animPath.SetExt( "ranm" );
        FOutStream os( animPath.GetFullPath() );
        g_pPersistServer->Save( pAnm, os, PersistFmt_JML );
        os.Close();
        pAnm->Release();
    }
}
