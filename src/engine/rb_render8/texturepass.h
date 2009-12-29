/***********************************************************************************/
//  File:   TexturePass.h
//  Date:   16.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TEXTUREPASS_H__
#define __TEXTUREPASS_H__

/***********************************************************************************/
/*  Enum:  TexOp
//  Desc:   
/***********************************************************************************/
enum TexOp
{
    toUnknown                   = 0,
    toDisable                   = 1,  // disables stage
    toSelectArg1                = 2,  
    toSelectArg2                = 3,
    toModulate                  = 4,  // multiply args together
    toModulate2x                = 5,  // multiply and  1 bit
    toModulate4x                = 6,  // multiply and  2 bits
    toAdd                       = 7,  // add arguments together
    toAddSigned                 = 8,  // add with -0.5 bias
    toAddSigned2x               = 9,  // as above but left  1 bit
    toSubtract                  = 10, // Arg1 - Arg2, with no saturation
    toAddSmooth                 = 11, // add 2 args, subtract product
    toBlendDiffuseAlpha         = 12, // iterated alpha
    toBlendTextureAlpha         = 13, // texture alpha
    toBlendFactorAlpha          = 14, // alpha from D3DRS_TEXTUREFACTOR
    toBlendTextureAlphaPM       = 15, // texture alpha
    toBlendCurrentAlpha         = 16, // by alpha of current color
    toPremodulate               = 17, // modulate with next texture before use
    toModulateAlpha_AddColor    = 18, // Arg1.RGB + Arg1.A*Arg2.RGB
    toModulateColor_AddAlpha    = 19, // Arg1.RGB*Arg2.RGB + Arg1.A
    toModulateInvAlpha_AddColor = 20, // (1-Arg1.A)*Arg2.RGB + Arg1.RGB
    toModulateInvColor_AddAlpha = 21, // (1-Arg1.RGB)*Arg2.RGB + Arg1.A
    toBumpEnvMap                = 22, // per pixel env map perturbation
    toBumpEnvMapLuminance       = 23, // with luminance channel
    toDotProduct3               = 24, 
    toMultyplyAdd               = 25, // Arg0 + Arg1*Arg2
    toLerp                      = 26, // (Arg0)*Arg1 + (1-Arg0)*Arg2
}; // enum TexOp

enum_beg(TexOp)
enum_val( "Unknown",                    toUnknown                    ),
enum_val( "Disable",                    toDisable                    ),
enum_val( "SelectArg1",                 toSelectArg1                 ),
enum_val( "SelectArg2",                 toSelectArg2                 ),
enum_val( "Modulate",                   toModulate                   ),
enum_val( "Modulate2x",                 toModulate2x                 ),
enum_val( "Modulate4x",                 toModulate4x                 ),
enum_val( "Add",                        toAdd                        ),
enum_val( "AddSigned",                  toAddSigned                  ),
enum_val( "AddSigned2x",                toAddSigned2x                ),
enum_val( "Subtract",                   toSubtract                   ),
enum_val( "AddSmooth",                  toAddSmooth                  ),
enum_val( "BlendDiffuseAlpha",          toBlendDiffuseAlpha          ),
enum_val( "BlendTextureAlpha",          toBlendTextureAlpha          ),
enum_val( "BlendFactorAlpha",           toBlendFactorAlpha           ),
enum_val( "BlendTextureAlphaPM",        toBlendTextureAlphaPM        ),
enum_val( "BlendCurrentAlpha",          toBlendCurrentAlpha          ),
enum_val( "Premodulate",                toPremodulate                ),
enum_val( "ModulateAlpha_AddColor",     toModulateAlpha_AddColor     ),
enum_val( "ModulateColor_AddAlpha",     toModulateColor_AddAlpha     ),
enum_val( "ModulateInvAlpha_AddColor",  toModulateInvAlpha_AddColor  ),
enum_val( "ModulateInvColor_AddAlpha",  toModulateInvColor_AddAlpha  ),
enum_val( "BumpEnvMap",                 toBumpEnvMap                 ),
enum_val( "BumpEnvMapLuminance",        toBumpEnvMapLuminance        ),
enum_val( "DotProduct3",                toDotProduct3                ),
enum_val( "MultyplyAdd",                toMultyplyAdd                ),
enum_val( "Lerp",                       toLerp                       )
enum_end(TexOp)

/***********************************************************************************/
/*  Enum:  TexArg
//  Desc:   
/***********************************************************************************/
enum TexArg
{
    taUnknown   = 0,
    taDiffuse   = 1,
    taCurrent   = 2,
    taTexture   = 3,
    taTFactor   = 4,
    taSpecular  = 5,
}; // enum TexArg

enum_beg(TexArg)
enum_val( "Unknown",  taUnknown     ),
enum_val( "Diffuse",  taDiffuse     ),
enum_val( "Current",  taCurrent     ),
enum_val( "Texture",  taTexture     ),
enum_val( "TFactor",  taTFactor     ),
enum_val( "Specular", taSpecular    )
enum_end(TexArg)

/***********************************************************************************/
/*  Enum:  TexAddress
//  Desc:   
/***********************************************************************************/
enum TexAddress
{
    tdUnknown         = 0,
    tdWrap            = 1,
    tdMirror          = 2,
    tdClamp           = 3,
    tdBorder          = 4,
    tdMirrorOnce      = 5,
}; // enum TexArg

