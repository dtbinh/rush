/*****************************************************************************/
/*	File:  base_model.fx
/*	Desc:  
/*****************************************************************************/
#include "globals.fx"

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
    float3 fog      : TEXCOORD1;
};

/*****************************************************************************/
/*  Vertex shader
/*****************************************************************************/
VSOut VSMain( float4 pos     : POSITION, 
              float3 normal  : NORMAL,
              float2 uv      : TEXCOORD0 )
{
    VSOut vOut;
    
    float3 cPos = pos.xyz;
    cPos = mul( cPos.xyz, (float3x3)g_WorldTM );
    cPos += float3( g_WorldTM._m30, g_WorldTM._m31, g_WorldTM._m32 );
    cPos = mul( cPos, g_ViewTM );
    cPos = mul( cPos, g_ProjTM );
    vOut.pos = float4( cPos, pos.w );
    
    vOut.pos.xyz = mul( pos.xyz, g_WorldTM );
    vOut.pos.xyz = mul( (float3x3)g_ViewTM, vOut.pos.xyz );
    vOut.pos.xyz += float3( g_WorldTM._m30, g_WorldTM._m31, g_WorldTM._m32 );
    vOut.pos = mul( vOut.pos, g_ViewTM );
    vOut.pos = mul( vOut.pos, g_ProjTM );
    
    normal = normalize( mul( normal, (float3x3)g_WorldTM ) );  
        
    float3 diffuse = saturate( g_SunDiffuse*dot( normal, g_SunDirection ) );
        
    vOut.diffuse.rgb = g_MtlDiffuse*diffuse + g_MtlAmbient*g_Ambience;   
    vOut.diffuse.a = 1.0f; 
    
    vOut.uv = uv; 
    
    vOut.fog = g_FogColor*( (vOut.pos.z - g_FogEnd)/g_FogRange );
    
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
    pOut.color.rgb = decal.rgb*vIn.diffuse.rgb + vIn.fog;
    pOut.color.a   = vIn.diffuse.a*decal.a;
    return pOut;
} // RenderScenePS

Technique PerVertexLighting
{
    Pass P0 
    {
        alphablendenable        = false;
        alphatestenable         = true;
        alphafunc               = greaterequal;
        alpharef                = 0x80;
        ditherenable            = true;
        specularenable          = false;
        zenable                 = true;
        zwriteenable            = true;
        zfunc                   = lessequal;
        fillmode                = solid;
        cullmode                = none;
        shademode               = gouraud;
        
        VertexShader = compile vs_1_1 VSMain();
        PixelShader  = compile ps_1_1 PSMain(); 
    }
};
