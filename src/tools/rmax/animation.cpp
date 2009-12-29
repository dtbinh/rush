/***********************************************************************************/
//  File:   animation.cpp
//  Desc:   Animation-related exporter part
//  Date:   26.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JModel.h"
#include "JModelAnim.h"
#include "JAnmSection.h"
#include "ExportConfig.h"
#include "rmax.h"
#include "decomp.h"

void RBExport::ProcessAnimation( INode* node )
{
    if (!m_pAnimation)
    {
        return;
    }

    TimeValue samplingInterval = SecToTicks( m_SamplingInterval/1000.0f );

    //  create exported animation for this node
    TrackPRS& track = m_pAnimation->AddTrack();
    track.m_Name = node->GetName();
    track.m_Tag  = -1;

    //  sample whole animation interval
    for (TimeValue t = m_AnimRange.Start(); t <= m_AnimRange.End(); t += samplingInterval)
    {
        float cTime = TimeToMs( t - m_AnimRange.Start() )*0.001f;
        
        //  decompose transform into affine parts
        Mat4 m = Convert( GetLocalTM( node, t ) );
        Vec3 pos, scale;
        Quaternion rot;
        m.factorize( scale, rot, pos );
        rot.normalize();
        
        //  add position key
        track.m_Pos.AddKey( cTime, pos );
        track.m_Rot.AddKey( cTime, rot );
        track.m_Scale.AddKey( cTime, scale );   

        //  add visibility key
        bool bVisible = (node->GetVisibility( t ) > 0.5f)? true : false;
        track.m_Visibility.AddKey( cTime, bVisible ? 0 : 1 );

    }

    //  flatten constant subtracks
    const float c_PosEpsilon   = 0.00001f;
    const float c_QuatEpsilon  = 0.000001f;
    const float c_ScaleEpsilon = 0.001f;
    if (track.m_Pos.IsConstant( c_PosEpsilon )) track.m_Pos.Flatten();
    if (track.m_Rot.IsConstant( c_QuatEpsilon )) track.m_Rot.Flatten();
    if (track.m_Scale.IsConstant( c_ScaleEpsilon )) track.m_Scale.Flatten();
    if (track.m_Visibility.IsConstant( c_ScaleEpsilon )) track.m_Visibility.Flatten();
    
    //  fix adjucent quaternion rotation keys, to make sure that they come through shortest arc
    track.m_Rot.FixupAdjacentKeys();

    //  reduce keys
    {
        track.m_Pos.Reduce( c_PosEpsilon );
        track.m_Rot.Reduce( c_QuatEpsilon );
        track.m_Scale.Reduce( c_ScaleEpsilon );
        track.m_Visibility.Reduce( c_ScaleEpsilon );
    }
   
} // RBExport::ProcessAnimation

bool RBExport::CheckAnimation()
{
    if (!m_pModel || !m_pAnimation) return false;
    m_pModel->GatherBones();

    JBoneInstance* pBones = m_pModel->GetBoneInstances();
    int nBones  = m_pModel->GetNBones();
    int nTracks = m_pAnimation->GetNTracks();
    bool bOK = true;
    for (int i = 0; i < nTracks; i++)
    {
        TrackPRS& track = m_pAnimation->GetTrack( i );
        track.m_Tag = -1;
        for (int j = 0; j < nBones; j++)
        {
            JBone* pBone = m_pModel->GetBoneInstance( j )->m_pBone;
            if (!stricmp( track.m_Name.c_str(), pBone->GetName() ))
            {
                track.m_Tag = j;
                break;
            }
        }
        if (track.m_Tag == -1)
        {
            Err( "Animation can not find the bone in the given model: %s", track.m_Name.c_str() );
            bOK = false;
        }
    }

    if (!bOK)
    {
        Err( "Animation does not fully match given model: %s", m_ModelPath.GetFullPath() );
    }

    return bOK;
} // RBExport::CheckAnimation

