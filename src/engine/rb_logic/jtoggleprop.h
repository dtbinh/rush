//****************************************************************************/
//  File:   JToggleProp.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JTOGGLEPROP_H__
#define __JTOGGLEPROP_H__

#include "JWeakRef.h"

//****************************************************************************/
//  Class:  JToggleProp
//  Desc:   Toggles boolean property value of some object of JObject-derived class
//****************************************************************************/
class JToggleProp : public JAnimation
{
    JString             m_Object;
    JString             m_Prop;
    JWeakRef<JObject>   m_pObject;

public: 
                    JToggleProp   ();
    virtual void    OnPlay        (); 
    virtual void    Init          (); 

    expose( JToggleProp )
    {
        parent(JAnimation);
        field( "Object",    m_Object     );
        field( "Prop",      m_Prop       );
    }
}; // class JToggleProp

#endif // __JTOGGLEPROP_H__


