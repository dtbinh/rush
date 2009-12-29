/***********************************************************************************/
//  File:   JObjectFrontEnd.h
//  Date:   14.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JOBJECTFRONTEND_H__
#define __JOBJECTFRONTEND_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JObjectFrontEnd
//  Desc:   Representation of the specific object's widget in the object tree 
/***********************************************************************************/
class JObjectFrontEnd : public JWidget
{
    JString     m_ObjectName;
    JString     m_ObjectClass;
    
public:
                    JObjectFrontEnd     ();

    const char*     GetObjectName   () const { return m_ObjectName.c_str(); } 
    const char*     GetObjectClass  () const { return m_ObjectClass.c_str(); }

    void            SetObjectName   ( const char* name ); 
    void            SetObjectClass  ( const char* name );

    expose(JObjectFrontEnd)
    {
        parent(JWidget);
        prop( "ObjectName",  GetObjectName, SetObjectName   );
        prop( "ObjectClass", GetObjectClass, SetObjectClass );
    }
}; // class JObjectFrontEnd

#endif //__JOBJECTFRONTEND_H__