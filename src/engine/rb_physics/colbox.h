/***********************************************************************************/
//  File:   ColBox.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLBOX_H__
#define __COLBOX_H__

#include "ColGeom.h"

/***********************************************************************************/
//  Class:  ColBox
//  Desc:   
/***********************************************************************************/
class ColBox : public ColGeom
{
    Vec3                m_Extents;

public:
                        ColBox      ();
    virtual void        DrawBounds  ();
    virtual void        Synchronize ( bool bFromSolver );

    virtual dGeomID     CreateGeom  ( dSpaceID spaceID );
    void                SetExtents  ( const Vec3& ext ) { m_Extents = ext; }
    const Vec3&         GetExtents  () const { return m_Extents; }
    virtual dMass       GetMass     () const;

    expose( ColBox )
    {
        parent( ColGeom );
        field( "Extents", m_Extents );
    }
}; // class ColBox

#endif //__COLBOX_H__