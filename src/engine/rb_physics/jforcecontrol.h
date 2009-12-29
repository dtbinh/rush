/***********************************************************************************/
//  File:   JForceControl.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JFORCECONTROL_H__
#define __JFORCECONTROL_H__

#include "JAnimation.h"
#include "JWeakRef.h"

class JModelInstance;
/***********************************************************************************/
//  Class:  JForceControl
//  Desc:   
/***********************************************************************************/
class JForceControl : public JAnimation
{
    JString                     m_ModelInstance;
    JString                     m_Body;
    float                       m_Magnitude;
    float                       m_JumpImpulse;

    JWeakRef<JModelInstance>    m_pModelInst;
    int                         m_PhysObjInstID;

public:
                                JForceControl      ();
                                ~JForceControl     ();

    virtual void                Render             ();
    virtual void                OnPlay             ();
    virtual void                OnStop             ();
    virtual void                Init               ();
    virtual void                DrawBounds         ();

    expose(JForceControl)
    {
        parent(JAnimation);
        field( "ModelInstance", m_ModelInstance );
        field( "Body",          m_Body          );
        field( "Magnitude",     m_Magnitude     );
        field( "JumpImpulse",   m_JumpImpulse   );
    }
}; // class JForceControl

#endif //__JFORCECONTROL_H__