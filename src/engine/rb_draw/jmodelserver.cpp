//****************************************************************************/
//  File:  JModelServer.cpp
//  Desc:  
//****************************************************************************/
#include "precompile.h"
#include <algorithm>
#include "vertextype.h"
#include "fstream.h"
#include "vec4.h"
#include "aabox.h"
#include "frustum.h"
#include "vec3.h"
#include "warping.h"
#include "colorconversion.h"

#include "irenderserver.h"

#include "jmodel.h"
#include "jmodelanim.h"
#include "ipersistserver.h"
#include "janmsection.h"
#include "jmodelserver.h"

const int c_StaticVBSize = 1024*1024*8;
const int c_StaticIBSize = 1024*1024*4;

//****************************************************************************/
/*	JModelServer implementation
//****************************************************************************/
decl_class(JModelServer);
JModelServer::JModelServer() 
{
    m_bShowShadowMap    = false;
    m_ShadowMapID       = -1;
    m_LightDir          = Vec3( 0.4f, 0.4, -0.8f );

    m_ShadowRange       = 100.0f;
    m_ShadowColor       = 0xFFAAAAAA;
    m_ShadowMapSide     = 512;
    m_bEnableShadows    = true;

    m_LightDir.normalize();

    SetName( "model" );
    
    g_pModelServer = this;
} // JModelServer::JModelServer

JModelServer::~JModelServer()
{
} // JModelServer::~JModelServer

void JModelServer::Init()
{
    g_pRenderServer->CreateVB( "DrawSystemStatic", c_StaticVBSize, true );
    g_pRenderServer->CreateIB( "DrawSystemStatic", c_StaticIBSize, true );
}

JModel* JModelServer::GetModel( int mID )
{
    if (mID < 0 || mID >= m_Models.size())
    {
        return NULL;
    }
    return m_Models[mID];
} // JModelServer::GetModel

int JModelServer::GetModelID( const char* name, JModel* pModel )
{
    if (name == NULL || name[0] == 0)
    {
        return -1;
    }
    Path path( name );
    path.SetExt( "rmdl" );
    int nM = (int)m_Models.size();
    for (int i = 0; i < nM; i++)
    {
        if (!stricmp( path.GetFile(), m_Models[i]->GetName() ))
        {
            return i;
        }
    }

    if (!pModel)
    {
        pModel = obj_cast<JModel>( g_pPersistServer->LoadObject( path.GetFullPath() ) );
        if (!pModel) return -1;
        
        pModel->InitTree();
    }

    int id = m_Models.size();

    pModel->SetName( path.GetFile() );
    pModel->SetModelID( id );

    pModel->Init();
    m_Models.push_back( pModel );

    AddChild( pModel );
    pModel->AddRef();
    
    return id;
} // JModelServer::GetModelID

void JModelServer::DrawModel( int mID, const Mat4& tm, int instID )
{
    if (mID < 0 || mID >= m_Models.size()) return;
    JModel* pModel = m_Models[mID];
    JBoneInstance* pBones = NULL;
    if (instID >= 0 && m_ModelInstances[instID].m_Bones.size() > 0) 
    {
        pBones = &m_ModelInstances[instID].m_Bones[0];
        m_ModelInstances[instID].m_RootTM = tm;
    }
    
    pModel->Render( tm, pBones );
} // JModelServer::DrawModel

AABox JModelServer::GetModelAABB( int mID )
{
    return AABox();
}

JModelAnim* JModelServer::GetAnim( int animID )
{
    if (animID < 0 || animID >= m_Anims.size()) return NULL;
    return m_Anims[animID];
}

int JModelServer::GetAnimID( const char* name, JModelAnim* pAnim )
{
    Path path( name );
    path.SetExt( "ranm" );
    int nM = (int)m_Anims.size();
    for (int i = 0; i < nM; i++)
    {
        if (!stricmp( path.GetFile(), m_Anims[i]->GetName() ))
        {
            return i;
        }
    }

    if (!pAnim)
    {
        pAnim = obj_cast<JModelAnim>( g_pPersistServer->LoadObject( path.GetFullPath(), pAnim ) );
        if (!pAnim ) return -1;
    }

    pAnim->Init();
    m_Anims.push_back( pAnim );

    AddChild( pAnim );
    pAnim->AddRef();
    pAnim->SetName( path.GetFile() );

    return m_Anims.size() - 1;
} // JModelServer::GetAnimID

