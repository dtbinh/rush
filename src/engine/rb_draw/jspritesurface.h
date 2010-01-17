//****************************************************************************/
//  File:  JSpriteSurface.h
//  Desc:
//****************************************************************************/
#ifndef __JSPRITESURFACE_H__
#define __JSPRITESURFACE_H__
#include <vector>
#include "staticarray.h"
#include "color.h"
#include "quadpacker.h"
#include "mat4.h"

const int   c_SpriteSurfaceSide   = 256;
const float c_fSpriteSurfaceSide  = float( c_SpriteSurfaceSide );
const int   c_MaxVertDrawn        = 8192;
const float c_HalfPixel           = 0.5f;

//****************************************************************************/
//  Class:  JSpriteSurface
//  Desc:
//****************************************************************************/
class JSpriteSurface
{
    int                         m_TexID;

    ColorFormat                 m_ColorFormat;
    uint32_t                       m_LastFrameUsed;
    uint32_t                       m_DropStamp;
    static uint32_t                s_Stamp;
    QuadPacker                  m_Layout;

    friend class                JDrawServer;

public:
    JSpriteSurface();
    ~JSpriteSurface();

    void Drop()
    {
        m_DropStamp = ++s_Stamp;
        m_Layout.Free();
    }

    void OnDestroyDevice()
    {
    }

}; // class JSpriteSurface

#endif // __JSPRITESURFACE_H__
