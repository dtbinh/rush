//****************************************************************************/
//  File:   JSetProp.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JSETPROP_H__
#define __JSETPROP_H__

#include "JWeakRef.h"

//****************************************************************************/
//  Class:  JSetProp
//  Desc:   Changes property value of some object of JObject-derived class
//****************************************************************************/
class JSetProp : public JAnimation
{
public: 
                    JSetProp      ();
    virtual void    OnPlay        (); 
    virtual void    Init          (); 

    expose( JSetProp )
    {
        parent(JAnimation);
        field( "Object",    m_Object     );
        field( "Prop",      m_Prop       );
        field( "Value",     m_Value      );
        field( "MinVal",    m_MinVal     );
        field( "MaxVal",    m_MaxVal     );
        field( "SrcObject", m_SrcObject  );
        field( "SrcProp",   m_SrcProp    );
    }

private:
    JString             m_Object;
    JString             m_Prop;
    JString             m_SrcObject;
    JString             m_SrcProp;
    JString             m_Value;

    int                 m_MinVal;
    int                 m_MaxVal;

    JWeakRef<JObject>   m_pObject;
    JWeakRef<JObject>   m_pSrcObject;
}; // class JSetProp

#endif // __JSETPROP_H__


