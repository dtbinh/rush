#ifndef __AUTORUN_H__
#define __AUTORUN_H__

#include "JObject.h"

class Autorun : public JObject
{
public:
                    Autorun             ();

    virtual void    Init                ();
    virtual void    Render              ();

    bool            IsGameInstalled     () const { return m_bInstalled; }
    bool            IsGameNotInstalled  () const { return !m_bInstalled; }

    void            Install             ();
    void            Uninstall           ();
    void            Readme              ();
    void            Exit                ();
    void            Play                ();
    void            DirectX             ();
    void            Codecs              ();

    expose(Autorun)
    {
        parent(JObject); 
        field( "AppKey",        m_AppKey );
        field( "UninstFile",    m_UninstFile );
        field( "SetupFile",     m_SetupFile );
        field( "ReadmeFile",    m_ReadmeFile );

        rprop( "GameInstalled", IsGameInstalled );
        rprop( "GameNotInstalled", IsGameNotInstalled );
        method( "Install",      Install );
        method( "Uninstall",    Uninstall );
        method( "Readme",       Readme );
        method( "Exit",         Exit );
        method( "Play",         Play );
        method( "DirectX",      DirectX );
        method( "Codecs",       Codecs );
    }

private:
    bool                    m_bInstalled;
    JString                 m_GamePath;
    JString                 m_AutorunPath;
    JString                 m_GameExecutable;


    JString                 m_AppKey;
    JString                 m_UninstFile;
    JString                 m_ReadmeFile;
    JString                 m_SetupFile;

    void                    UpdateStatus();
    bool                    OpenFile    ( const char* path );
}; // class Autorun

#endif // __AUTORUN_H__