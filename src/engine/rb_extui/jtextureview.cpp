//****************************************************************************/
//  File:   JTextureView.cpp
//  Date:   24.10.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "JTextureView.h"

//****************************************************************************/
/*  JTextureView implementation
//****************************************************************************/
decl_class(JTextureView);
JTextureView::JTextureView()
{
    m_TexID     = -1;
    m_TexName   = "";
} // JTextureView::JTextureView

void JTextureView::Render()
{    
    if (m_TexID == -1)
    {
        m_TexID = g_pRenderServer->GetTextureID( m_TexName.c_str() );
    }

    Frame ext = GetExt();

    VertexS quadV[4];
    quadV[0].x = ext.x;
    quadV[0].y = ext.y;
    quadV[0].z = 0.0f;
    quadV[0].u = 0.0f;  
    quadV[0].v = 0.0f;  
    quadV[0].diffuse = GetFgColor();

    quadV[1].x = ext.r();
    quadV[1].y = ext.y;
    quadV[1].z = 0.0f;
    quadV[1].u = 1.0f;  
    quadV[1].v = 0.0f;  
    quadV[0].diffuse = GetFgColor();

    quadV[2].x = ext.x;
    quadV[2].y = ext.b();
    quadV[2].z = 0.0f;
    quadV[2].u = 0.0f;
    quadV[2].v = 1.0f;
    quadV[0].diffuse = GetFgColor();

    quadV[3].x = ext.r();
    quadV[3].y = ext.b();
    quadV[3].z = 0.0f;
    quadV[3].u = 1.0f;
    quadV[3].v = 1.0f;
    quadV[0].diffuse = GetFgColor();

    int texID = m_TexID;
    if (texID == -1)
    {
        texID = g_pRenderServer->GetTextureID( "error_texture.dds" ); 
    }
    int spShaderID = g_pRenderServer->GetShaderID( "base_sprite" );
    g_pRenderServer->SetShader( spShaderID );
    g_pRenderServer->SetTexture( texID, 0 );
    g_pDrawServer->DrawPrimList( quadV, 4, PrimitiveType_QuadList );

} // JTextureView::Render

void JTextureView::SetTexName( const char* name )
{
    m_TexName = name;
    m_TexID   = g_pRenderServer->GetTextureID( name ); 
}

void JTextureView::SetTexID( int id )
{
    m_TexID = id;
    TextureProperties texProp;
    bool bRes = g_pRenderServer->GetTextureProp( id, texProp );
    if (!bRes)
    {
        rlog.warn( "Invalid texture ID (%d) assigned to textureview <%s>", id, GetName() );
        return;
    }
    m_TexName = texProp.m_Name;
}
