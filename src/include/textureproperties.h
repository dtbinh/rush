/*****************************************************************************/
/*  File:  TextureProperties.h
/*****************************************************************************/
#ifndef __TEXTUREPROPERTIES_H__
#define __TEXTUREPROPERTIES_H__
#include "color.h"

/*****************************************************************************/
/*  Enum:   DepthFormat
/*  Desc:   Decribes depth stencil buffer pixel configuration
/*****************************************************************************/
enum DepthFormat
{
    DepthFormat_Unknown     = 0,
    DepthFormat_D16Lockable = 1,
    DepthFormat_D32         = 2,
    DepthFormat_D15S1       = 3,
    DepthFormat_D24S8       = 4,
    DepthFormat_D16         = 5
}; // enum DepthFormat

/*****************************************************************************/
/*  Enum:   PoolType
/*  Desc:   Decribes memory pool type
/*****************************************************************************/
enum PoolType
{
    PoolType_Unknown    = 0,
    PoolType_System     = 1,
    PoolType_Video      = 2,
    PoolType_Managed    = 3,
}; // enum PoolType

/*****************************************************************************/
/*  Class:  TextureProperties
/*  Desc:   Bundle of texture resource properties
/*****************************************************************************/
struct TextureProperties
{
    int             m_Width;
    int             m_Height;
    int             m_NMips;
    bool            m_bAutoGenMips;
    bool            m_bRenderTarget;
    PoolType        m_PoolType;
    ColorFormat     m_Format;
    DepthFormat     m_DSFormat;
    char            m_Name[_MAX_PATH];

    TextureProperties() : m_Width(0), m_Height(0), m_NMips(0),
                          m_bAutoGenMips( false ), m_PoolType( PoolType_Unknown ),
                          m_Format( ColorFormat_Unknown ),
                          m_DSFormat( DepthFormat_Unknown ),
                          m_bRenderTarget( false )
    {
        m_Name[0] = 0;
    }

    void SetName( const char* name )
    {
        strcpy( m_Name, name );
    }
}; // struct TextureProperties

#endif // __TEXTUREPROPERTIES_H__


