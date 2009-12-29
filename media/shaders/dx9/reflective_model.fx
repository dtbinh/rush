/*****************************************************************************/
/*	File:  base_model.fx
/*	Desc:  
/*****************************************************************************/
#include "globals.fx"

Technique FFP
{
    Pass P0 
    {
        alphablendenable        = true;
        srcblend                = srcalpha;
        destblend               = invsrcalpha;
        alphatestenable         = true;
        alphafunc               = greaterequal;
        alpharef                = 0x01;
        ditherenable            = false;
        specularenable          = true;
        lighting                = true;
        zenable                 = true;
        zwriteenable            = true;
        zfunc                   = lessequal;
        fillmode                = solid;
        cullmode                = none;
        
        normalizenormals        = true;
        shademode               = gouraud;
        colorvertex             = false;
        diffusematerialsource   = material;
        ambientmaterialsource   = material;
        materialdiffuse         = float4( 1.0f, 1.0f, 1.0f, 1.0f );
        materialambient         = float4( 1.0f, 1.0f, 1.0f, 1.0f );
        
        fogenable               = false;
        fogstart                = 4000.0f;
        fogend                  = 7000.0f;
        fogcolor                = 0xFF000000;
        fogvertexmode           = linear;
        rangefogenable          = true;
        
        lightenable[0]          = true;
        lightdirection[0]       = float3( 0.64f, -0.64f, 0.7f );
        lightdiffuse[0]         = float4( 1.0f, 1.0f, 1.0f, 1.0f );
        lightambient[0]         = float4( 0.5f, 0.4f, 0.3f, 0.6f );
        lighttype[0]            = Directional;
        lightrange[0]           = 1000000;
        
        lightenable[1]          = true;
        lightdirection[1]       = float3( -0.2f, 0.4f, 0.8f );
        lightdiffuse[1]         = float4( 0.4f, 0.4f, 0.3f, 0.2f );
        lightambient[1]         = float4( 0.2f, 0.2f, 0.2f, 0.0f );
        lighttype[1]            = Directional;
        lightrange[1]           = 1000000;
        
        colorop[0]              = Modulate;
        colorarg1[0]            = Texture;
        colorarg2[0]            = Diffuse;
        
        alphaop[0]              = Modulate;
        alphaarg1[0]            = Texture;
        alphaarg2[0]            = Diffuse;
        
        addressu[0]             = wrap;
        addressv[0]             = wrap;
        magfilter[0]            = linear;
        minfilter[0]            = linear;
        mipfilter[0]            = linear;
        
        texturetransformflags[0]    = Disable;
        texcoordindex[0]            = 0;
        
        
        colorop[1]              = Modulate;
        colorarg1[1]            = Current;
        colorarg2[1]            = TFactor;
        
        alphaop[1]              = Modulate;
        alphaarg1[1]            = Current;
        alphaarg2[1]            = TFactor;
        
        
        colorop[2]              = Add;
        colorarg1[2]            = Current;
        colorarg2[2]            = Texture;
        
        alphaop[2]              = SelectArg1;
        alphaarg1[2]            = Current;
        
        addressu[2]             = wrap;
        addressv[2]             = wrap;
        magfilter[2]            = linear;
        minfilter[2]            = linear;
        mipfilter[2]            = linear;
        
        texturetransformflags[2]    = Count3;
        texcoordindex[2]            = CameraSpaceReflectionVector|1;
    }
};
