/***********************************************************************************/
//  File:   ColPlane.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLPLANE_H__
#define __COLPLANE_H__

#include "ColGeom.h"

/***********************************************************************************/
//  Class:  ColPlane
//  Desc:   
/***********************************************************************************/
class ColPlane : public ColGeom
{
    Plane               m_Plane;
public:
                        ColPlane    ();
    virtual dGeomID     CreateGeom  ( dSpaceID spaceID );
    virtual bool        IsPlaceable () const { return false; }

    expose( ColPlane )
    {
        parent( ColGeom );
        field( "Plane", m_Plane );
    }
}; // class ColPlane

#endif //__COLPLANE_H__