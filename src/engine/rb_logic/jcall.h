//****************************************************************************/
//  File:   JCall.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JCALL_H__
#define __JCALL_H__

#include "JWeakRef.h"

//****************************************************************************/
//  Class:  JCall
//  Desc:   Calls some method of some object
//****************************************************************************/
class JCall : public JAnimation
{
    JString         m_Object;
    JString         m_Method;
    JWeakRef<JObject>   m_pObject;

public: 
                    JCall      ();
    virtual void    OnPlay     (); 
    virtual void    Init       (); 

    expose( JCall )
    {
        parent(JAnimation);
        field( "Object",    m_Object  );
        field( "Method",    m_Method  );
    }
}; // class JCall

#endif // __JCALL_H__


