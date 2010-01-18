/***********************************************************************************/
//  File:   MeshUtils.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "FStream.h" 
#include "AABox.h"
#include "Tri2.h"
#include "JModel.h"
#include "JMesh.h"
#include "JMaterial.h"
#include "../rb_texture/ColorSurface.h"
      
struct SortPoly
{
    WORD    a, b, c;
    float   transpRatio;

    bool operator < (const SortPoly& p) const
    {
        return (transpRatio < p.transpRatio);
    }
};

inline void WrapUV( Vec2& uv )
{
    while (uv.x < 0.0f) uv.x += 1.0f;
    while (uv.x > 1.0f) uv.x -= 1.0f;
    while (uv.y < 0.0f) uv.y += 1.0f;
    while (uv.y > 1.0f) uv.y -= 1.0f;
}

void SortTransparentPolys( JModel* pModel )
{
    if (!pModel) return;
    int nMeshes = pModel->GetNMeshes();
    int cMtl = -1;
    ColorSurface surf;
    Buffer uv, idx;
    for (int i = 0; i < nMeshes; i++)
    {
        JMesh* pMesh = pModel->GetMesh( i );
        if (!pMesh->IsVisible()) 
        {
            continue;
        }
        int mtlID = pMesh->GetMaterialID();
        pMesh->GetComponents( uv, VertexComponent_TexCoor0 );
        pMesh->GetIndices( idx );
        if (mtlID >= 0 && mtlID != cMtl)
        {
            JMaterial* pMtl = pModel->GetMaterial( mtlID );
            int texID = pMtl->GetDiffuseMapID();
            if (texID == -1) continue;
            surf.ReadFromTexture( texID );
            cMtl = mtlID;
        }
        float texW = (float)surf.GetW();
        float texH = (float)surf.GetH();
        //  scan through polys, calculating their transparency ratio
        int nPolys = pMesh->GetNIndices()/3;
        std::vector<SortPoly> polys;
        polys.resize( nPolys );
        Vec2* pUV = (Vec2*)uv.GetData();
        WORD* pIdx = (WORD*)idx.GetData();
        for (int i = 0; i < nPolys; i++)
        {
            SortPoly& poly = polys[i];
            poly.a = pIdx[i*3 + 0];
            poly.b = pIdx[i*3 + 1];
            poly.c = pIdx[i*3 + 2];
            Tri2 tri( pUV[poly.a], pUV[poly.b], pUV[poly.c] );
            WrapUV( tri.a );
            WrapUV( tri.b );
            WrapUV( tri.c );

            tri.a.x *= texW;
            tri.a.y *= texH;
            tri.b.x *= texW;
            tri.b.y *= texH;
            tri.c.x *= texW;
            tri.c.y *= texH;

            int nPixels = abs( (int)tri.signed_area() );
            int nTransp = 0;
            for (int j = 0; j < nPixels; j++)
            {
                Vec2 pos = tri.from_bary_coords( Vec2( frand(), frand() ) );
                float alpha = surf.GetColor( pos.x, pos.y ).a;
                if (alpha < 1.0f)
                {
                    nTransp++;
                }
            }
            poly.transpRatio = float( nTransp );
        }   
        std::stable_sort( polys.begin(), polys.end() );
        //  write sorted indices back
        for (int i = 0; i < nPolys; i++)
        {
            SortPoly& poly = polys[i];
            pIdx[i*3 + 0] = poly.a;
            pIdx[i*3 + 1] = poly.b;
            pIdx[i*3 + 2] = poly.c;
        }
        pMesh->SetIndices( idx );
    }
    pModel->Init();
} // SortTransparentPolys
