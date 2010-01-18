/*****************************************************************************/
/*	File:  skydome.fx
/*	Desc:  
/*****************************************************************************/
#include "globals.fx"

float3 g_TopColor    = { 0.5f, 0.6f, 1.0f };
float  g_TopBound    = 70.0f;
float3 g_BottomColor = { 0.8f, 0.6f, 0.7f };
float  g_BottomBound = 20.0f;

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
    
    float factor = saturate( (pos.z - g_BottomBound)/(g_TopBound - g_BottomBound) );
    vOut.diffuse.rgb = lerp( g_BottomColor, g_TopColor, factor );
    
    float4 cPos = pos;
    cPos.xyz = mul( cPos.xyz, g_ViewTM );  
    cPos = mul( cPos, g_ProjTM );   
    vOut.pos = cPos; 
        
       
    vOut.diffuse.a = 1.0f; 
    
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
    pOut.color.rgb = vIn.diffuse.rgb;
    pOut.color.a   = 1.0f;
    return pOut;
} // PSMain

Technique Main
{
    Pass P0 
    {
        alphablendenable        = false;
        alphatestenable         = false;
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
