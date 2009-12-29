/***********************************************************************************/
//  File:   JQuestGame.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JQUESTGAME_H__
#define __JQUESTGAME_H__

#include "ColorSurface.h"
#include "JWeakRef.h"

class JLocation;
class JSwitch;
class JCursor;
class JCharacter;
class JQuestSettings;

/***********************************************************************************/
//  Class:  JQuestGame
//  Desc:   
/***********************************************************************************/
class JQuestGame : public JWidget
{
    JString                 m_CurLocation; 
    JWeakRef<JLocation>     m_pCurLocation;

    JSwitch*                m_pLocations;   // grouping node for all game locations 
    JCursor*                m_pCursor;
    JCharacter*             m_pMainChar;


    JString                 m_CharName;
    JString                 m_Cursor;

    JString                 m_SaveRoot;

    bool                    m_bPaused;
    bool                    m_bFirstStart;
    JString                 m_GameTitle;

public:
                        JQuestGame          ();
    virtual             ~JQuestGame         ();
    virtual void        Render              ();
    virtual void        PostRender          ();
    virtual void        Init                ();
    void                Reset               ();

    bool                SetCursor           ( const char* name );
    const char*         GetCursor           ();
    void                ShowCursor          ( bool bShow );

    JLocation*          GetLocation         ( const char* name );
    void                SetCurLocation      ( const char* name );
    const char*         GetCurLocation      () const { return m_CurLocation.c_str(); }
    void                ReloadCurLocation   ();

    void                SetCharPos          ( const Vec3& pos, float dir );
    Vec3                GetCharPos          () const;

    void                StartNewGame        ();
    void                Quit                ();
    void                Pause               ();
    void                Unpause             ();
    void                TogglePause         ();
    void                LoadGame            ( int gameIdx );
    void                SaveGame            ( int gameIdx );  

    JString             GetSaveGameText     ( int idx ) const;

    void                StopAllSounds       ();
    bool                SaveGameExists      ( int idx ) const;

    const char*         GetCharacter        () const { return m_CharName.c_str(); }
    void                SetCharacter        ( const char* chName );
    void                OnChangeLocation    () {}

    bool                GetGameUserFolder   ( char* path ) const;
    void                HideCurLocation     ();
    void                ShowCurLocation     ();
    void                CloseCurLocation    ();


    expose( JQuestGame )
    {
        parent( JWidget );
        field ( "GameTitle",            m_GameTitle             );
        field ( "CurLocation",          m_CurLocation           );
        prop  ( "Character",            GetCharacter,       SetCharacter        );
        rprop ( "SaveGameExists",       SaveGameExists          );
        field ( "CharName",             m_CharName              );
        rprop ( "SaveGameText",         GetSaveGameText         );
        field ( "SaveRoot",             m_SaveRoot              );

        method( "StartNewGame",         StartNewGame            );
        method( "Quit",                 Quit                    );
        method( "Load",                 LoadGame                );
        method( "Save",                 SaveGame                );
        method( "Pause",                Pause                   );
        method( "TogglePause",          TogglePause             );
        method( "Unpause",              Unpause                 );
        method( "OnInit",               Init                    );
        method( "ReloadCurLocation",    ReloadCurLocation       );
        method( "OnChangeLocation",     OnChangeLocation        );
        method( "HideCurLocation",      HideCurLocation         );
        method( "ShowCurLocation",      ShowCurLocation         );
        method( "CloseCurLocation",     CloseCurLocation        );
    }
protected:
    void                BindPointers        ();
    bool                GetSavegamePath     ( char* path, int idx ) const;

    bool                IsSystemCursor      () const; 
    void                ShowSystemCursor    ( bool bShow = true );

}; // class JQuestGame

extern JQuestGame* g_pQuestGame;

#endif //__JQUESTGAME_H__