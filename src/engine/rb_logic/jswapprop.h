//****************************************************************************/
//  File:   JSwapProp.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JSWAPPROP_H__
#define __JSWAPPROP_H__

#include "JWeakRef.h"

//****************************************************************************/
//  Class:  JSwapProp
//  Desc:   Changes property value of some object of JObject-derived class
//****************************************************************************/
class JSwapProp : public JAnimation
{
    JString         m_Object1;
    JString         m_Prop1;

    JString         m_Object2;
    JString         m_Prop2;

    JWeakRef<JObject>   m_pObject1;
    JWeakRef<JObject>   m_pObject2;

public: 
                    JSwapProp     ();
    virtual void    OnPlay        (); 
    virtual void    Init          (); 

    expose( JSwapProp )
    {
        parent(JAnimation);
        field( "Object1",    m_Object1  );
        field( "Prop1",      m_Prop1    );
        field( "Object2",    m_Object2  );
        field( "Prop2",      m_Prop2    );
    }
}; // class JSwapProp

#endif // __JSWAPPROP_H__


