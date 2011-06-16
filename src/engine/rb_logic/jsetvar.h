//****************************************************************************/
//  File:   JSetVar.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JSETVAR_H__
#define __JSETVAR_H__

#include "JWeakRef.h"

//****************************************************************************/
//  Enum:  JSetOperation
//  Desc:  Operation when setting value
//****************************************************************************/
enum JSetOperation
{
    SetOp_Set          = 0,
    SetOp_Add          = 1,
    SetOp_Subtract     = 2,
    SetOp_Multiply     = 3,
    SetOp_SetIfGreater = 4,
    SetOp_SetIfLess    = 5,
}; // enum JSetOperation

enum_beg(JSetOperation)
    enum_val( "Set",            SetOp_Set          ),
    enum_val( "Add",            SetOp_Add          ),
    enum_val( "Subtract",       SetOp_Subtract     ),
    enum_val( "Multiply",       SetOp_Multiply     ),
    enum_val( "SetIfGreater",   SetOp_SetIfGreater ),
    enum_val( "SetIfLess" ,     SetOp_SetIfLess    )
enum_end(JSetOperation)

//****************************************************************************/
//  Class:  JSetVar
//  Desc:   Changes value of some variable
//****************************************************************************/
class JSetVar : public JAnimation
{
    JString     m_Var;
    JString     m_Value;    
    JSetOperation   m_Operation;

    JWeakRef<JVar>   m_pVar;

public: 
                    JSetVar       ();
    virtual void    OnPlay        (); 
    virtual void    Init          (); 

    expose( JSetVar )
    {
        parent(JAnimation);
        field( "Var",       m_Var       );
        field( "Operation", m_Operation );
        field( "Value",     m_Value     );
    }
}; // class JSetVar

#endif // __JSetVar_H__


