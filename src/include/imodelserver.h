/*****************************************************************************/
/*	File:  IModelServer.h
/*****************************************************************************/
#ifndef __IMODELSERVER_H__
#define __IMODELSERVER_H__

class Vec3;
class Mat4;
class AABox;
class JModel;
class JModelAnim;

/*****************************************************************************/
/*	Class:  IModelServer
/*  Desc:
/*****************************************************************************/
class IModelServer
{
public:
    virtual int         GetModelID      ( const char* name, JModel* pModel = NULL ) = 0;
    virtual JModel*     GetModel        ( int mID ) = 0;
    virtual void        DrawModel       ( int mID, const Mat4& tm, int instID = -1 ) = 0;
    virtual AABox       GetModelAABB    ( int mID ) = 0;
    virtual int         GetModelBoneID  ( int mID, const char* boneName ) const = 0;
    virtual bool        GetBoneTM       ( int instID, int boneID, Mat4& tm ) const = 0;
    virtual bool        SetBoneTM       ( int instID, int boneID, Mat4& tm, bool bLocal = false ) = 0;

    virtual void        DrawShadow      ( int instID, const Mat4& tm ) = 0;
    virtual void        DrawShadows     () = 0;
    virtual void        SetLightDir     ( const Vec3& dir ) = 0;
    virtual void        SetShadowColor  ( uint32_t color ) = 0;

    virtual void        EnableShadows   ( bool bEnable = true ) = 0;
    virtual bool        ShadowsEnabled  () const = 0;

    virtual int         GetAnimID       ( const char* name, JModelAnim* pAnim = NULL ) = 0;
    virtual JModelAnim* GetAnim         ( int animID ) = 0;
    virtual float       GetAnimDuration ( int animID, int sectionID = -1 ) const = 0;
    virtual int         GetAnimSectionID( int animID, const char* name ) const = 0;

    virtual void        AnimateModel    ( int mID, const Mat4& tm, int anmID, float cTime,
                                          int instID = -1, float weight = 1.0f, int sectionID = -1 ) = 0;
    virtual int         InstanceModel   ( int mID ) = 0;
    virtual JModel*     GetModelByInst  ( int instID ) = 0;

}; // class IModelServer

extern IModelServer* g_pModelServer;

#endif // __IMODELSERVER_H__


