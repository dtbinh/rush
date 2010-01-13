//****************************************************************************/
//  File:  JModelServer.h
//  Desc:  
//****************************************************************************/
#ifndef __JMODELSERVER_H__
#define __JMODELSERVER_H__
#include <vector>
#include "Color.h"
#include "Mat4.h"
#include "Path.h"
#include "IModelServer.h"

class JModel;
class JBoneInstance;
//****************************************************************************/
//  Class:  ModelInstance
//  Desc:   Concrete instance of rendered model
//****************************************************************************/
struct ModelInstance
{
    JModel*                     m_pModel;
    std::vector<JBoneInstance>  m_Bones;
    Mat4                        m_RootTM;
}; // struct ModelInstance

struct ShadowObj
{
    int     m_InstanceID;
    Mat4    m_TM;
    ShadowObj( int instID = -1, const Mat4& tm = Mat4::identity ) 
        : m_InstanceID( instID ), m_TM( tm ){}
}; // struct ShadowObj

//****************************************************************************/
//  Class:  JModelServer
//  Desc:   
//****************************************************************************/
class JModelServer : public JObject, public IModelServer
{
    std::vector<JModel*>        m_Models;
    std::vector<JModelAnim*>    m_Anims;
    std::vector<ModelInstance>  m_ModelInstances;

    int                         m_ModelVBID;
    int                         m_ModelIBID;

    int                         m_ShadowMapID;
    std::vector<ShadowObj>      m_ShadowObj;
    bool                        m_bShowShadowMap;
    Vec3                        m_LightDir;
    uint32_t                       m_ShadowColor;
    float                       m_ShadowRange;
    int                         m_ShadowMapSide;
    bool                        m_bEnableShadows;


public:
                JModelServer      ();
    virtual     ~JModelServer     ();
    virtual void Init             ();

    int         GetModelID      ( const char* name, JModel* pModel = NULL );
    JModel*     GetModel        ( int mID );
    void        DrawModel       ( int mID, const Mat4& tm, int instID = -1 );
    AABox       GetModelAABB    ( int mID );

    int         GetAnimID       ( const char* name, JModelAnim* pAnim = NULL );
    JModelAnim* GetAnim         ( int animID );
    float       GetAnimDuration ( int animID, int sectionID = -1 ) const;
    int         GetAnimSectionID( int animID, const char* name ) const;

    void        AnimateModel    ( int mID, const Mat4& tm, int anmID, float cTime, int instID = -1, 
        float weight = 1.0f, int sectionID = -1 );
    int         InstanceModel   ( int mID );
    int         GetModelBoneID  ( int mID, const char* boneName ) const;
    bool        SetBoneTM       ( int instID, int boneID, Mat4& tm, bool bLocal = false );
    bool        GetBoneTM       ( int instID, int boneID, Mat4& tm ) const;
    JModel*     GetModelByInst  ( int instID );
    
    void        EnableShadows   ( bool bEnable = true ) { m_bEnableShadows = bEnable; }
    bool        ShadowsEnabled  () const { return m_bEnableShadows; }

    void        DrawShadow      ( int instID, const Mat4& tm );
    void        DrawShadows     ();

    void        SetLightDir     ( const Vec3& dir ) { m_LightDir = dir; }
    void        SetShadowColor  ( uint32_t color ) { m_ShadowColor = color; }

    int         GetShadowMapSide() const { return m_ShadowMapSide; }
    void        SetShadowMapSide( int side );
    
    expose(JModelServer)
    {
        parent(JObject);

        field( "ShadowRange",   m_ShadowRange );
        field( "ShadowColor",   m_ShadowColor );
        field( "EnableShadows", m_bEnableShadows );
        prop ( "ShadowMapSide", GetShadowMapSide, SetShadowMapSide );
    }

private:
}; // class JModelServer

#endif // __JMODELSERVER_H__