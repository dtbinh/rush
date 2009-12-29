/***********************************************************************************/
//  File:   ColCapsule.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLCAPSULE_H__
#define __COLCAPSULE_H__

#include "ColGeom.h"

/***********************************************************************************/
//  Class:  ColCapsule
//  Desc:   
/***********************************************************************************/
class ColCapsule : public ColGeom
{
    float               m_Height;
    float               m_Radius;

public:
                        ColCapsule  ();
    virtual void        DrawBounds  ();
    virtual void        Synchronize ( bool bFromSolver );

    virtual dGeomID     CreateGeom  ( dSpaceID spaceID );

    void                SetRadius   ( float radius ) { m_Radius = radius; }
    float               GetRadius   () const { return m_Radius; }

    void                SetHeight   ( float height ) { m_Height = height; }
    float               GetHeight   () const { return m_Height; }
    virtual dMass       GetMass     () const;

    expose( ColCapsule )
    {
        parent( ColGeom );
        field( "Height", m_Height );
        field( "Radius", m_Radius );
    }

}; // class ColCapsule

#endif //__COLCAPSULE_H__