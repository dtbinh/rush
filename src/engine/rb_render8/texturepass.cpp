/***********************************************************************************/
//  File:   TexturePass.cpp
//  Date:   16.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "TexturePass.h"

/***********************************************************************************/
/*  TexturePass implementation
/***********************************************************************************/
decl_class( TexturePass );
TexturePass::TexturePass()
{
    m_ColorOp       = toUnknown;
    m_AlphaOp       = toUnknown;
    m_ColorArg1     = taUnknown;
    m_ColorArg2     = taUnknown;
    m_AlphaArg1     = taUnknown;
    m_AlphaArg2     = taUnknown;
    m_AddressU      = tdUnknown;
    m_AddressV      = tdUnknown;
    m_MagFilter     = tfUnknown;
    m_MinFilter     = tfUnknown;
    m_MipFilter     = tfUnknown;
    m_MipMapLodBias = 0.0f;
    m_TexCoordIndex = tciUnknown;
    m_TextureTransformFlags = ttmUnknown;

    m_Pass          = 0.0f;
} // TexturePass::TexturePass

IDirect3DDevice8* GetDirect3DDevice8();
void TexturePass::Render()
{
    IDirect3DDevice8* pDev = GetDirect3DDevice8();
    if (!pDev) return;

    if (m_ColorOp   != toUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_COLOROP,     D3DTEXTUREOP( m_ColorOp ) );
    if (m_AlphaOp   != toUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_ALPHAOP,     D3DTEXTUREOP( m_AlphaOp ) );
    if (m_ColorArg1 != taUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_COLORARG1,   m_ColorArg1 - 1 );
    if (m_ColorArg2 != taUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_COLORARG2,   m_ColorArg2 - 1 );
    if (m_AlphaArg1 != taUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_ALPHAARG1,   m_AlphaArg1 - 1 );
    if (m_AlphaArg2 != taUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_ALPHAARG2,   m_AlphaArg2 - 1 );
    if (m_AddressU  != tdUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_ADDRESSU,    m_AddressU  );
    if (m_AddressV  != tdUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_ADDRESSV,    m_AddressV  );
    if (m_MagFilter != tfUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_MAGFILTER,   m_MagFilter - 1 );
    if (m_MinFilter != tfUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_MINFILTER,   m_MinFilter - 1 );
    if (m_MipFilter != tfUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_MIPFILTER,   m_MipFilter - 1 );
    if (m_MipMapLodBias != 0.0f)  
        pDev->SetTextureStageState( m_Pass,  D3DTSS_MIPMAPLODBIAS, *((LPDWORD)(&m_MipMapLodBias)) );
    
    if (m_TexCoordIndex != tciUnknown) pDev->SetTextureStageState( m_Pass,  D3DTSS_TEXCOORDINDEX, m_TexCoordIndex );
    if (m_TextureTransformFlags != ttmUnknown) 
        pDev->SetTextureStageState( m_Pass,  D3DTSS_TEXTURETRANSFORMFLAGS, m_TextureTransformFlags );

} // TexturePass::Render
