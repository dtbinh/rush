/***********************************************************************************/
//  File:   JLocator.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JLOCATOR_H__
#define __JLOCATOR_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JLocator
//  Desc:   
/***********************************************************************************/
class JLocator : public JWidget
{
    Vec3                m_Pos;
    bool                m_bScreenSpace;
    float               m_Rotation;
    Vec3                m_Dir;

public:
                        JLocator        ();
    virtual void        DrawBounds      ();
    virtual bool        PtIn            ( int px, int py ) const;
    virtual void        OnDrag          ( JDragEvent& e );

    bool                IsScreenSpace   () const { return m_bScreenSpace; }
    void                SetScreenSpace  ( bool bSet = true );

    Vec3                GetPos          () const { return m_Pos; }
    void                SetPos          ( const Vec3& pos ) { m_Pos = pos; }

    expose(JLocator)
    {
        parent(JWidget);
        prop ( "ScreenSpace",   IsScreenSpace, SetScreenSpace );
        field( "Position",      m_Pos          );
        field( "Rotation",      m_Rotation     );
        field( "Dir",           m_Dir          );

        field( "PosX:d",        m_Pos.x        );
        field( "PosY:d",        m_Pos.y        );
        field( "PosZ:D",        m_Pos.z        );

    }
}; // class JLocator

#endif //__JLOCATOR_H__