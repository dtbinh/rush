/*****************************************************************************/
/*	File:  globals.fx
/*	Desc:  
/*****************************************************************************/
shared float4x4	    g_WorldTM;				
shared float4x4	    g_ViewTM;
shared float4x4	    g_ProjTM;
shared float4x4     g_WorldViewProjTM;
shared float3       g_CamPos        = { 0.0f, 0.0f, 0.0f };
shared float        g_Time          = 0.0f;

shared float3	    g_MtlDiffuse    = { 1.0f, 1.0f, 1.0f };
shared float3	    g_MtlAmbient    = { 1.0f, 1.0f, 1.0f };

shared float3	    g_Ambience      = { 0.6f, 0.6f, 0.6f };
shared float3	    g_SunDirection  = { 0.84f, -0.84f, 0.4f };
shared float3	    g_SunDiffuse	= { 1.0f, 1.0f, 1.0f };
shared float3	    g_SunSpecular	= { 1.0f, 1.0f, 1.0f };

shared float        g_FogRange      = 4000.0f;
shared float        g_FogEnd        = 50000.0f;
shared float3       g_FogColor      = { 0.5f, 0.3f, 1.0f };

