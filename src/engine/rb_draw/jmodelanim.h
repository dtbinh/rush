/***********************************************************************************/
//  File:   JModelAnim.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JMODELANIM_H__
#define __JMODELANIM_H__

#include "TrackPRS.h"

typedef std::vector<TrackPRS>   TrackArray;
class JBoneInstance;
/***********************************************************************************/
//  Class:  JModelAnim
//  Desc:   
/***********************************************************************************/
class JModelAnim : public JObject
{
public:
                        JModelAnim      ();
    virtual void        Init            ();
    TrackPRS&           AddTrack        ();
    void                Clear           ();
    TrackPRS&           GetTrack        ( int trackID )  { return m_Tracks[trackID]; }
    float               GetDuration     () const { return m_Duration; }
    void                SetDuration     ( float val ) { m_Duration = val; }

    int                 GetNTracks      () const { return m_Tracks.size(); }
    bool                Apply           ( JBoneInstance* pBones, float cTime, float weight = 1.0f );
    bool                Bind            ( JModel* pModel );

    int                 GetModelID      () const { return m_ModelID; }
    void                SetModelID      ( int id ) { m_ModelID = id; }

    void                SetModelName    ( const char* name ) { m_ModelName = name; }
    const char*         GetModelName    () const { return m_ModelName.c_str(); }

    void                Serialize       ( OutStream& os ) const;
    bool                Unserialize     ( InStream& is );

    expose( JModelAnim )
    {
        parent( JObject );
        rprop ( "NumTracks", GetNTracks  );
        rprop ( "Duration",  GetDuration );
        field ( "ModelName", m_ModelName );
    }

private:
    TrackArray      m_Tracks;
    float           m_Duration;
    int             m_ModelID;

    std::string     m_ModelName;
}; // class JModelAnim

#endif //__JMODELANIM_H__