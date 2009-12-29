/***********************************************************************************/
//  File:   ColCylinder.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLCYLINDER_H__
#define __COLCYLINDER_H__

#include "ColGeom.h"

/***********************************************************************************/
//  Class:  ColCylinder
//  Desc:   
/***********************************************************************************/
class ColCylinder : public ColGeom
{
    float               m_Height;
    float               m_Radius;

public:
                        ColCylinder ();
    virtual void        DrawBounds  ();
    virtual dGeomID     CreateGeom  ( dSpaceID spaceID );
    virtual void        Synchronize ( bool bFromSolver );

    void                SetRadius   ( float radius ) { m_Radius = radius; }
    float               GetRadius   () const { return m_Radius; }

    void                SetHeight   ( float height ) { m_Height = height; }
    float               GetHeight   () const { return m_Height; }
    virtual dMass       GetMass     () const; 

    expose( ColCylinder )
    {
        parent( ColGeom );
        field( "Height", m_Height );
        field( "Radius", m_Radius );
    }
}; // class ColCylinder

#endif //__COLCYLINDER_H__