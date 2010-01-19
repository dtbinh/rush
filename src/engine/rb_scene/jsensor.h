/***********************************************************************************/
//  File:   JSensor.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSENSOR_H__
#define __JSENSOR_H__

#include "jwidget.h"

/***********************************************************************************/
//  Class:  JSensor
//  Desc:
/***********************************************************************************/
class JSensor : public JWidget
{
public:
                        JSensor             ();
    virtual void        DrawBounds          ();
    virtual bool        PtIn                ( int px, int py ) const;
    bool                IsScreenSpace       () const { return m_bScreenSpace; }
    void                SetScreenSpace      ( bool bSet = true );

    Vec3                GetPos              () const { return m_Position; }
    void                SetPos              ( const Vec3& pos ) { m_Position = pos; }

    const char*         GetAttachInstance   () const { return m_AttachInstance.c_str(); }
    const char*         GetAttachBone       () const { return m_AttachBone.c_str(); }

    void                SetAttachInstance   ( const char* name );
    void                SetAttachBone       ( const char* name );

    expose(JSensor)
    {
        parent(JWidget);
        prop ( "ScreenSpace",       IsScreenSpace, SetScreenSpace );
        field( "Position",          m_Position     );
        field( "Radius",            m_Radius       );

        field( "PosX",              m_Position.x   );
        field( "PosY",              m_Position.y   );
        field( "PosZ",              m_Position.z   );

        prop ( "AttachInstance",    GetAttachInstance, SetAttachInstance );
        prop ( "AttachBone",        GetAttachBone, SetAttachBone );
    }

private:
    Vec3                m_Position;
    float               m_Radius;
    bool                m_bScreenSpace;

    JString             m_AttachInstance;
    JString             m_AttachBone;
    int                 m_AttachInstanceID;
    int                 m_AttachBoneID;
}; // class JSensor

#endif //__JSENSOR_H__
