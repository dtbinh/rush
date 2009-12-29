/***********************************************************************************/
//  File:   JVec3Editor.h
//  Date:   23.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JVEC3EDITOR_H__
#define __JVEC3EDITOR_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JVec3Editor
//  Desc:   
/***********************************************************************************/
class JVec3Editor : public JWidget
{
    Vec3                m_Value;

public:
    JVec3Editor     () : m_Value( Vec3::null ){}
    float               GetX            () const { return m_Value.x; }
    float               GetY            () const { return m_Value.y; }
    float               GetZ            () const { return m_Value.z; }

    Vec3                GetValue        () const { return m_Value; }
    void                SetValue        ( Vec3 val ) 
    {
        m_Value = val;
        SendSignal( "x" );
        SendSignal( "y" );
        SendSignal( "z" );
    }
    void                SetX            ( float val ) { m_Value.x = val; SendSignal( "Value" ); }
    void                SetY            ( float val ) { m_Value.y = val; SendSignal( "Value" ); }
    void                SetZ            ( float val ) { m_Value.z = val; SendSignal( "Value" ); }

    expose(JVec3Editor)
    {
        parent(JWidget);
        prop ( "x", GetX, SetX );
        prop ( "y", GetY, SetY );
        prop ( "z", GetZ, SetZ );
        prop ( "Value", GetValue, SetValue );
    }
}; // class JVec3Editor

#endif //__JVEC3EDITOR_H__