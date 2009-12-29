//****************************************************************************/
//  File:  JPropertyAttr.h
//****************************************************************************/
#ifndef __JPROPERTYATTR_H__
#define __JPROPERTYATTR_H__

//****************************************************************************/
//  Class:	JPropertyAttr
//****************************************************************************/
template <class BaseT, class PropT>
class JPropertyAttr : public JClassAttr
{
    typedef PropT (BaseT::*FnGetter) () const;
    typedef void  (BaseT::*FnSetter)( PropT val );

    FnGetter		m_Getter;	
    FnSetter		m_Setter; 

public:
    JPropertyAttr()
    {
        m_Getter    = NULL;
        m_Setter    = NULL;
    }

    JPropertyAttr( const char* name, FnGetter getter, 
                   FnSetter setter ) 
    { 
        m_Getter    = getter;
        m_Setter    = setter;
        m_Type      = type_name<PropT>();
        SetName( name );
    }

    virtual int     GetTypeID   () const 
    {
        return ::GetTypeID<PropT>(); 
    }

    virtual int     GetSizeOf   () const { return sizeof( PropT ); }

    virtual bool Get( const void* pThis, JString& str, int tag = -1 ) const 
    {
        PropT val;
        if (!Get( pThis, &val )) return false;
        if (cast( str, val )) return true;
        return false;
    }

    virtual bool Set( void* pThis, const JString& str, int tag = -1 ) 
    {
        PropT val;
        if (!cast( val, str )) return false;
        return Set( pThis, &val );
    }

    virtual bool Get( const void* pThis, void* pVal, int tag = -1 ) const 
    {
        if (!m_Getter || !pThis || !pVal) return false;
        PropT* pObj = new ( pVal )PropT;
        *pObj = ((reinterpret_cast<const BaseT*>( pThis )->*m_Getter)());
        return true;
    }

    virtual bool Set( void* pThis, const void* pVal, int tag = -1 ) 
    {
        if (!m_Setter || !pThis || !pVal) return false;
        (reinterpret_cast<BaseT*>( pThis )->*m_Setter)( *((const PropT*)pVal) );
        return true;
    }

    virtual int Serialize( void* pThis, OutStream& os ) 
    { 
        if (!m_Getter || !pThis || !m_Setter) return 0;
        PropT val;
        if (!Get( pThis, &val )) return 0;
        os << val; 
        return sizeof( val );
    }

    virtual int Unserialize( void* pThis, InStream& is ) 
    { 
        if (!m_Getter || !pThis || !m_Setter) return 0;
        PropT val;
        is >> val; 
        Set( pThis, &val );
        return sizeof( val );
    }

    virtual bool Enumerate( JStringList& lst ) 
    { 
        return enumerate<PropT>( lst ); 
    }

}; // class JPropertyAttr


#endif // __JPROPERTYATTR_H__


