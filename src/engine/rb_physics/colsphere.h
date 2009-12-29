/***********************************************************************************/
//  File:   ColSphere.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLSPHERE_H__
#define __COLSPHERE_H__

#include "ColGeom.h"

/***********************************************************************************/
//  Class:  ColSphere
//  Desc:   
/***********************************************************************************/
class ColSphere : public ColGeom
{
    float               m_Radius;
public:
                        ColSphere   ();
    virtual void        DrawBounds  ();  
    virtual dGeomID     CreateGeom  ( dSpaceID spaceID );
    void                SetRadius   ( float radius ) { m_Radius = radius; }
    float               GetRadius   () const { return m_Radius; }
    virtual void        Synchronize ( bool bFromSolver );
    virtual dMass       GetMass     () const;

    expose( ColSphere )
    {
        parent( ColGeom );
        field( "Radius", m_Radius );
    }
}; // class ColSphere

#endif //__COLSPHERE_H__