enum_beg(TexAddress)
enum_val( "Unknown",      tdUnknown    ),
enum_val( "Wrap",         tdWrap       ),
enum_val( "Mirror",       tdMirror     ),
enum_val( "Clamp",        tdClamp      ),
enum_val( "Border",       tdBorder     ),
enum_val( "MirrorOnce",   tdMirrorOnce )
enum_end(TexAddress)

/***********************************************************************************/
/*  Enum:  TexFilter
//  Desc:   
/***********************************************************************************/
enum TexFilter
{
    tfUnknown         = 0,
    tfNone            = 1,    // filtering disabled (valid for mip filter only)
    tfPoint           = 2,    // nearest
    tfLinear          = 3,    // linear interpolation
    tfAnisotropic     = 4,    // anisotropic
    tfFlatCubic       = 5,    // cubic
    tfGaussianCubic   = 6,    // different cubic kernel
}; // enum TexFilter

enum_beg(TexFilter)
enum_val( "Unknown",        tfUnknown ),
enum_val( "None",           tfNone          ),
enum_val( "Point",          tfPoint         ),
enum_val( "Linear",         tfLinear        ),
enum_val( "Anisotropic",    tfAnisotropic   ),
enum_val( "FlatCubic",      tfFlatCubic     ),
enum_val( "GaussianCubic",  tfGaussianCubic )
enum_end(TexFilter)


/***********************************************************************************/
/*  Enum:  TexCoordIndex
//  Desc:   
/***********************************************************************************/
enum TexCoordIndex
{
    tci1                            = 0x00000001,
    tci2                            = 0x00000002,
    tci3                            = 0x00000003,
    tciUnknown                      = 0xFFFFFFFF,
    tciPassThru                     = 0x00000000,    
    tciCameraSpaceNormal            = 0x00010000,    
    tciCameraSpacePosition          = 0x00020000,  
    tciCameraSpaceReflectionVector  = 0x00030000,  
}; // enum TexCoordIndex

enum_beg(TexCoordIndex)
enum_val( "1",                              tci1                            ),
enum_val( "2",                              tci2                            ),
enum_val( "3",                              tci3                            ),
enum_val( "Unknown",                        tciUnknown                      ),
enum_val( "PassThru",                       tciPassThru                     ),
enum_val( "CameraSpaceNormal",              tciCameraSpaceNormal            ),
enum_val( "CameraSpacePosition",            tciCameraSpacePosition          ),
enum_val( "CameraSpaceReflectionVector",    tciCameraSpaceReflectionVector  )
enum_end(TexCoordIndex)

/***********************************************************************************/
/*  Enum:  TexTMFlags
//  Desc:   
/***********************************************************************************/
enum TexTMFlags
{
    ttmUnknown          = 0xFFFFFFFF,
    ttmDisable          = 0,
    ttmCount1           = 1,
    ttmCount2           = 2,
    ttmCount3           = 3,
    ttmCount4           = 4,
    ttmCount3Projected  = 3|256,
    ttmCount4Projected  = 4|256,
    ttmProjected        = 256,
}; // enum TexTMFlags

enum_beg(TexTMFlags)
enum_val( "Unknown",            ttmUnknown          ),
enum_val( "Disable",            ttmDisable          ),
enum_val( "Count1",             ttmCount1           ),
enum_val( "Count2",             ttmCount2           ),
enum_val( "Count3",             ttmCount3           ),
enum_val( "Count4",             ttmCount4           ),
enum_val( "Count3Projected",    ttmCount3Projected  ),
enum_val( "Count4Projected",    ttmCount4Projected  ),
enum_val( "Projected",          ttmProjected        )
enum_end(TexTMFlags)

/***********************************************************************************/
//  Class:  TexturePass
//  Desc:   
/***********************************************************************************/
class TexturePass : public JObject
{
    TexOp               m_ColorOp;
    TexOp               m_AlphaOp;            
    TexArg              m_ColorArg1;          
    TexArg              m_ColorArg2;
    TexArg              m_AlphaArg1;
    TexArg              m_AlphaArg2;
    TexAddress          m_AddressU;
    TexAddress          m_AddressV;
    TexFilter           m_MagFilter;
    TexFilter           m_MinFilter;
    TexFilter           m_MipFilter;
    float               m_MipMapLodBias;

    TexCoordIndex       m_TexCoordIndex;
    TexTMFlags          m_TextureTransformFlags;

    int                 m_Pass;

public:
                        TexturePass();    
    virtual void        Render();

    expose(TexturePass)
    {
        parent(JObject);
        field( "Pass",                  m_Pass                  );
        field( "ColorOp",               m_ColorOp               );
        field( "AlphaOp",               m_AlphaOp               );
        field( "ColorArg1",             m_ColorArg1             );
        field( "ColorArg2",             m_ColorArg2             );
        field( "AlphaArg1",             m_AlphaArg1             );
        field( "AlphaArg2",             m_AlphaArg2             );
        field( "AddressU",              m_AddressU              );
        field( "AddressV",              m_AddressV              );
        field( "MagFilter",             m_MagFilter             );
        field( "MinFilter",             m_MinFilter             );
        field( "MipFilter",             m_MipFilter             );
        field( "MipMapLodBias",         m_MipMapLodBias         );
        field( "TexCoordIndex",         m_TexCoordIndex         );
        field( "TextureTransformFlags", m_TextureTransformFlags );

    }
}; // class TexturePass

#endif //__TEXTUREPASS_H__