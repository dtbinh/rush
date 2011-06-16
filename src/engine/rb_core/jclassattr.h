//****************************************************************************/
//  File:  JClassAttr.h
//****************************************************************************/
#ifndef __JCLASSATTR_H__
#define __JCLASSATTR_H__

#include "jstring.h"

class OutStream;
class InStream;
class JClassAttr;

typedef std::vector<JClassAttr*> JClassAttrList;

//****************************************************************************/
//  Class:  JClassAttr
//  Desc:   Interface for the property map attributes
//****************************************************************************/
class JClassAttr
{
public:
    const char*     GetName     () const;
    const char*     GetType     () const;

    virtual bool    Get         ( const void* pThis, void* pVal, int tag = -1 ) const { return false; }
    virtual bool    Set         ( void* pThis, const void* pVal, int tag = -1 ) { return false; }
    virtual int     Serialize   ( void* pThis, OutStream& os ) { return 0; }
    virtual int     Unserialize ( void* pThis, InStream& is ) { return 0; }


    virtual bool    Get         ( const void* pThis, JString& str, int tag = -1 ) const { return false; }
    virtual bool    Set         ( void* pThis, const JString& str, int tag = -1 ) { return false; }
    virtual bool    Enumerate   ( JStringList& lst ) { return false; }
    virtual bool    Call        ( void* pThis, int tag = -1 ) { return false; }
    virtual int     GetTypeID   () const { return -1; }
    virtual int     GetSizeOf   () const { return 0; }

    inline bool     HasName     ( const char* name ) const;
    inline bool     HasType     ( const char* type ) const;
    inline bool     IsDerived   () const;

    void            SetName     ( const char* name );

protected:
                    JClassAttr  ();
                    JClassAttr  ( const char* name, const char* type );

    JString         m_Name;     //  name of the attribute
    JString         m_Type;     //  type name

    JMetaClass*     m_pMeta;    //  property map for child attributes (for complex types)

    bool            m_bDerived;

    friend class JMetaClass;
}; // class JClassAttr

//*****************************************************************************
//  JClassAttr inlines
//*****************************************************************************
inline JClassAttr::JClassAttr()
    :   m_pMeta     ( NULL ),
        m_bDerived  ( false )
{
}

inline JClassAttr::JClassAttr( const char* name, const char* type )
    :  m_Type( type ),
       m_pMeta( NULL ),
       m_bDerived  ( false )
{
    SetName( name );
}

inline const char* JClassAttr::GetName() const
{
    return m_Name.c_str();
}

inline const char* JClassAttr::GetType() const
{
    return m_Type.c_str();
}

inline bool JClassAttr::HasName( const char* name ) const
{
    return m_Name.is_equal_ci( name );
}

inline bool JClassAttr::HasType( const char* type ) const
{
    return m_Type == type;
}

inline bool JClassAttr::IsDerived() const
{
    return m_bDerived;
}

#endif // __JCLASSATTR_H__


