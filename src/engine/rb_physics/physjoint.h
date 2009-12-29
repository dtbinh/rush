/***********************************************************************************/
//  File:   PhysJoint.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JOINT_H__
#define __JOINT_H__
#include "PhysEntity.h"

/***********************************************************************************/
//  Class:  PhysJoint
//  Desc:   
/***********************************************************************************/
class PhysJoint : public PhysEntity
{
    JString         m_HostBone;
    JString         m_Body1;
    JString         m_Body2;

public:
    PhysJoint           ();
    ~PhysJoint          ();

    const char*     GetHostBone     () const { return m_HostBone; }
    void            SetHostBone     ( const char* name ) { m_HostBone = name; }

    expose(PhysJoint)
    {
        parent(PhysEntity);
        field( "HostBone",   m_HostBone  );
        field( "Body1",      m_Body1     );
        field( "Body2",      m_Body2     );
    }
}; // class PhysJoint

#endif //__JOINT_H__