int JModelServer::InstanceModel( int mID )
{
    if (mID < 0 || mID >= m_Models.size())
    {
        return -1;
    }
    JModel* pModel = m_Models[mID];
    m_ModelInstances.push_back( ModelInstance() );
    ModelInstance& inst = m_ModelInstances.back();
    inst.m_pModel = pModel;
    inst.m_Bones.resize( pModel->GetNBones() );
    inst.m_RootTM = Mat4::identity;
    memcpy( &inst.m_Bones[0], pModel->GetBoneInstances(), 
            pModel->GetNBones()*sizeof( JBoneInstance ) );
    return m_ModelInstances.size() - 1;
}  

float JModelServer::GetAnimDuration( int animID, int sectionID ) const
{
    if (animID < 0 || animID >= m_Anims.size())
    {
        return 0.0f;
    } 
    JModelAnim* pAnim  = m_Anims[animID]; 
    if (sectionID >= 0)
    {
        JAnmSection* pSection = obj_cast<JAnmSection>( pAnim->GetChild( sectionID ) );
        if (pSection)
        {
            return pSection->GetEnd() - pSection->GetStart();
        }
    }
    return pAnim->GetDuration();
} // JModelServer::GetAnimDuration

int JModelServer::GetAnimSectionID( int animID, const char* name ) const
{
    if (animID < 0 || animID >= m_Anims.size())
    {
        return -1;
    }
    JModelAnim* pAnim  = m_Anims[animID]; 
    JAnmSection* pSection = g_pObjectServer->FindObject<JAnmSection>( name, pAnim );
    if (!pSection)
    {
        return -1;
    }
    return pAnim->GetChildIndex( pSection );
} // JModelServer::GetAnimSectionID

void JModelServer::AnimateModel( int mID, const Mat4& tm, int anmID, float cTime, int instID, float weight, int sectionID )
{
    if (mID < 0 || anmID < 0 || mID >= m_Models.size() || anmID >= m_Anims.size())
    {
        return;
    }
    JModel*          pModel = m_Models[mID];
    JModelAnim*      pAnim  = m_Anims[anmID];   
    JBoneInstance*   pBones = pModel->GetBoneInstances();

    if (pAnim->GetModelID() != mID)
    {
        pAnim->Bind( pModel );
        pAnim->SetModelID( mID );
    }
    
    if (instID >= 0)
    {
        ModelInstance& inst = m_ModelInstances[instID];
        pBones = inst.m_Bones.size() > 0 ? &(inst.m_Bones[0]) : NULL;
    }

    if (sectionID >= 0)
    {
        JAnmSection* pSection = obj_cast<JAnmSection>( pAnim->GetChild( sectionID ) );
        if (pSection)
        {
            cTime += pSection->GetStart();
        }
    }

    pAnim->Apply( pBones, cTime, weight );
} // JModelServer::AnimateModel

JModel* JModelServer::GetModelByInst( int instID )
{
    if (instID >= 0)
    {
        ModelInstance& inst = m_ModelInstances[instID];
        return inst.m_pModel;
    }
    return NULL;
} //JModelServer::GetModelByInst

int JModelServer::GetModelBoneID( int mID, const char* boneName ) const
{
    if (mID < 0 || mID >= m_Models.size())
    {
        return -1;
    }
    JModel* pModel = m_Models[mID];
    return pModel->FindBoneInstance( boneName );
} // JModelServer::GetModelBoneID

