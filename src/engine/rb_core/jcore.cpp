//****************************************************************************/
//  File:  JCore.cpp
//****************************************************************************/
#include "precompile.h"
#include "JObject.h"
#include "JGroup.h"
#include "JObjectIterator.h"
 
#include "JObjectServer.h"
#include "JSignalServer.h"
#include "JPersistServer.h"
#include "JFileServer.h"

#include "IParticleServer.h"
#include "IScriptServer.h"

#include "direct.h"
#include "Path.h"
#include "JCore.h"

JCore* JCore::s_pInstance = NULL;
//****************************************************************************/
/*  JCore implementation
//****************************************************************************/
decl_class( JCore );

JCore::JCore()
{
    s_pInstance = this;
} // JCore::JCore

JCore::~JCore()
{
} // JCore::~JCore

void JCore::SetRootObject( const char* objName ) 
{ 
    m_RootObject = objName; 
    SendSignal( "RootObject" );
}

void JCore::SaveRoot()
{
    JObject* pObj = g_pObjectServer->FindObject( m_RootObject.c_str() );
    if (!pObj)
    {
        rlog.warn( "Could not save object <%s>: no such object", m_RootObject.c_str() );
        return;
    }
    g_pPersistServer->Save( pObj );
} // JCore::SaveRoot

void JCore::Init()
{
    //  create servers
    if (!g_pObjectServer->FindObject<JGroup>( "srv", this ))
    {
        JGroup* pServers = new JGroup();
        pServers->SetName( "srv" );
        AddChild( pServers );
        
        JSignalServer*  pSignalServer   = new JSignalServer();
        JPersistServer* pPersistServer  = new JPersistServer();
        JFileServer*    pFileServer     = new JFileServer();

        pServers->AddChild( pSignalServer   );    
        pServers->AddChild( pPersistServer  );    
        pServers->AddChild( pFileServer     );
    }
} // JCore::Init

JObject* JCore::AddServer( const char* className )
{
    JGroup* pServers = g_pObjectServer->FindObject<JGroup>( "srv", this );
    if (!pServers) return NULL;
    JObject* pServer = g_pObjectServer->Create( className );
    if (!pServer)
    {
        rlog.err( "Could not create server: %s.", className );
        return NULL;
    }
    pServers->AddChild( pServer );
    return pServer;
} // JCore::AddServer

void JCore::Render()
{
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JObject* pChild = GetChild( i );
        if (!pChild || !pChild->IsVisible()) continue;
        pChild->Render();
    }
} // JCore::Render



