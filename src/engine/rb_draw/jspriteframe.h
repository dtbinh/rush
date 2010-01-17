/***********************************************************************************/
//  File:   JSpriteFrame.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSPRITEFRAME_H__
#define __JSPRITEFRAME_H__

#include "jframechunk.h"

enum JFrameFlags
{
    FrameFlags_HasBorder  = 0x01,
    FrameFlags_Difference = 0x02
}; // enum JFrameFlags

//****************************************************************************/
//  Class:  JSpriteFrame
//  Desc:
//****************************************************************************/
struct JSpriteFrame
{
    int                         m_Width;        //  full frame width
    int                         m_Height;       //  full frame height
    Frame                       m_Frame;        //  trimmed bounding frame
    Vec2                        m_Pivot;
    Vec2                        m_Shift;
    bool                        m_bHasBorder;
    int                         m_RefFrame;     //  "reference" frame for difference-frames
    ColorFormat                 m_ColorFormat;  //  sprite pixels color format

    std::vector<JFrameChunk>    m_Chunks;       //  chunks list

    int                         m_NumPixelsTotal;
    uint32_t                       m_LastFrame;
    int                         m_FirstByteInPack;
    uint32_t*                      m_pPixels;
}; // struct JSpriteFrame

#endif //__JSPRITEFRAME_H__