bool JModelServer::SetBoneTM( int instID, int boneID, Mat4& tm, bool bLocal )
{
    if (instID < 0 || boneID < 0 || m_ModelInstances[instID].m_Bones.size() <= 0) 
    {
        return false;
    }
    ModelInstance& inst = m_ModelInstances[instID];
    JBoneInstance& bone = inst.m_Bones[boneID];
    const Mat4& parentWorldTM = (bone.m_ParentIdx == -1) ? Mat4::identity : 
        inst.m_Bones[bone.m_ParentIdx].m_ModelTM;
    if (bLocal)
    {
        bone.m_LocalTM = tm;
        bone.m_ModelTM = tm*parentWorldTM;
    }
    else
    {
        Mat4 invParentTM, invRootTM;
        invParentTM.inverse( parentWorldTM );
        invRootTM.inverse( inst.m_RootTM );
        bone.m_ModelTM = tm*invRootTM;
        bone.m_LocalTM = bone.m_ModelTM*invParentTM;
    }
    return true;
} // JModelServer::SetBoneTM

bool JModelServer::GetBoneTM( int instID, int boneID, Mat4& tm ) const
{
    if (instID < 0 || m_ModelInstances[instID].m_Bones.size() <= 0) 
    {
        tm = Mat4::identity;
        return false;
    }
    const ModelInstance& inst = m_ModelInstances[instID];
    if (boneID < 0)
    {
        tm = inst.m_RootTM;
        return false;
    }
    const JBoneInstance* pBones = &inst.m_Bones[0];
    tm = pBones[boneID].m_ModelTM*inst.m_RootTM;
    return true;
} // JModelServer::GetBoneTM

void JModelServer::DrawShadow( int instID, const Mat4& tm )
{
    if (instID < 0) return;
    m_ShadowObj.push_back( ShadowObj( instID, tm ) );
} // JModelServer::DrawShadow

void JModelServer::SetShadowMapSide( int side )
{
    if (m_ShadowMapID != -1)
    {
        //g_pRenderServer->DeleteTexture( m_ShadowMapID );
    }
    m_ShadowMapID = -1;
    m_ShadowMapSide = side;
}

