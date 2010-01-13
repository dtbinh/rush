/***********************************************************************************/
//  File:   JMaterial.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JMaterial.h"

/***********************************************************************************/
/*  JMaterial implementation
/***********************************************************************************/
decl_class(JMaterial);

JMaterial::JMaterial() : 
                m_ShaderName    ( "" ),
                m_DiffuseMap    ( "white.bmp" ),
                m_Diffuse       ( 0xFFFFFFFF ),
                m_Specular      ( 0xFFFFFFFF ),
                m_SpecularPower ( 32.0f ),
                m_Transparency  ( 1.0f ),
                m_bHasAlpha     ( false ),
                m_ShaderID      ( -1 ),
                m_DiffuseMapID  ( -1 ),
                m_DetailMapID   ( -1 ),
                m_Diffuse2MapID ( -1 ),
                m_NormalMapID   ( -1 ),
                m_EnvMapID      ( -1 )
{

}

bool JMaterial::IsEqual( JMaterial* pMtl ) const
{
    if (stricmp( m_ShaderName.c_str(),  pMtl->m_ShaderName.c_str()  )) return false;
    if (stricmp( m_DiffuseMap.c_str(),  pMtl->m_DiffuseMap.c_str()  )) return false;
    if (stricmp( m_Diffuse2Map.c_str(), pMtl->m_Diffuse2Map.c_str() )) return false;
    if (stricmp( m_NormalMap.c_str(),   pMtl->m_NormalMap.c_str()   )) return false;
    if (stricmp( m_EnvMap.c_str(),      pMtl->m_EnvMap.c_str()      )) return false;
        
    if (m_Diffuse       != pMtl->m_Diffuse      ) return false;
    if (m_Specular      != pMtl->m_Specular     ) return false;
    if (m_SpecularPower != pMtl->m_SpecularPower) return false;
    if (m_Transparency  != pMtl->m_Transparency ) return false;
    if (m_bHasAlpha     != pMtl->m_bHasAlpha    ) return false;

    return true;
} // JMaterial::IsEqual

void JMaterial::Init()
{
    if (!g_pRenderServer) return;
    m_ShaderID          = g_pRenderServer->GetShaderID ( m_ShaderName.c_str()   );
    m_DiffuseMapID      = g_pRenderServer->GetTextureID( m_DiffuseMap.c_str()   );
    m_Diffuse2MapID     = g_pRenderServer->GetTextureID( m_Diffuse2Map.c_str()  );
    m_NormalMapID       = g_pRenderServer->GetTextureID( m_NormalMap.c_str()    );
    m_EnvMapID          = g_pRenderServer->GetTextureID( m_EnvMap.c_str()       );

    if (m_DiffuseMapID == -1) m_DiffuseMapID = g_pRenderServer->GetTextureID( "white.tga" );
} // JMaterial::Init

void JMaterial::Render( bool bIgnoreShader )
{

    if (m_DiffuseMapID  != -1) g_pRenderServer->SetTexture( m_DiffuseMapID,  0 );
    if (m_Diffuse2MapID != -1) g_pRenderServer->SetTexture( m_Diffuse2MapID, 1 );
    if (m_NormalMapID   != -1) g_pRenderServer->SetTexture( m_NormalMapID,   1 );
    if (m_EnvMapID      != -1) g_pRenderServer->SetTexture( m_EnvMapID,      2 );

    if (!bIgnoreShader)
    {
        g_pRenderServer->SetShader ( m_ShaderID );
    }
} // JMaterial::Render
    