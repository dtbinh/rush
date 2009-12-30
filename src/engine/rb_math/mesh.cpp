/***********************************************************************************/
//  File:   Mesh.cpp
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "mesh.h"

/***********************************************************************************/
/*  Mesh implementation
/***********************************************************************************/
Mesh::Mesh()
{
    m_dRHW          = -1;
    m_dNormal       = -1;
    m_dTangent      = -1;
    m_dBinormal     = -1;
    m_dUV           = -1;
    m_dBlendWeight  = -1;
    m_dBlendIdx     = -1;
    m_dDiffuse      = -1;
    m_dSpecular     = -1;
} // Mesh::Mesh

void Mesh::Clear()
{
    m_Faces.clear();
    m_VertexData.clear();
} // Mesh::Clear

void Mesh::AddVert( const Vec3& pos )
{
    int cVert = GetNVert();
    m_VertexData.resize( m_VertexData.size() + GetStride() );
    SetPos( cVert, pos );
} // Mesh::AddVert

void Mesh::ClearFaces()
{
    m_Faces.clear();
} // Mesh::ClearFaces

void Mesh::ClearVerts()
{
    m_VertexData.clear();
} // Mesh::ClearVerts

void Mesh::Create( const VertexDeclaration& vd )
{
    m_VDecl = vd;
} // Mesh::Create

const Vec3& Mesh::GetPos( int idx ) const
{
    int vuint8_t = idx*GetStride();
    assert( vuint8_t < m_VertexData.size() );
    return *((Vec3*)&m_VertexData[vuint8_t]);
} // Mesh::GetVertPos

float Mesh::GetRHW( int idx ) const
{
    return 1.0f;
} // Mesh::GetRH

const Vec3& Mesh::GetNormal( int idx ) const
{
    return Vec3::oZ;
} // Mesh::GetNormal

const Vec3& Mesh::GetTangent( int idx ) const
{
    return Vec3::null;
} // Mesh::GetTangent

const Vec3& Mesh::GetBinormal( int idx ) const
{
    return Vec3::null;
} // Mesh::GetBinormal

const Vec2& Mesh::GetUV( int idx ) const
{
    return Vec2::null;
} // Mesh::GetUV

const Vec2& Mesh::GetUV( int idx, int uvPair ) const
{
    return Vec2::null;
} // Mesh::GetUV

float Mesh::GetBlendWeight( int idx, int wIdx ) const
{
    return 0.0f;
} // Mesh::GetBlendWeight

uint8_t Mesh::GetBlendIdx( int idx, int wIdx ) const
{
    return 255;
} // Mesh::GetBlendIdx

uint32_t Mesh::GetDiffuse( int idx ) const
{
    return 0;
} // Mesh::GetDiffuse

uint32_t Mesh::GetSpecular( int idx ) const
{
    return 0;
} // Mesh::GetSpecular

bool Mesh::SetPos( int idx, const Vec3& pos )
{
    return true;
} // Mesh::SetPos

bool Mesh::SetRHW( int idx, float rhw )
{
    return true;
} // Mesh::SetRHW

bool Mesh::SetRHW( float rhw )
{
    return true;
} // Mesh::SetRHW

bool Mesh::SetNormal( int idx, const Vec3& normal )
{
    return true;
} // Mesh::SetNormal

bool Mesh::SetTangent( int idx, const Vec3& tangent )
{
    return true;
} // Mesh::SetTangent

bool Mesh::SetBinormal( int idx, const Vec3& binormal )
{
    return true;
} // Mesh::SetBinormal

bool Mesh::SetUV( int idx, const Vec2& uv ) const
{
    return true;
} // Mesh::SetUV

bool Mesh::SetUV( int idx, int uvPair, const Vec2& uv ) const
{
    return true;
} // Mesh::SetUV

bool Mesh::SetBlendWeight( int idx, int wIdx, float w ) const
{
    return true;
} // Mesh::SetBlendWeight

bool Mesh::SetBlendIdx( int idx, int wIdx, uint8_t blendIdx ) const
{
    return true;
} // Mesh::SetBlendIdx

bool Mesh::SetDiffuse( int idx, uint32_t diffuse ) const
{
    return true;
} // Mesh::SetDiffuse

bool Mesh::SetSpecular( int idx, uint32_t specular ) const
{
    return true;
} // Mesh::SetSpecular

void Mesh::SetNFaces( int nFaces )
{

} // Mesh::SetNFaces

void Mesh::SetNVerts( int nVerts )
{

} // Mesh::SetNVerts

