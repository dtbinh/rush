//****************************************************************************/
//  File:   JVar.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JVAR_H__
#define __JVAR_H__

//****************************************************************************/
//  Class:  JVar
//  Desc:   Variable
//****************************************************************************/
class JVar : public JObject
{
    JString         m_Value;
    JString         m_Default;
public: 
                    JVar            () {}

    bool            GetFloatValue   ( float& val ) const;
    bool            GetIntValue     ( int& val ) const;
    void            ResetValue      () { m_Value = m_Default; }
    virtual void    Init            () { if (m_Value.size() == 0) ResetValue(); }

    const char*     GetValue        () const { return m_Value.c_str(); }
    const char*     GetDefault      () const { return m_Default.c_str(); }

    void            SetValue        ( const char* val );
    void            SetDefault      ( const char* val );

    expose( JVar )
    {
        parent(JObject);
        prop( "Value",     GetValue, SetValue );
        prop( "Default",   GetDefault, SetDefault );
    }
}; // class JVar

#endif // __JVAR_H__


