/***********************************************************************************/
//  File:   ModelViewer.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "IParticleServer.h"
#include "IPersistServer.h"
#include "JWindowServer.h"
#include "IModelServer.h"
#include "IFileServer.h"
#include "FStream.h" 
#include "Path.h"
#include "AABox.h"
#include "CommandLine.h"
#include "JModel.h"
#include "JModelAnim.h"
#include "JModelInstance.h"
#include "JDropFileEvent.h"
#include "ModelViewer.h"  

/***********************************************************************************/
/*  ModelViewer implementation
/***********************************************************************************/
decl_class(ModelViewer);
ModelViewer::ModelViewer()
{
    m_pAnimInstance = NULL;
    m_pModelInstance = NULL;
}
    
void ModelViewer::Render()
{
    SendSignal( "Render" );
} // ModelViewer::Render

void ModelViewer::PostRender()
{
    SendSignal( "PostRender" );
    g_pModelServer->DrawShadows();
} // ModelViewer::PostRender

void ModelViewer::Init()
{
    m_pAnimInstance  = g_pObjectServer->FindObject<JAnimInstance> ( "PlayedAnim",  this );
    m_pModelInstance = g_pObjectServer->FindObject<JModelInstance>( "PlayedModel", this );
} // ModelViewer::Init

void ModelViewer::Exit()
{
    exit( 0 );
} // ModelViewer::Exit

void ModelViewer::SortTransparent()
{
    int mdlID = GetModelID();
    void SortTransparentPolys( JModel* pModel );
    JModel* pModel = g_pModelServer->GetModel( mdlID );
    SortTransparentPolys( pModel );
} // ModelViewer::SortTransparent

void ModelViewer::SaveModel()
{
    int mdlID = GetModelID();
    JModel* pModel = g_pModelServer->GetModel( mdlID );
    Path modelPath( m_ModelName.c_str() );
    if (pModel && modelPath.Exists())
    {
        FOutStream os( modelPath.GetFullPath() );
        g_pPersistServer->Save( pModel, os, PersistFmt_JML );
    }
}  

void ModelViewer::SetModel( const char* name )
{
    if (name == NULL || name[0] == 0)
    {
        return;
    }
    m_ModelName = name;

    //  try to find media path
    Path modelPath( name );
    g_pFileServer->AddMediaPath( modelPath.GetDriveDir() );

    if (!m_pAnimInstance) return;
    m_pAnimInstance->SetModel( name );
    m_pModelInstance->SetModel( name );
}

void ModelViewer::SetAnim( const char* name )
{
    if (name == NULL || name[0] == 0)
    {
        return;
    }
    m_AnimName = name;
    if (!m_pAnimInstance) 
    {
        return;
    }
    m_pAnimInstance->SetAnim( name );

    Path animPath( name );
    Path modelPath = animPath;
    modelPath.SetExt( "rmdl" );
            
    //  try to fetch model name from the animation
    JModelAnim* pAnim = g_pModelServer->GetAnim( GetAnimID() );
    if (pAnim && strlen( pAnim->GetModelName() ) > 0)
    {
        modelPath.SetFile( pAnim->GetModelName() );
    }
    if (!modelPath.Exists())
    {
        modelPath.SetFile( animPath.GetFile() );
    }
        
    SetModel( modelPath.GetFullPath() );
}

int ModelViewer::GetAnimID() const
{
    if (!m_pAnimInstance) return -1;
    return m_pAnimInstance->GetAnimID();
}

int ModelViewer::GetModelID() const
{
    if (!m_pAnimInstance) return -1;
    return m_pModelInstance->GetModelID();
}

void ModelViewer::OnEvent( JEvent* evt )
{
    if (evt->GetID() == JDropFileEvent::ID)
    {
        JDropFileEvent* pEvt = (JDropFileEvent*)evt;
        Path path( pEvt->GetPath() );
        if (!stricmp( path.GetExt(), "rmdl" ))
        {
            SetModel( path.GetFullPath() );
        }
        if (!stricmp( path.GetExt(), "ranm" ))
        {
            SetAnim( path.GetFullPath() );
        }
    }
}



