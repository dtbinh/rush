/***********************************************************************************/
//  File:   PhysBody.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __PHYSBODY_H__
#define __PHYSBODY_H__

#include "ode/ode.h"
#include "PhysEntity.h"

/***********************************************************************************/
//  Class:  PhysBody
//  Desc:   
/***********************************************************************************/
class PhysBody : public PhysEntity
{
    float			    m_Mass;		
    Mat3                m_Inertia;

    Vec3	            m_Velocity;		
    Vec3	            m_AngVelocity;	
    Mat4                m_TM;

    bool                m_bAutoDisable;

    Mat4                m_OffsetTM; // offset transform from the host bone

    dBodyID             m_ID;

    Vec3                m_Force;
    Vec3                m_Torque;

    JString             m_HostBone;

    int                 m_BoneID;
    Vec3                m_BodyScale;

    dMass               m_AccumulatedMass;
    int                 m_NMasses;

public:
                        PhysBody        ();
                        ~PhysBody       ();

    virtual void        Init            ();
    virtual void        DrawBounds      ();
    virtual void        Synchronize     ( bool bFromSolver );

    dBodyID             GetBodyID       () const;

    Vec3                GetPos          () const;
    void                SetPos          ( Vec3 pos ); 

    Vec3                GetVelocity     () const;
    void                SetVelocity     ( Vec3 v );

    Vec3                GetForce        () const;
    void                SetForce        ( Vec3 v );

    Vec3                GetTorque       () const;
    void                SetTorque       ( Vec3 v );

    Vec3                GetAngVelocity  () const;
    void                SetAngVelocity  ( Vec3 v ); 

    float               GetMass         () const;
    void                SetMass         ( float m );

    Mat3                GetInertia      () const;
    void                SetInertia      ( Mat3 inertiaTensor );

    const Mat4&         GetTM           () const;
    void                SetTM           ( const Mat4& tm );

    const Mat4&         GetOffsetTM     () const;
    void                SetOffsetTM     ( const Mat4& tm );

    const char*         GetHostBone     () const;
    void                SetHostBone     ( const char* name );

    float               GetBodyScale    () const;

    void                AccumulateMass  ( const dMass& mass );
    virtual void        BindToSolver    ();
    
    expose(PhysBody)
    {
        parent(PhysEntity);
        prop ( "Velocity",      GetVelocity,    SetVelocity     );
        prop ( "AngVelocity",   GetAngVelocity, SetAngVelocity  );
        prop ( "Mass",          GetMass,        SetMass         );
        prop ( "Inertia",       GetInertia,     SetInertia      );
        prop ( "Position",           GetPos,         SetPos          );
        prop ( "Force",         GetForce,       SetForce        );
        prop ( "Torque",        GetTorque,      SetTorque       );
        prop ( "HostBone",      GetHostBone,    SetHostBone     );
        field( "OffsetTM",      m_OffsetTM      );
        field( "AutoDisable",   m_bAutoDisable  );
    }
}; // class PhysBody

/***********************************************************************************/
/*  PhysBody inlines
/***********************************************************************************/
inline dBodyID PhysBody::GetBodyID() const 
{ 
    return m_ID; 
}

inline Vec3 PhysBody::GetPos() const
{
    return m_TM.translation();
}

inline void PhysBody::SetPos( Vec3 pos )
{
    m_TM.translation() = pos;
}

inline Vec3 PhysBody::GetVelocity() const
{
    return m_Velocity;
}

inline void PhysBody::SetVelocity( Vec3 v )
{
    m_Velocity = v;
}

inline Vec3 PhysBody::GetForce() const
{
    return m_Force;
}

inline void PhysBody::SetForce( Vec3 v )
{
    m_Force = v;
}

inline Vec3 PhysBody::GetTorque() const
{
    return m_Torque; 
}

inline void PhysBody::SetTorque( Vec3 v )
{
    m_Torque = v;
}

inline Vec3 PhysBody::GetAngVelocity() const
{
    return m_AngVelocity;
}

inline void PhysBody::SetAngVelocity( Vec3 v )
{
    m_AngVelocity = v;
}

inline float PhysBody::GetMass() const
{
    return m_Mass;
}

inline void PhysBody::SetMass( float m )
{
    m_Mass = m;
}

inline Mat3 PhysBody::GetInertia() const
{
    return m_Inertia;
}

inline void PhysBody::SetInertia( Mat3 inertiaTensor )
{
    m_Inertia = inertiaTensor;
}

inline const Mat4& PhysBody::GetTM() const
{
    return m_TM;
}

inline void PhysBody::SetTM( const Mat4& tm )
{
    m_TM = tm;
}

inline const Mat4& PhysBody::GetOffsetTM() const
{
    return m_OffsetTM;
}

inline void PhysBody::SetOffsetTM( const Mat4& tm )
{
    m_OffsetTM = tm;
}

inline const char* PhysBody::GetHostBone() const 
{ 
    return m_HostBone; 
}

inline void PhysBody::SetHostBone( const char* name ) 
{ 
    m_HostBone = name; 
}

inline float PhysBody::GetBodyScale() const
{
    return m_BodyScale.x;
}

inline void PhysBody::AccumulateMass( const dMass& mass )
{
    if (m_NMasses == 0)
    {
        m_AccumulatedMass = mass;
    }
    else
    {
        dMassAdd( &m_AccumulatedMass, &mass ); 
    }
    dBodySetMass( m_ID, &m_AccumulatedMass );
    m_NMasses++;
} // PhysBody::AccumulateMass

#endif //__PHYSBODY_H__