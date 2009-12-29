/***********************************************************************************/
//  File:   JAnmPhysics.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANMPHYSICS_H__
#define __JANMPHYSICS_H__

#include "JAnimation.h"
#include "JWeakRef.h"

class JModelInstance;
/***********************************************************************************/
//  Class:  JAnmPhysics
//  Desc:   
/***********************************************************************************/
class JAnmPhysics : public JAnimation
{
    JString                     m_ModelInstance;
    int                         m_PhysObjInstID;
    JWeakRef<JModelInstance>    m_pModelInst;

public:
                        JAnmPhysics        ();
                        ~JAnmPhysics       ();

    virtual void        Render             ();
    virtual void        Init               ();
    
    virtual void        OnPlay             ();
    virtual void        OnStop             ();
    virtual void        DrawBounds         ();

    int                 GetPhysObjInstID   () const { return m_PhysObjInstID; }

    expose(JAnmPhysics)
    {
        parent(JAnimation);
        field( "ModelInstance", m_ModelInstance );
        rprop( "PhysObjInstID", GetPhysObjInstID );
    }
}; // class JAnmPhysics

#endif //__JANMPHYSICS_H__