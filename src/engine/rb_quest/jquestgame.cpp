/***********************************************************************************/
//  File:   JQuestGame.cpp
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JQuestGame.h"
#include "shlobj.h"
#include "ISoundServer.h"
#include "IScriptServer.h"
#include "FStream.h"
#include "ColorConversion.h"
#include "JSwitch.h"
#include "JLocation.h"
#include "JCursor.h"
#include "IWindowServer.h"
#include "JCharacter.h"
#include "JAnimation.h"
#include "JAnimServer.h"
#include "JQuestSettings.h"
#include "JAnmSound.h"
#include "JObjectIterator.h"
#include "IPersistServer.h"
#include "IModelServer.h"
#include "JCore.h"
#include "JQuestSettings.h"
#include "Logger.h"

const int c_CurSaveVersion = 3;

JQuestGame* g_pQuestGame = NULL;
/***********************************************************************************/
/*  JQuestGame implementation
/***********************************************************************************/
decl_class(JQuestGame);
JQuestGame::JQuestGame()
{
    Reset();
    m_bPaused       = false;
    g_pQuestGame    = this;

    m_CharName      = "Char";
    m_bFirstStart   = true;

    SetCursor( "default" );

    Reset();
} // JQuestGame::JQuestGame

void JQuestGame::Reset()
{
    m_pCurLocation  = NULL;
    m_pLocations    = NULL; 
    m_pCursor       = NULL; 
    m_pMainChar     = NULL; 
} // JQuestGame::Reset

JQuestGame::~JQuestGame()
{
} // JQuestGame::~JQuestGame

void JQuestGame::StartNewGame()
{
    const char* curLoc = GetCurLocation();

    g_pPersistServer->LoadObject( GetName(), this );
    InitTree();
    SetCurLocation( curLoc );
} // JQuestGame::StartNewGame

void JQuestGame::Quit()
{
    g_pWindowServer->Exit();
} // JQuestGame::Quit

void JQuestGame::Init()
{
    BindPointers();
    SendSignal( "OnInit" );

    if (m_pLocations) 
    {
        int nCh = m_pLocations->GetNChildren();
        for (int i = 0; i < nCh; i++)
        {
            JObject* pChild = m_pLocations->GetChild( i );
            pChild->SetVisible( false );
        }
    }
} // JQuestGame::Init

void JQuestGame::BindPointers()
{
    m_pCursor = g_pObjectServer->FindObject<JCursor>( "GameCursor", this );
    if (!m_pCursor)
    {
        m_pCursor = (JCursor*)g_pPersistServer->LoadObject( "GameCursor" );
        if (m_pCursor && !is_a<JCursor>( m_pCursor ))
        {
            m_pCursor->Release();
            m_pCursor = NULL;
        }
    }

    m_pMainChar     = g_pObjectServer->FindObject<JCharacter>( m_CharName.c_str(), this );
    
    //  locations block
    m_pLocations = g_pObjectServer->FindObject<JSwitch>( "Locations", this );
    if (!m_pLocations) 
    {
        m_pLocations = new JSwitch();
        AddChild( m_pLocations );
        m_pLocations->SetName( "Locations" );
    }
    m_pCurLocation = obj_cast<JLocation>( m_pLocations->GetActiveChild() );
} // JQuestGame::BindPointers

void JQuestGame::SetCharacter( const char* chName )
{
    m_pMainChar = g_pObjectServer->FindObject<JCharacter>( chName, this );
    m_CharName = chName;
} // JQuestGame::SetCharacter

void JQuestGame::SetCharPos( const Vec3& pos, float dir )
{
    if (!m_pMainChar) return;
    m_pMainChar->SetPos( pos );
    m_pMainChar->SetPrevPos( pos );
    m_pMainChar->SetDir( dir );
} // JQuestGame::SetCharPos
 
Vec3 JQuestGame::GetCharPos() const
{
    if (!m_pMainChar) return Vec3::null;
    return m_pMainChar->GetPos();
} // JQuestGame::GetCharPos

