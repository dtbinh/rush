/*****************************************************************************/
/*	File:  thin_model.fx
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
        texturefactor           = 0xFFFFFFFF;
        
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
        lightambient[0]         = float4( 0.9f, 0.9f, 0.8f, 0.9f );
        lighttype[0]            = Directional;
        lightrange[0]           = 1000000;
        
        lightenable[1]          = false;
        
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
    }
};

