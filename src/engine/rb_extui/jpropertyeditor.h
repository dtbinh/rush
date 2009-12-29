/***********************************************************************************/
//  File:   JPropertyEditor.h
//  Date:   15.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPROPERTYEDITOR_H__
#define __JPROPERTYEDITOR_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JPropertyEditor
//  Desc:    
/***********************************************************************************/
class JPropertyEditor : public JWidget
{
    JString         m_InVal;
    JString         m_OutVal;

    JStringList     m_EnumList;

public:
    JPropertyEditor()
    {
    }

    const char*     GetInVal        () const { return m_InVal.c_str(); }    
    const char*     GetOutVal       () const { return m_OutVal.c_str(); }

    virtual void    Render          ();
    void            SetInVal        ( const char* val );
    void            SetOutVal       ( const char* val );

    void            CallMethod      ();
    JStringList     GetEnumList     () const { return m_EnumList; }
    void            SetEnumList     ( JStringList lst );

    expose(JPropertyEditor)
    {
        parent( JWidget );
        prop  ( "InVal",        GetInVal,      SetInVal     );
        prop  ( "OutVal",       GetOutVal,     SetOutVal    );
        rprop ( "EnumList",     GetEnumList );
        method( "CallMethod",   CallMethod  );
    }   
protected:
}; // class JPropertyEditor

#endif //__JPROPERTYEDITOR_H__