/***********************************************************************************/
//  File:   RenderState.h
//  Date:   16.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __RENDERSTATE_H__
#define __RENDERSTATE_H__

/***********************************************************************************/
/*  Enum:  CmpFunc
//  Desc:   
/***********************************************************************************/
enum CmpFunc
{
    cfUnknown       = 0,
    cfNever         = 1,
    cfLess          = 2,
    cfEqual         = 3,
    cfLessEqual     = 4,
    cfGreater       = 5,
    cfNotEqual      = 6,
    cfGreaterEqual  = 7,
    cfAlways        = 8,
}; // enum CmpFunc  

enum_beg(CmpFunc)
enum_val( "Unknown",     cfUnknown      ),
enum_val( "Never",       cfNever        ),
enum_val( "Less",        cfLess         ),
enum_val( "Equal",       cfEqual        ),
enum_val( "LessEqual",   cfLessEqual    ),
enum_val( "Greater",     cfGreater      ),
enum_val( "NotEqual",    cfNotEqual     ),
enum_val( "GreaterEqual",cfGreaterEqual ),
enum_val( "Always",      cfAlways       )
enum_end(CmpFunc)

/***********************************************************************************/
/*  Enum:  FillMode
//  Desc:   
/***********************************************************************************/
enum FillMode
{
    fmUnknown   = 0,
    fmPoint     = 1,
    fmWireframe = 2,
    fmSolid     = 3,
}; // enum FillMode

enum_beg(FillMode)
enum_val( "Unknown",   fmUnknown   ),
enum_val( "Point",     fmPoint     ),
enum_val( "Wireframe", fmWireframe ),
enum_val( "Solid",     fmSolid     )
enum_end(FillMode)

/***********************************************************************************/
/*  Enum:  CullMode
//  Desc:   
/***********************************************************************************/
enum CullMode
{
    cmUnknown   = 0,
    cmNone      = 1,
    cmCW        = 2,
    cmCCW       = 3,
}; // enum CullMode

enum_beg(CullMode)
enum_val( "Unknown",  cmUnknown ),
enum_val( "None",     cmNone    ),
enum_val( "CW",       cmCW      ),
enum_val( "CCW",      cmCCW     )
enum_end(CullMode)

/***********************************************************************************/
/*  Enum:  BlendMode
//  Desc:   
/***********************************************************************************/
enum BlendMode
{
    bmUnknown           = 0,
    bmZero              = 1,
    bmOne               = 2,
    bmSrcColor          = 3,
    bmInvSrcColor       = 4,
    bmSrcAlpha          = 5,
    bmInvSrcAlpha       = 6,
    bmDestAlpha         = 7,
    bmInvDestAlpha      = 8,
    bmDestColor         = 9,
    bmInvDestColor      = 10,
    bmSrcAlphaSat       = 11,
    bmBothSrcAlpha      = 12,
    bmBothInvSrcAlpha   = 13,
    bmBlendFactor       = 14,
    bmInvBlendFactor    = 15,
}; // enum BlendMode

enum_beg(BlendMode)
enum_val( "Unknown",            bmUnknown         ),
enum_val( "Zero",               bmZero            ),
enum_val( "One",                bmOne             ),
enum_val( "SrcColor",           bmSrcColor        ),
enum_val( "InvSrcColor",        bmInvSrcColor     ),
enum_val( "SrcAlpha",           bmSrcAlpha        ),
enum_val( "InvSrcAlpha",        bmInvSrcAlpha     ),
enum_val( "DestAlpha",          bmDestAlpha       ),
enum_val( "InvDestAlpha",       bmInvDestAlpha    ),
enum_val( "DestColor",          bmDestColor       ),
enum_val( "InvDestColor",       bmInvDestColor    ),
enum_val( "SrcAlphaSat",        bmSrcAlphaSat     ),
enum_val( "BothSrcAlpha",       bmBothSrcAlpha    ),
enum_val( "BothInvSrcAlpha",    bmBothInvSrcAlpha ),
enum_val( "BlendFactor",        bmBlendFactor     ),
enum_val( "InvBlendFactor",     bmInvBlendFactor  )
enum_end(BlendMode)
/***********************************************************************************/
/*  Enum:  bool3
//  Desc:   
/***********************************************************************************/
enum bool3
{
    bUnknown   = 0,
    bTrue      = 1,
    bFalse     = 2,
}; // enum bool3

enum_beg(bool3)
enum_val( "Unknown",  bUnknown ),
enum_val( "True",     bTrue    ),
enum_val( "False",    bFalse   )
enum_end(bool3)

/***********************************************************************************/
//  Class:  RenderState
//  Desc:   
/***********************************************************************************/
enum FogVertexMode   
{
    fvmUnknown  = 0,
    fvmNone     = 1,
    fvmExp      = 2,
    fvmExp2     = 3,
    fvmLinear   = 4,
}; // enum FogVertexMode 

