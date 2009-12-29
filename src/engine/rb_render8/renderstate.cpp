/***********************************************************************************/
//  File:   RenderState.cpp
//  Date:   16.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "RenderState.h"

/***********************************************************************************/
/*  RenderState implementation
/***********************************************************************************/
decl_class( RenderState );
RenderState::RenderState()
{
    m_bAlphaBlendEnable     = bUnknown;
    m_SrcBlend              = bmUnknown;
    m_DestBlend             = bmUnknown;
    m_bAlphaTestEnable      = bUnknown;
    m_AlphaFunc             = cfUnknown;
    m_bDitherEnable         = bUnknown;
    m_bLighting             = bUnknown;
    m_bZEnable              = bUnknown;
    m_bZWriteEnable         = bUnknown;
    m_ZFunc                 = cfUnknown;
    m_CullMode              = cmUnknown;
    m_TextureFactor         = 0x00000000;
    m_AlphaRef              = 0x00;
    m_bFogEnable            = bUnknown;
    m_FillMode              = fmUnknown;

    m_FogColor              = 0x00000000;
    m_FogVertexMode         = fvmUnknown;
    m_FogStart              = 0.0f;
    m_FogEnd                = 0.0f;
    m_bRangeFogEnable       = bUnknown;

    m_bNormalizeNormals     = bUnknown;
    m_ShadeMode             = ShadeMode_Unknown;
    m_ColorVertex           = bUnknown;
    m_bSpecularEnable       = bUnknown;

    m_DiffuseMaterialSource     = msUnknown;
    m_SpecularMaterialSource    = msUnknown;
    m_AmbientMaterialSource     = msUnknown;
    m_EmissiveMaterialSource    = msUnknown;
} // RenderState::RenderState

IDirect3DDevice8* GetDirect3DDevice8();

void RenderState::Render()
{
    IDirect3DDevice8* pDev = GetDirect3DDevice8();
    if (!pDev) return;

    if (m_bAlphaBlendEnable != bUnknown)
        pDev->SetRenderState( D3DRS_ALPHABLENDENABLE, m_bAlphaBlendEnable == bTrue ? TRUE : FALSE );

    if (m_SrcBlend != bmUnknown) 
        pDev->SetRenderState( D3DRS_SRCBLEND, (D3DBLEND)m_SrcBlend );
    
    if (m_DestBlend != bmUnknown) 
        pDev->SetRenderState( D3DRS_DESTBLEND, (D3DBLEND)m_DestBlend);

    if (m_bAlphaTestEnable != bUnknown)
        pDev->SetRenderState( D3DRS_ALPHATESTENABLE, m_bAlphaTestEnable == bTrue ? TRUE : FALSE );

    if (m_AlphaFunc != cfUnknown) pDev->SetRenderState( D3DRS_ALPHAFUNC, (D3DCMPFUNC)m_AlphaFunc );
    if (m_AlphaRef != 0x00) pDev->SetRenderState( D3DRS_ALPHAREF, m_AlphaRef );
    
    if (m_bDitherEnable != bUnknown)
        pDev->SetRenderState( D3DRS_DITHERENABLE, m_bDitherEnable == bTrue ? TRUE : FALSE );

    if (m_bLighting != bUnknown)
        pDev->SetRenderState( D3DRS_LIGHTING, m_bLighting == bTrue ? TRUE : FALSE );

    if (m_bZEnable != bUnknown)
        pDev->SetRenderState( D3DRS_ZENABLE, m_bZEnable == bTrue ? TRUE : FALSE );

    if (m_bZWriteEnable != bUnknown)
        pDev->SetRenderState( D3DRS_ZWRITEENABLE, m_bZWriteEnable == bTrue ? TRUE : FALSE );

    if (m_ZFunc != cfUnknown) 
        pDev->SetRenderState( D3DRS_ZFUNC, (D3DCMPFUNC)m_ZFunc );
    
    if (m_FillMode != fmUnknown) 
        pDev->SetRenderState( D3DRS_FILLMODE, (D3DFILLMODE)m_FillMode );
    
    if (m_CullMode != fmUnknown) 
        pDev->SetRenderState( D3DRS_CULLMODE, (D3DCULL)m_CullMode );
    
    if (m_TextureFactor != 0x00) 
        pDev->SetRenderState( D3DRS_TEXTUREFACTOR, m_TextureFactor );

    if (m_bFogEnable != bUnknown)
        pDev->SetRenderState( D3DRS_FOGENABLE, m_bFogEnable == bTrue ? TRUE : FALSE );

    if (m_FogColor != 0x00000000)
        pDev->SetRenderState( D3DRS_FOGCOLOR, m_FogColor );

    if (m_FogVertexMode != fvmUnknown)
        pDev->SetRenderState( D3DRS_FOGVERTEXMODE,  m_FogVertexMode - 1 );
    
    if (m_FogStart != 0.0f)
        pDev->SetRenderState( D3DRS_FOGSTART,  *(DWORD*)(&m_FogStart) );

    if (m_FogEnd != 0.0f)
        pDev->SetRenderState( D3DRS_FOGEND,  *(DWORD*)(&m_FogEnd) );
    
    if (m_bRangeFogEnable != bUnknown)
        pDev->SetRenderState( D3DRS_RANGEFOGENABLE, m_bRangeFogEnable == bTrue ? TRUE : FALSE );
    
    if (m_bNormalizeNormals != bUnknown)
        pDev->SetRenderState( D3DRS_NORMALIZENORMALS, m_bNormalizeNormals == bTrue ? TRUE : FALSE );

    if (m_bSpecularEnable != bUnknown)
        pDev->SetRenderState( D3DRS_SPECULARENABLE, m_bSpecularEnable == bTrue ? TRUE : FALSE );

    if (m_ColorVertex != bUnknown)
        pDev->SetRenderState( D3DRS_COLORVERTEX, m_ColorVertex == bTrue ? TRUE : FALSE );

    if (m_ShadeMode != cfUnknown) 
        pDev->SetRenderState( D3DRS_SHADEMODE, m_ShadeMode );
    
    if (m_DiffuseMaterialSource != msUnknown) 
        pDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, m_DiffuseMaterialSource - 1 );
    if (m_SpecularMaterialSource != msUnknown) 
        pDev->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, m_SpecularMaterialSource - 1 );
    if (m_AmbientMaterialSource != msUnknown) 
        pDev->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, m_AmbientMaterialSource - 1 );
    if (m_EmissiveMaterialSource != msUnknown) 
        pDev->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE, m_EmissiveMaterialSource - 1 );

} // RenderState::Render