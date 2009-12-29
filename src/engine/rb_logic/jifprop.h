//****************************************************************************/
//  File:   JIfProp.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JIFPROP_H__
#define __JIFPROP_H__

#include "JWeakRef.h"
#include "JIfVar.h"

//****************************************************************************/
//  Class:  JIfProp
//  Desc:   Used as conditional branching block predicate
//****************************************************************************/
class JIfProp : public JAnimation
{
    JCmp                m_Cmp;
    JString         m_Prop;
    JString         m_Value;
    JString         m_Object;
    JWeakRef<JObject>   m_pObject;
    bool                m_bStatic;
    bool                m_bAlwaysTrue;

public: 
                    JIfProp       ();
    virtual void    OnPlay        (); 
    virtual void    Init          (); 
    virtual void    Render        (); 

    expose( JIfProp )
    {
        parent(JAnimation);
        field( "AlwaysTrue",    m_bAlwaysTrue   );
        field( "Static",        m_bStatic       );
        field( "Cmp",           m_Cmp           );
        field( "Prop",          m_Prop          );
        field( "Value",         m_Value         );
        field( "Object",        m_Object        );
    }
}; // class JIfProp

#endif // __JIFPROP_H__


