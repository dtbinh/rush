/***********************************************************************************/
//  File:   IPhysicsServer.h
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __IPHYSICS_H__
#define __IPHYSICS_H__

class JObject;
/***********************************************************************************/
//  Class:  IPhysicsServer
//  Desc:   
/***********************************************************************************/
class IPhysicsServer 
{
public:
    virtual void        Init                () = 0;
    virtual void        Render              () = 0;

    virtual float       GetTimeStep         () const = 0;
    virtual int         GetObjectID         ( const char* name, JObject* context = NULL ) = 0;
    virtual int         GetObjectID         ( int modelInstID ) = 0;

    
    virtual int         GetPhysMaterialID    ( const char* name ) = 0;

    virtual const Mat4& GetObjectTM         ( int objID ) = 0;
    virtual void        SetObjectTM         ( int objID, const Mat4& tm ) = 0;

    virtual Vec3        GetObjectPos        ( int objID ) = 0;
    virtual void        SetObjectPos        ( int objID, const Vec3& pos ) = 0;

    virtual Vec3        GetObjectVel        ( int objID ) = 0;
    virtual void        SetObjectVel        ( int objID, const Vec3& v ) = 0;

    virtual Vec3        GetObjectForce      ( int objID ) = 0;
    virtual void        SetObjectForce      ( int objID, const Vec3& v ) = 0;

    virtual Vec3        GetObjectTorque     ( int objID ) = 0;
    virtual void        SetObjectTorque     ( int objID, const Vec3& v ) = 0;

    virtual Vec3        GetObjectAngVel     ( int objID ) = 0;
    virtual void        SetObjectAngVel     ( int objID, const Vec3& v ) = 0;

    virtual float       GetObjectMass       ( int objID ) = 0;
    virtual void        SetObjectMass       ( int objID, float mass ) = 0;
    
    virtual Mat3        GetObjectInertia    ( int objID ) = 0;
    virtual void        SetObjectInertia    ( int objID, const Mat3& inertiaTensor ) = 0;

    virtual void        EnableObject        ( int objID, bool bEnable = true ) = 0;

    virtual void        DrawBounds          ( int objID ) = 0;
}; // class PhysicsServer

extern IPhysicsServer* g_pPhysicsServer;

#endif //__PHYSICS_H__