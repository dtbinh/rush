/*****************************************************************************/
/*	File:  skydome.fx
/*	Desc:  
/*****************************************************************************/
#include "globals.fx"

float2 g_CloudSpeed = { 0.004f, 0.01f };

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
    
    vOut.diffuse.rgb = float3( 1.0f, 1.0f, 1.0f );    
    float4 cPos = pos;
    cPos.xyz = mul( cPos.xyz, g_ViewTM ); 
    cPos = mul( cPos, g_ProjTM );   
    vOut.pos = cPos; 
        
       
    vOut.diffuse.a = 1.0f; 
    
    vOut.uv = uv + g_CloudSpeed*g_Time; 
    
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
    pOut.color.rgb = decal.rgb*vIn.diffuse.rgb;
    pOut.color.a   = decal.a;
    return pOut;
} // PSMain

Technique Main
{
    Pass P0 
    {
        alphablendenable        = true;
        srcblend                = srcalpha;
        destblend               = invsrcalpha;
        alphatestenable         = true;
        alphafunc               = greaterequal;
        alpharef                = 0x01;
        ditherenable            = true;
        zenable                 = true;
        zwriteenable            = false;
        zfunc                   = lessequal;
        fillmode                = solid;
        cullmode                = ccw;
        
        VertexShader = compile vs_1_1 VSMain();
        PixelShader  = compile ps_1_1 PSMain(); 
    }
};
