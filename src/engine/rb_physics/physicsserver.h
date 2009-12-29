/***********************************************************************************/
//  File:   PhysicsServer.h
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __PHYSICSSERVER_H__
#define __PHYSICSSERVER_H__

#include "ode/ode.h"
#include "JReflect.h"
#include "IPhysicsServer.h"
#include "Timer.h"

enum SolverMode
{
    smUnknown   = 0,
    smNormal    = 1,
    smQuickStep = 2,
    smStepFast  = 3
}; // enum SolverMode

enum_beg(SolverMode)
enum_val( "Unknown",   smUnknown    ),
enum_val( "Normal",    smNormal     ),
enum_val( "QuickStep", smQuickStep  ),
enum_val( "StepFast",  smStepFast   )
enum_end(SolverMode)

class ColGeom;
class PhysMaterial;
class PhysObject;

const int c_MaxContacts = 256;
/***********************************************************************************/
//  Class:  PhysicsServer
//  Desc:   
/***********************************************************************************/
class PhysicsServer : public JObject, public IPhysicsServer
{
    dWorldID                            m_WorldID;
    dSpaceID                            m_DefaultSpaceID;
    dJointGroupID                       m_ContactGroupID;
    PhysMaterial*                       m_pDefMaterial;

    float                               m_ERP;
    float                               m_CFM;
    Vec3                                m_Gravity;
    float                               m_WorldScale;
    SolverMode                          m_Mode;
    int                                 m_MaxIter;
    float                               m_StepSize;

    float                               m_TimeRepr;

    dContact                            m_Contacts[c_MaxContacts];
    int                                 m_NContacts;
    int                                 m_NStepsPerFrame;

    bool                                m_bDrawVolumes;
    bool                                m_bInited;

    DWORD                               m_GeomColor;    
    DWORD                               m_GeomLinesColor;


    std::vector<PhysObject*>            m_Objects;
public:
                    PhysicsServer       ();
                    ~PhysicsServer      ();
    virtual void    Render              ();
    virtual void    Init                ();

    void            SetCFM              ( float val );
    void            SetERP              ( float val );
    void            SetGravity          ( Vec3 g );
    void            SetWorldScale       ( float scale ) { m_WorldScale = scale; } 

    bool            IsInited            () const { return m_bInited; }
    float           GetCFM              () const { return m_CFM; }
    float           GetERP              () const { return m_ERP; } 
    Vec3            GetGravity          () const { return m_Gravity; }
    float           GetWorldScale       () const { return m_WorldScale; }

    dWorldID        GetWorldID          () const { return m_WorldID; }
    dSpaceID        GetSpaceID          ( const ColGeom* pGeom ) const { return m_DefaultSpaceID; }


    void            StepSolver          ( float dt );
    int             GetObjectID         ( const char* name, JObject* context = NULL );
    int             GetObjectID         ( int modelInstID );
    int             GetPhysMaterialID    ( const char* name );

    int             GetStepsPerFrame    () const { return m_NStepsPerFrame; }

    const Mat4&     GetObjectTM         ( int objID );
    void            SetObjectTM         ( int objID, const Mat4& tm );

    Vec3            GetObjectPos        ( int objID );
    void            SetObjectPos        ( int objID, const Vec3& pos );

    Vec3            GetObjectVel        ( int objID );
    void            SetObjectVel        ( int objID, const Vec3& v );

    Vec3            GetObjectForce      ( int objID );
    void            SetObjectForce      ( int objID, const Vec3& v );

    Vec3            GetObjectTorque     ( int objID );
    void            SetObjectTorque     ( int objID, const Vec3& v );

    void            EnableObject        ( int objID, bool bEnable = true );

    float           GetTimeStep         () const { return m_StepSize; }

    Vec3            GetObjectAngVel     ( int objID );
    void            SetObjectAngVel     ( int objID, const Vec3& v );
    PhysMaterial*   GetGeomSurface      ( dGeomID geomID );

    float           GetObjectMass       ( int objID );
    void            SetObjectMass       ( int objID, float mass );

    Mat3            GetObjectInertia    ( int objID );
    void            SetObjectInertia    ( int objID, const Mat3& inertiaTensor );

    void            DrawBounds          ( int objID );

    void            UnregisterObject    ( PhysObject* pObject );

    int             GetNBodies          () const;
    int             GetNGeoms           () const;

    DWORD           GetGeomColor        () const { return m_GeomColor; }
    DWORD           GetGeomLinesColor   () const { return m_GeomLinesColor; }

    PhysMaterial*   GetDefaultMaterial  () const { return m_pDefMaterial; }

    static PhysicsServer* s_pInstance;

    expose( PhysicsServer )
    {
        parent  ( JObject );
        prop    ( "ERP",                    GetERP, SetERP      );
        prop    ( "CFM",                    GetCFM, SetCFM      );
        prop    ( "Gravity",                GetGravity, SetGravity );
        field   ( "WorldScale",             m_WorldScale        );
        field   ( "Mode",                   m_Mode              );
        field   ( "MaxIter",                m_MaxIter           );
        field   ( "StepSize",               m_StepSize          );
        field   ( "DrawVolumes",            m_bDrawVolumes      );
        field   ( "Contacts",               m_NContacts         );
        rprop   ( "Bodies",                 GetNBodies          );
        rprop   ( "Geoms",                  GetNGeoms           );
        field   ( "GeomColor",              m_GeomColor         );
        field   ( "GeomLinesColor",         m_GeomLinesColor    );

        rprop   ( "StepsPerFrame",          GetStepsPerFrame    );
    }

private:

    friend void OnCollide( void *data, dGeomID o1, dGeomID o2 );
    void        ProcessCollision( void *data, dGeomID o1, dGeomID o2 );
}; // class PhysicsServer

#endif //__PHYSICSSERVER_H__