void JModelServer::DrawShadows()
{
    if (!m_bEnableShadows)
    {
        m_ShadowObj.clear();
        return;
    }
    if (m_ShadowObj.size() == 0)
    {
        return;
    }
    if (m_ShadowMapID == -1)
    {
        m_ShadowMapID = g_pRenderServer->GetTextureID( "BackBufferCopy" );
        if (m_ShadowMapID == -1)
        {
            m_ShadowMapID = g_pRenderServer->CreateRenderTarget( "shadowMap", m_ShadowMapSide, m_ShadowMapSide );
        }
    }

    Mat4 viewTM = g_pRenderServer->GetViewTM();
    Mat4 projTM = g_pRenderServer->GetProjTM();
    Mat4 camTM;
    camTM.inverse( viewTM );
    Frustum frustum( viewTM*projTM );
    Vec3 camPos = camTM.translation();
    frustum.GetTop( camPos );
    
    //  caclulate trapezoid
    Vec3 dirLB = frustum.lbn() - camPos;
    Vec3 dirLT = frustum.ltn() - camPos;
    Vec3 dirRB = frustum.rbn() - camPos;
    Vec3 dirRT = frustum.rtn() - camPos;

    dirLB.normalize();
    dirLT.normalize();
    dirRB.normalize();
    dirRT.normalize();

    Vec3 ptLB, ptRB, ptLT, ptRT;
    bool bCrossLB = Plane::xOy.IntersectRay( camPos, dirLB, ptLB );
    bool bCrossRB = Plane::xOy.IntersectRay( camPos, dirRB, ptRB );

    if (!bCrossLB || !bCrossRB)
    {
        m_ShadowObj.clear();
        return;
    }

    if (!Plane::xOy.IntersectRay( camPos, dirLT, ptLT ) || 
        camPos.dist( ptLT ) > m_ShadowRange)
    {
        Vec3 leftDir = ptLB - Vec3( camPos.x, camPos.y, 0.0f );
        leftDir.normalize();
        ptLT = ptLB + m_ShadowRange*leftDir;
    }
    if (!Plane::xOy.IntersectRay( camPos, dirRT, ptRT ) || 
        camPos.dist( ptRT ) > m_ShadowRange)
    {
        Vec3 rightDir = ptRB - Vec3( camPos.x, camPos.y, 0.0f );
        rightDir.normalize();
        ptRT = ptRB + m_ShadowRange*rightDir;
    }

    //  create light camera
    Vec3 center = 0.25f*(ptLT + ptRT + ptLB + ptRB);
    Vec3 lcUp  = Vec3::oZ;
    Vec3 lcDir = m_LightDir;
    Vec3 lcRight;
    lcRight.cross( lcUp, lcDir );
    lcRight.normalize();
    lcUp.cross( lcDir, lcRight );
    lcUp.normalize();

    const float c_SunDist = 5000.0f;
    center -= lcDir*c_SunDist;

    Mat4 lcTM( lcRight, lcUp, lcDir, center );
    Mat4 lcViewTM;
    lcViewTM.inverse( lcTM );
    
    Vec3 lt( ptLT ), lb( ptLB ), rt( ptRT ), rb( ptRB );
    lcViewTM.tmpt( lt );
    lcViewTM.tmpt( lb );
    lcViewTM.tmpt( rt );
    lcViewTM.tmpt( rb );
    float minX = tmin( lt.x, rt.x, rb.x, lb.x );
    float maxX = tmax( lt.x, rt.x, rb.x, lb.x );
    float minY = tmin( lt.y, rt.y, rb.y, lb.y );
    float maxY = tmax( lt.y, rt.y, rb.y, lb.y );

    Frame projVP( minX, minY, maxX - minX, maxY - minY );

    Mat4 lcProjTM;
    OrthoProjTM( lcProjTM, projVP, 0.0f, c_SunDist*4.0f );

    lcProjTM.tmpt( lt );
    lcProjTM.tmpt( lb );
    lcProjTM.tmpt( rt );
    lcProjTM.tmpt( rb );

    //  build warping transform, to make lumels distribution in screen space more even 
    Mat3 warpTM = UnitSquareToQuadTM( Vec2( rb.x, rb.y ),
                                      Vec2( lb.x, lb.y ), 
                                      Vec2( lt.x, lt.y ), 
                                      Vec2( rt.x, rt.y ) );
    const Mat3 c_ProjToUnitSq( 0.5f, 0.0f,  0.0f,
                               0.0f, -0.5f, 0.0f,
                               0.5f, 0.5f,  1.0f );
    warpTM = c_ProjToUnitSq*warpTM;
    warpTM.invert();
    Mat4 lcWarpTM( warpTM.e00, warpTM.e01, 0.0f, warpTM.e02,
                   warpTM.e10, warpTM.e11, 0.0f, warpTM.e12,
                   0.0f,       0.0f,       1.0f, 0.0f,
                   warpTM.e20, warpTM.e21, 0.0f, warpTM.e22 );
    lcProjTM *= lcWarpTM;
    lcProjTM *= 1.0f/lcProjTM.e33;

    Vec4 checkPt( 0.0f, 0.0f, 0.0f, 1.0f );
    lcViewTM.tm( checkPt );
    lcProjTM.tm( checkPt );
    checkPt /= checkPt.w;
    
    //  render shadow casters to the shadow map
    int oldRT = g_pRenderServer->GetRenderTarget();
    Frame oldVP = g_pRenderServer->GetViewport();
    g_pRenderServer->SetRenderTarget( m_ShadowMapID );
    TextureProperties prop;
    g_pRenderServer->GetTextureProp( m_ShadowMapID, prop );
    int vpW = prop.m_Width;
    int vpH = prop.m_Height;
    g_pRenderServer->SetViewport( Frame( 0.0f, 0.0f, vpW, vpH ) );
    g_pRenderServer->ClearViewport( 0xFFFFFFFF );

    int casterShaderID = g_pRenderServer->GetShaderID( "shadow" );
    g_pRenderServer->SetShader( casterShaderID );
    g_pRenderServer->SetColorTint( m_ShadowColor );

    g_pRenderServer->SetViewTM( lcViewTM );
    g_pRenderServer->SetProjTM( lcProjTM );
    
    int nCasters = m_ShadowObj.size();
    for (int i = 0; i < nCasters; i++)
    {
        if (m_ShadowObj[i].m_InstanceID == -1) continue;
        ModelInstance& inst = m_ModelInstances[m_ShadowObj[i].m_InstanceID];
        if (!inst.m_pModel || inst.m_Bones.size() == 0) continue;
        inst.m_pModel->Render( m_ShadowObj[i].m_TM, &inst.m_Bones[0], true, true );
    }
    
    //  render quad with shadow texture applied
    g_pRenderServer->SetRenderTarget( oldRT );
    g_pRenderServer->SetViewport( oldVP );

    int shadowShaderID = g_pRenderServer->GetShaderID( "shadow_plane" );
    g_pRenderServer->SetShader( shadowShaderID );
    g_pRenderServer->SetTexture( m_ShadowMapID, 0 );
    g_pRenderServer->SetWorldTM( Mat4::identity );
    g_pRenderServer->SetViewTM( viewTM );
    g_pRenderServer->SetProjTM( projTM );

    //g_pRenderServer->SaveTexture( m_ShadowMapID, "c:\\shmap.dds" );

    //  calculate shadow texture transform
    const Mat4 c_ProjToUV( 0.5f,   0.0f,   0.0f, 0.0f,
                           0.0f,   -0.5f,  0.0f, 0.0f,
                           0.0f,   0.0f,   1.0f, 0.0f,
                           0.5f,   0.5f,   0.0f, 1.0f );
    Mat4 texTM = camTM;
    texTM *= lcViewTM;
    texTM *= lcProjTM;
    texTM *= c_ProjToUV;
    g_pRenderServer->SetTextureTM( texTM );

    
    VertexW quadV[4];
    quadV[0].x = ptLT.x;
    quadV[0].y = ptLT.y;
    quadV[0].z = ptLT.z;
    quadV[0].u = 0.0f;  
    quadV[0].v = 0.0f;  

    quadV[1].x = ptRT.x;
    quadV[1].y = ptRT.y;
    quadV[1].z = ptRT.z;
    quadV[1].u = 1.0f;  
    quadV[1].v = 0.0f;  

    quadV[2].x = ptLB.x;
    quadV[2].y = ptLB.y;
    quadV[2].z = ptLB.z;
    quadV[2].u = 0.0f;
    quadV[2].v = 1.0f;

    quadV[3].x = ptRB.x;
    quadV[3].y = ptRB.y;
    quadV[3].z = ptRB.z;
    quadV[3].u = 1.0f;
    quadV[3].v = 1.0f;

    g_pDrawServer->DrawPrimList( quadV, 4, PrimitiveType_QuadList );
    m_ShadowObj.clear();
    g_pRenderServer->SetTextureTM( Mat4::identity );

    if (m_bShowShadowMap)
    {
        int spShaderID = g_pRenderServer->GetShaderID( "base_sprite" );
        g_pRenderServer->SetShader( spShaderID );
        g_pRenderServer->SetTexture( m_ShadowMapID, 0 );
        const int c_ShowSMSide = 256.0f;

        VertexS v[4];
        v[0].x = 0.0f;
        v[0].y = 0.0f;
        v[0].z = 0.0f;
        v[0].w = 1.0f;
        v[0].u = 0.0f;  
        v[0].v = 0.0f;  
        v[0].diffuse = 0xFFFFFFFF;

        v[1].x = c_ShowSMSide;
        v[1].y = 0.0f;
        v[1].z = 0.0f;
        v[1].w = 1.0f;
        v[1].u = 1.0f;  
        v[1].v = 0.0f;  
        v[1].diffuse = 0xFFFFFFFF;

        v[2].x = 0.0f;
        v[2].y = c_ShowSMSide;
        v[2].z = 0.0f;
        v[2].w = 1.0f;
        v[2].u = 0.0f;
        v[2].v = 1.0f;
        v[2].diffuse = 0xFFFFFFFF;

        v[3].x = c_ShowSMSide;
        v[3].y = c_ShowSMSide;
        v[3].z = 0.0f;
        v[3].w = 1.0f;
        v[3].u = 1.0f;
        v[3].v = 1.0f;
        v[3].diffuse = 0xFFFFFFFF;

        g_pDrawServer->DrawPrimList( v, 4, PrimitiveType_QuadList );
    }
} // JModelServer::DrawShadows







