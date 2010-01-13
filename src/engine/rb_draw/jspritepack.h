/***********************************************************************************/
//  File:   JSpritePack.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSPRITEPACK_H__
#define __JSPRITEPACK_H__

#include "MappedFile.h"
#include "JSpriteFrame.h"

/***********************************************************************************/
//  Class:  JSpritePack
//  Desc:   
/***********************************************************************************/
class JSpritePack : public JObject
{
    ColorFormat                 m_ColorFormat;
    PSpriteUpdateCB             m_UpdateCB;
    std::vector<JSpriteFrame>   m_Frames;
    MappedFile                  m_Mapping;
    bool                        m_bAlwaysCache;

    int                         m_HeaderSize;
    uint32_t                       m_LastFrame;

    friend class                JDrawServer;
public:
            JSpritePack ();

    int     GetNumFrames() const { return m_Frames.size(); }

    expose( JSpritePack )
    {
        parent( JObject );
        //field( "ColorFormat",  m_ColorFormat  );
        rprop( "NumFrames",    GetNumFrames   );
        field( "AlwaysCache",  m_bAlwaysCache );
    }
}; // class JSpritePack

#endif //__JSPRITEPACK_H__