const char* JQuestGame::GetCursor()
{
    if (!m_pCursor || !m_pCursor->GetActive()) return "";
    int idx = m_pCursor->GetActiveIdx();
    return m_pCursor->GetChild( idx )->GetName();
} // JQuestGame::GetCursor

bool JQuestGame::SetCursor( const char* name )
{
    if (!m_pCursor) return false;
    if (!stricmp( m_pCursor->GetActive(), name )) 
    {
        return true;
    }
    m_pCursor->SetActive( name );
    return true;
} // JQuestGame::SetCursor

void JQuestGame::ShowCursor( bool bShow )
{
    if (!m_pCursor) return;
    m_pCursor->SetVisible( bShow );
} // JQuestGame::ShowCursor

void JQuestGame::HideCurLocation()
{
    if (m_pCurLocation)
    {
        m_pCurLocation->SetVisible( false );
    }
}

void JQuestGame::ShowCurLocation()
{
    if (m_pCurLocation)
    {
        m_pCurLocation->SetVisible( true );
    }
}

void JQuestGame::CloseCurLocation()
{
    if (m_pCurLocation)
    {
        m_pCurLocation->Close();
    }
}

void JQuestGame::SetCurLocation( const char* name )
{
    if (!m_pLocations) return;

    m_pLocations->SetActive( name ); 
    if (m_pCurLocation)
    {
        if (m_pCurLocation->HasName( name ))
        {
            return;
        }
        m_pCurLocation->Close();
    }

    m_pCurLocation = obj_cast<JLocation>( m_pLocations->GetActiveChild() );
    if (m_pCurLocation) 
    {
        m_pCurLocation->Show();
        m_CurLocation = m_pCurLocation->GetName();
    }

    if (m_Cursor.size()) SetCursor( m_Cursor.c_str() );

    SendSignal( "CurLocation" );
    SendSignal( "OnChangeLocation" );
} // JQuestGame::SetCurLocation

void JQuestGame::ReloadCurLocation()
{
    if (!m_pCurLocation) return;
    
    //g_pObjectServer->DumpTree( m_pCurLocation );

    //  reload scripts
    if (g_pScriptServer)
    {
        g_pScriptServer->ReloadScripts();
    }

    const char* name = m_pCurLocation->GetName();
    g_pPersistServer->LoadObject( name, m_pCurLocation );
    m_pCurLocation->InitTree();
    m_pLocations->SetActive( name );
    m_pCurLocation = obj_cast<JLocation>( m_pLocations->GetActiveChild() );
    if (m_pCurLocation) m_pCurLocation->Show();

    g_pRenderServer->ReloadResources();
} // JQuestGame::ReloadCurLocation

JLocation* JQuestGame::GetLocation( const char* name )
{
    JLocation* pLoc = g_pObjectServer->FindObject<JLocation>( name, m_pLocations );
    return pLoc;
} // JQuestGame::GetLocation

void JQuestGame::Render()
{
    JAnmContext& ctx = JAnimation::PushContext();
    float timeDelta = ctx.m_TimeDelta;
    if (m_bPaused) 
    {
        timeDelta = 0.0f;
    }
    ctx.Reset();
    ctx.m_TimeDelta = timeDelta;

} // JQuestGame::Render

void JQuestGame::PostRender()
{
    JAnimation::PopContext();
} // JQuestGame::Render

void JQuestGame::Pause()
{
    m_bPaused = true;
    g_pSoundServer->Pause();
} // JQuestGame::Pause

void JQuestGame::Unpause()
{
    m_bPaused = false;
    g_pSoundServer->Pause( false );
} // JQuestGame::Unpause

void JQuestGame::TogglePause()
{
    if (m_bPaused) 
    {
        Unpause(); 
    }
    else 
    {
        Pause();
    }
} // JQuestGame::TogglePause

bool JQuestGame::SaveGameExists( int idx ) const
{
    char path[_MAX_PATH];
    GetSavegamePath( path, idx );

    FInStream is( path );
    if (!is) return false;

    return true;
} // JQuestGame::SaveGameExists

