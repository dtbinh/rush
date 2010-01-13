/***********************************************************************************/
//  File:   JSpriteProbe.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSPRITEPROBE_H__
#define __JSPRITEPROBE_H__

#include "MappedFile.h"
#include "JSpriteFrame.h"

/***********************************************************************************/
//  Class:  JSpriteProbe
//  Desc:   
/***********************************************************************************/
class JSpriteProbe : public JObject
{
public:
            JSpriteProbe ();

    Vec2    GetPosition() const { return m_Position; }
    void    SetPosition( Vec2 pos ) { m_Position = pos; }

    float   GetAlpha() const;
    uint32_t   GetColor() const;

    expose( JSpriteProbe )
    {
        parent( JObject );
        field( "SkinPack",  m_SkinPack  );
        field( "SkinFrame", m_SkinFrame );
        prop( "Position",   GetPosition, SetPosition );
        rprop( "Alpha",     GetAlpha );
        rprop( "Color",     GetColor );
    }

private:
    JString         m_SkinPack;
    int             m_SkinFrame;
    Vec2            m_Position;

}; // class JSpriteProbe

#endif //__JSPRITEPROBE_H__