/***********************************************************************************/
//  File:   JFrameChunk.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JFRAMECHUNK_H__
#define __JFRAMECHUNK_H__

//****************************************************************************/
//  Class:  JFrameChunk
//  Desc:   
//****************************************************************************/
struct JFrameChunk
{
    int                     m_SprX;
    int                     m_SprY;
    int                     m_Side;
    int                     m_Width;
    int                     m_Height;
    WORD                    m_Flags;

    BYTE*                   m_Pixels;
    DWORD                   m_CacheStamp;
    int                     m_FrameIdx;
    int                     m_PackIdx;
    
    int                     m_SurfID;
    float                   m_X0, m_Y0, m_X1, m_Y1;
    float                   m_U0, m_V0, m_U1, m_V1;

    JFrameChunk() : m_Pixels(NULL), m_SurfID(-1){}
    ~JFrameChunk()
    {
        // FIXME
        //delete []m_Pixels;
        m_Pixels = NULL;
    }
}; // struct JFrameChunk

#endif //__JFRAMECHUNK_H__