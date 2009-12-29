/***********************************************************************************/
//  File:   PhysObject.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __PHYSOBJECT_H__
#define __PHYSOBJECT_H__

#include "PhysEntity.h"

class PhysBody;
/***********************************************************************************/
//  Class:  PhysObject
//  Desc:   
/***********************************************************************************/
class PhysObject : public PhysEntity
{
    std::string         m_RootBody;
    PhysBody*           m_pRootBody;

    int                 m_ID;
    int                 m_ModelInstID;
    bool                m_bEnabled; 

public:
    PhysObject  () :   
        m_pRootBody  ( NULL ), 
        m_ID         ( -1   ), 
        m_bEnabled   ( true ), 
        m_ModelInstID( -1   ) 
    {}   
                        ~PhysObject ();

    PhysBody*           GetRootBody ();

    void                SetID       ( int id ) { m_ID = id; }
    int                 GetID       () const { return m_ID; }
    void                Enable      ( bool bEnable = true );

    int                 GetModelInstID() const { return m_ModelInstID; }
    void                SetModelInstID( int id ) { m_ModelInstID = id; }

    virtual void        DrawBounds  ();

    expose( PhysObject )
    {
        parent( PhysEntity );
        field( "RootBody", m_RootBody );
    }

}; // class PhysObject

#endif //__PHYSICSOBJECT_H__