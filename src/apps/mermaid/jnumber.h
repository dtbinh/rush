/*****************************************************************************/
/*	File:   JNumber.h
/*  Date:   08.08.2006
/*  Author: Juriy Skolyar
/*****************************************************************************/
#ifndef __JNumber_H__
#define __JNumber_H__

/*****************************************************************************/
/*	Class:  JNumber
/*	Desc:   Variable
/*****************************************************************************/
class JNumber : public JObject
{
    int             m_Value;//Value of a number
public: 
                    JNumber        () { m_Value = 13; }

    const char*     GetAdded        () const { return "0"; }//Makes no sense
    void            SetAdded        ( const char* val);//Addition the val

    const char*     GetDeduction    () const { return "0"; }//Makes no sense
    void            SetDeduction    ( const char* val);//Deduction the val

    const char*     GetMeasure      () const { return "0"; }//Makes no sense
    void            SetMeasure      ( const char* val);//Division on val

    const char*     GetMultiplier   () const { return "0"; }//Makes no sense
    void            SetMultiplier   ( const char* val);//Multiplication on val

    virtual void    Init            (){}

    expose( JNumber )
    {
        parent(JObject);
        field( "Value",         m_Value                         );
        prop ( "Added",         GetAdded,SetAdded               );
        prop ( "Deduction",     GetDeduction,SetDeduction       );
        prop ( "Multiplier",    GetMultiplier,SetMultiplier     );
        prop ( "Measure",       GetMeasure,SetMeasure           );
    }
}; // class JNumber

#endif // __JNumber_H__