void JQuestGame::StopAllSounds()
{
    JObjectIterator it( this );
    while (it)
    {
        JAnmSound* pSound = obj_cast<JAnmSound>( *it );
        ++it;
        if (!pSound || !pSound->IsPlaying()) continue;
        pSound->Stop();
        if (pSound->IsAutoPlayed())
        {
            pSound->SetVisible( true );
        }
    }
} // JQuestGame::StopAllSounds

static const char* c_SaveGameID = "QUSG";
static DWORD c_SaveGameFOURCC = *((DWORD*)c_SaveGameID);
static int c_SaveGameVersion = 2;

JString JQuestGame::GetSaveGameText( int idx ) const
{
    char path[_MAX_PATH];
    GetSavegamePath( path, idx );
    FInStream is( path );

    JString text;
    DWORD fourcc = 0;
    int   version = 0;
    is >> fourcc >> version;
    if (fourcc != c_SaveGameFOURCC || version != c_SaveGameVersion)
    {
        return "";
    }
    is >> text;
    return text;
}

void JQuestGame::LoadGame( int gameIdx )
{
    char path[_MAX_PATH];
    GetSavegamePath( path, gameIdx );
    FInStream is( path );

    DWORD fourcc = 0;
    int   version = 0;
    is >> fourcc >> version;
    if (fourcc != c_SaveGameFOURCC)
    {
        rlog.err( "Invalid save game fourcc ID" );
        return;
    }

    if (version != c_SaveGameVersion)
    {
        rlog.err( "Invalid save game version: expecting %d, got %d", c_SaveGameVersion, version );
        return;
    }

    JString text;
    is >> text;

    m_pCurLocation = NULL;
    StopAllSounds();

    JObject* pSaveRoot = g_pObjectServer->FindObject( m_SaveRoot.c_str() );
    if (pSaveRoot == NULL) 
    {
        pSaveRoot = this;
    }

    g_pPersistServer->Load( is, PersistFmt_Bin, pSaveRoot );

    BindPointers();

    //  perform initialization
    InitTree();

    if (m_pCurLocation)
    {
        m_pCurLocation->SetVisible( true );
    }
} 

void JQuestGame::SaveGame( int gameIdx )
{
    char path[_MAX_PATH];
    GetSavegamePath( path, gameIdx );

    FOutStream os( path );
    JQuestSettings* pSettings = 
        obj_cast<JQuestSettings>( g_pObjectServer->FindObject( "gamesettings", NULL, this ) );
    JString text = pSettings ? pSettings->GetText() : "";

    os << c_SaveGameFOURCC << c_SaveGameVersion;
    os << text;

    JObject* pSaveRoot = g_pObjectServer->FindObject( m_SaveRoot.c_str() );
    if (pSaveRoot == NULL) 
    {
        pSaveRoot = this;
    }
    g_pPersistServer->Save( pSaveRoot, os, PersistFmt_Bin );
}  

bool JQuestGame::GetGameUserFolder( char* path ) const
{
    char cwd[_MAX_PATH];
    getcwd( cwd, _MAX_PATH );

    BOOL bRes = SHGetSpecialFolderPath( (HWND)g_pWindowServer->GetRootHandle(), path, CSIDL_PERSONAL, TRUE );
    if (!bRes)
    {
        sprintf( path, "\\saves" );
    }
    else
    {
        strcat( path, "\\My Games\\" );
        //  create directory if not existent
        if (chdir( path ) != 0)
        {
            mkdir( path );
        }

        strcat( path, m_GameTitle.c_str() );
    }
    //  create directory if not existent
    if (chdir( path ) != 0)
    {
        mkdir( path );
    }
    chdir( cwd );
    return true;
} // JQuestGame::GetGameUserFolder

bool JQuestGame::GetSavegamePath( char* path, int idx ) const
{
    if (!GetGameUserFolder( path ))
    {
        return false;
    }

    //  append savegame filename
    char fname[_MAX_PATH];
    sprintf( fname, "\\save%02d.sav", idx );
    strcat( path, fname );

    return true;
} // JQuestGame::GetSavegamePath