enum_beg(FogVertexMode)
enum_val( "Unknown",     fvmUnknown ),
enum_val( "None",        fvmNone    ),
enum_val( "Exp",         fvmExp     ),
enum_val( "Exp2",        fvmExp2    ),
enum_val( "Linear",      fvmLinear  )
enum_end(FogVertexMode)

/***********************************************************************************/
/*  Enum:  ShadeMode
//  Desc:   
/***********************************************************************************/
enum ShadeMode      
{
    ShadeMode_Unknown       = 0,
    ShadeMode_Flat          = 1,
    ShadeMode_Gouraud       = 2,
    ShadeMode_Phong         = 3,
}; // enum ShadeMode

enum_beg(ShadeMode)
enum_val( "Unknown",     ShadeMode_Unknown ),
enum_val( "Flat",        ShadeMode_Flat    ),
enum_val( "Gouraud",     ShadeMode_Gouraud ),
enum_val( "Phong",       ShadeMode_Phong   )
enum_end(ShadeMode)

/***********************************************************************************/
/*  Enum:  MaterialSource
//  Desc:   
/***********************************************************************************/
enum MaterialSource
{
    msUnknown       = 0,
    msMaterial      = 1,
    msColor1        = 2,
    msColor2        = 3,
}; // enum MaterialSource

enum_beg(MaterialSource)
enum_val( "Unknown",     msUnknown  ),
enum_val( "Material",    msMaterial ),
enum_val( "Color1",      msColor1   ),
enum_val( "Color2",      msColor2   )
enum_end(MaterialSource)

/***********************************************************************************/
//  Class:  RenderState
//  Desc:   
/***********************************************************************************/
class RenderState : public JObject
{
    bool3           m_bAlphaBlendEnable;
    BlendMode       m_SrcBlend;        
    BlendMode       m_DestBlend;       
    bool3           m_bAlphaTestEnable; 
    CmpFunc         m_AlphaFunc;       
    DWORD           m_AlphaRef;        
    bool3           m_bDitherEnable;    
    bool3           m_bLighting;        
    bool3           m_bZEnable;         
    bool3           m_bZWriteEnable;   
    bool3           m_bSpecularEnable;   
    CmpFunc         m_ZFunc;           
    CullMode        m_CullMode;        
    DWORD           m_TextureFactor;   
    FillMode        m_FillMode;

    bool3           m_bNormalizeNormals;
    ShadeMode       m_ShadeMode;
    bool3           m_ColorVertex;

    MaterialSource  m_DiffuseMaterialSource;
    MaterialSource  m_SpecularMaterialSource;
    MaterialSource  m_AmbientMaterialSource;
    MaterialSource  m_EmissiveMaterialSource;

    bool3           m_bFogEnable; 
    DWORD           m_FogColor;
    FogVertexMode   m_FogVertexMode;
    float           m_FogStart;
    float           m_FogEnd;
    bool3           m_bRangeFogEnable;

public:
                    RenderState ();
    virtual void    Render      ();
    

    expose(RenderState)
    {
        parent(JObject);
        field( "alphablendenable", m_bAlphaBlendEnable );
        field( "srcblend",         m_SrcBlend          );
        field( "destblend",        m_DestBlend         );
        field( "alphatestenable",  m_bAlphaTestEnable  );
        field( "alphafunc",        m_AlphaFunc         );
        field( "alpharef",         m_AlphaRef          );
        field( "ditherenable",     m_bDitherEnable     );
        field( "lighting",         m_bLighting         );
        field( "zenable",          m_bZEnable          );
        field( "zwriteenable",     m_bZWriteEnable     );
        field( "zfunc",            m_ZFunc             );
        field( "cullmode",         m_CullMode          );
        field( "texturefactor",    m_TextureFactor     );
        field( "fogenable",        m_bFogEnable        );
        field( "specularenable",   m_bSpecularEnable   );

        field( "fogcolor",         m_FogColor          );
        field( "fogvertexmode",    m_FogVertexMode     );
        field( "fogstart",         m_FogStart          );
        field( "fogend",           m_FogEnd            );
        field( "rangefogenable",   m_bRangeFogEnable   );

        field( "fillmode",         m_FillMode          );

        field( "NormalizeNormals",          m_bNormalizeNormals      );
        field( "ShadeMode",                 m_ShadeMode              );
        field( "ColorVertex",               m_ColorVertex            );
        field( "DiffuseMaterialSource",     m_DiffuseMaterialSource  );
        field( "SpecularMaterialSource",    m_SpecularMaterialSource );
        field( "AmbientMaterialSource",     m_AmbientMaterialSource  );
        field( "EmissiveMaterialSource",    m_EmissiveMaterialSource );
    }
}; // class RenderState

#endif //__RENDERSTATE_H__