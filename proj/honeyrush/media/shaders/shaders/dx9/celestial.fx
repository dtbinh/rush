/*****************************************************************************/
/*	File:  celestial.fx
/*	Desc:  
/*****************************************************************************/
#include "globals.fx"

float4   g_Color = { 1.0f, 0.3f, 0.0f, 0.6f };

/*****************************************************************************/
/*  Samplers
/*****************************************************************************/
sampler DiffuseSampler = sampler_state
{
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = Wrap;
    AddressV  = Wrap;
};

/*****************************************************************************/
/*  Vertex shader output
/*****************************************************************************/
struct VSOut
{
    float4 pos      : POSITION;   // vertex position 
    float4 diffuse  : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 uv       : TEXCOORD0;  // vertex texture coords 
};

/*****************************************************************************/
/*  Vertex shader
/*****************************************************************************/
VSOut VSMain( float4 pos     : POSITION, 
              float2 uv      : TEXCOORD0 )
{
    VSOut vOut;
    
    vOut.diffuse = g_Color;    
    vOut.pos = mul( pos, g_WorldViewProjTM );  
       
    
    vOut.uv = uv; 
    
    return vOut;    
} // RenderSceneVS

/*****************************************************************************/
/*  Pixel shader output
/*****************************************************************************/
struct PSOut
{
    float4 color : COLOR0;   
};

/*****************************************************************************/
/*  Pixel shader  
/*****************************************************************************/
PSOut PSMain( VSOut vIn ) 
{ 
    PSOut pOut;
    float4 decal = tex2D( DiffuseSampler, vIn.uv );
    pOut.color = decal*vIn.diffuse;
    return pOut;
} // PSMain

Technique Main
{
    Pass P0 
    {
        alphablendenable        = true;
        srcblend                = srcalpha;
        destblend               = one;
        alphatestenable         = true;
        alphafunc               = greaterequal;
        alpharef                = 0x01;
        ditherenable            = true;
        zenable                 = true;
        zwriteenable            = false;
        zfunc                   = lessequal;
        fillmode                = solid;
        cullmode                = none;
        
        VertexShader = compile vs_1_1 VSMain();
        PixelShader  = compile ps_1_1 PSMain(); 
    }
};
