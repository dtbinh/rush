/***********************************************************************************/
//  File:   JQuestSettings.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JQuestSettings_H__
#define __JQuestSettings_H__

/***********************************************************************************/
//  Class:  JQuestSettings
//  Desc:   Describes user-tweakable game settings
/***********************************************************************************/
class JQuestSettings : public JObject
{
public:
                        JQuestSettings      ();

    void                SetMusicVolume      ( float volume );
    float               GetMusicVolume      () const;
    void                SetSoundVolume      ( float volume );
    float               GetSoundVolume      () const;

    void                SetPlayMusic        ( bool bPlay );
    bool                GetPlayMusic        () const;
    void                SetPlaySounds       ( bool bPlay );
    bool                GetPlaySounds       () const;

    void                Load                ();
    void                Save                ();
    virtual void        Init                ();

    const char*         GetText             () const { return m_Text.c_str(); }
    void                SetText             ( const char* text );


    expose( JQuestSettings )
    {
        parent( JObject ); 
        method( "Load",          Load                           );
        method( "Save",          Save                           );
        prop ( "PlayMusic",      GetPlayMusic, SetPlayMusic     );
        prop ( "PlaySounds",     GetPlaySounds, SetPlaySounds   );
        prop ( "MusicVolume",    GetMusicVolume, SetMusicVolume );
        prop ( "SoundVolume",    GetSoundVolume, SetSoundVolume );
        field( "Tier",           m_Tier );
        prop( "Text",            GetText, SetText );
    }

private:
    
    bool            m_bPlayMusic;
    bool            m_bPlaySounds;
    float           m_SoundVol;
    float           m_MusicVol;
    int             m_Tier;
    JString         m_Text;

    friend class JQuestGame;
}; // class JQuestSettings

#endif //__JQuestSettings_H__