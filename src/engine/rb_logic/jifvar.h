//****************************************************************************/
//  File:   JIfVar.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JIFVAR_H__
#define __JIFVAR_H__

#include "JWeakRef.h"
#include "JVar.h"

//****************************************************************************/
//	Enum:  JCmp
//  Desc:  Comparing operation used for predicates
//****************************************************************************/
enum JCmp
{
    Cmp_Equal       = 0,
    Cmp_NotEqual    = 1,
    Cmp_Less        = 2,
    Cmp_Greater     = 3,
    Cmp_LEqual      = 4,
    Cmp_GEqual      = 5
}; // enum JCmp

enum_beg(JCmp)
    enum_val( "Equal",    Cmp_Equal    ),
    enum_val( "NotEqual", Cmp_NotEqual ),
    enum_val( "Less",     Cmp_Less     ),
    enum_val( "Greater",  Cmp_Greater  ),
    enum_val( "LEqual",   Cmp_LEqual   ),
    enum_val( "GEqual" ,  Cmp_GEqual   )
enum_end(JCmp)

//****************************************************************************/
//  Class:  JIfVar
//  Desc:   Used as conditional branching block predicate
//****************************************************************************/
class JIfVar : public JAnimation
{
    JCmp            m_Cmp;
    JString     m_Var;
    JString     m_Value;
    JWeakRef<JVar>  m_pVar;
    bool            m_bAlwaysTrue;

public: 
                    JIfVar        ();
    virtual void    Init          (); 
    virtual void    OnPlay        ();
    virtual void    Render        ();

    expose( JIfVar )
    {
        parent(JAnimation);
        field( "AlwaysTrue",    m_bAlwaysTrue );
        field( "Cmp",           m_Cmp         );
        field( "Var",           m_Var         );
        field( "Value",         m_Value       );
    }
protected:
}; // class JIfVar

bool IsTrue( const char* a, const char* b, JCmp cmp );

#endif // __JIFVAR_H__