int RBExport::ExtractAnimSet()
{
    if (m_pConfig->m_bExportAnimSet == false || 
        m_pConfig->m_bExportAnimSet == false ||
        !m_pAnimation)
    {
        return 0;
    }
    int nSections = 0;

    PROPSPEC spec;
    spec.propid = PIDSI_COMMENTS;
    spec.ulKind = PRSPEC_PROPID;
    int idx = m_pInterface->FindProperty( PROPSET_SUMMARYINFO, &spec );
    if (idx == -1)
    {
        return 0;
    }

    const PROPVARIANT* pProp = m_pInterface->GetPropertyVariant( PROPSET_SUMMARYINFO, idx );
    if (!pProp)
    {
        return 0;
    }

    const char* propText = pProp->pszVal;
    const char* pch = propText;
    while( true )
    {
        const char* c_Digits        = "0123456789";
        const char* c_LineEnd       = "\n\r";
        const char* c_Dividers      = " \t;,.-|";
        const char* c_AllDividers   = " \t;,.-|\n\r";
        
        //  parse range start
        pch += strspn( pch, c_Dividers );
        int val = 0;
        if (sscanf( pch, "%d", &val ) != 1)
        {
            break;
        }
        float start = val;
        pch += strcspn( pch, c_AllDividers );

        //  parse range end
        pch += strspn( pch, c_Dividers );
        if (sscanf( pch, "%d", &val ) != 1)
        {
            break;
        }   
        float end = val;
        pch += strcspn( pch, c_AllDividers );

        JString id, descr;
        if (strchr( c_LineEnd, *pch ))
        {   
            //  premature line end (no ID, no description)
            char buf[256];
            sprintf( buf, "anim%d", nSections );
            pch += strspn( pch, c_LineEnd );
        }
        else
        {
            //  parse range ID
            pch += strspn( pch, c_Dividers );
            int nSym = strcspn( pch, c_AllDividers );
            if (nSym != 0)
            {
                id = JString( pch, nSym );
            }
            pch += nSym;

            if (strchr( c_LineEnd, *pch ))
            {   
                //  premature line end (no description)
                pch += strspn( pch, c_LineEnd );
            }
            else
            {
                //  parse description
                JString descr;
                pch += strspn( pch, c_Dividers );
                nSym = strcspn( pch, c_LineEnd );
                if (nSym != 0)
                {
                    descr = JString( pch, nSym );
                }
                pch += nSym;
                pch += strspn( pch, c_LineEnd );
            }
        }
        
        if (start < m_AnimRange.Start() || nSections == 0)
        {
            m_AnimRange.SetStart( (TimeValue)start );
        }

        if (end > m_AnimRange.End() || nSections == 0)
        {
            m_AnimRange.SetEnd( (TimeValue)end );
        }
        
        start = TimeToMs( start*GetTicksPerFrame() )*0.001f;
        end   = TimeToMs( end*GetTicksPerFrame() )*0.001f;
        m_pAnimation->AddChild( new JAnmSection( id.c_str(), start, end, descr.c_str() ) );
        nSections++;
    }

    //  normalize animation sections to be relative to the start of animation range
    float anmStart = TimeToMs( m_AnimRange.Start()*GetTicksPerFrame() )*0.001f;
    int nCh = m_pAnimation->GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JAnmSection* pSection = obj_cast<JAnmSection>( m_pAnimation->GetChild( i ) );
        if (pSection)
        {
            pSection->SetStart( pSection->GetStart() - anmStart );
            pSection->SetEnd( pSection->GetEnd() - anmStart );
        }
    }

    
    Msg( LogType_Info, "Found %d animation sections in set (total range %d-%d).", nSections, 
        m_AnimRange.Start(), m_AnimRange.End() );
    
    m_AnimRange.SetStart( m_AnimRange.Start()*GetTicksPerFrame() );
    m_AnimRange.SetEnd  ( m_AnimRange.End()*GetTicksPerFrame() );
    
    return nSections;
}




