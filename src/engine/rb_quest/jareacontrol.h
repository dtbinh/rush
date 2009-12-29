/***********************************************************************************/
//  File:   JAreaControl.h
//  Date:   29.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JAREACONTROL_H__
#define __JAREACONTROL_H__

#include "JCharControl.h"

/***********************************************************************************/
//  Class:  JAreaControl
//  Desc:   Used to control character movement inside the location
/***********************************************************************************/
class JAreaControl : public JCharControl
{
    PolyLine2           m_Area; // area inside which character can move

public:
                        JAreaControl    ();
    virtual void        OnMouse         ( JMouseEvent& e );
    virtual void        Render          ();
    virtual void        DrawBounds      ();
    virtual bool        MoveTo          ( const Vec3& pos, bool bImmediately, float direction = -1.0f );
    virtual bool        PtIn            ( int px, int py ) const { return m_Area.PtIn( Vec2( px, py ) ); }
    const PolyLine2&    GetArea         () const { return m_Area; }

    expose( JAreaControl )
    {
        parent( JCharControl );
        field( "Contour", m_Area );
    }
}; // class JAreaControl

#endif //__JAREACONTROL_H__