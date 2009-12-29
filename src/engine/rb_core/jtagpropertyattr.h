/***********************************************************************************/
//  File:   JTagPropertyAttr.h
//  Date:   28.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JTAGPROPERTYATTR_H__
#define __JTAGPROPERTYATTR_H__

/***********************************************************************************/
//  Class:  JTagPropertyAttr
//  Desc:   
/***********************************************************************************/
template <class BaseT, class PropT>
class JTagPropertyAttr : public JClassAttr
{
    typedef PropT (BaseT::*FnGetter)( int tag ) const;
    typedef void  (BaseT::*FnSetter)( int tag, PropT val );
    
    FnGetter		m_Getter;	
    FnSetter		m_Setter; 

public:
    JTagPropertyAttr( const char* name, FnGetter getter, FnSetter setter ) 
    { 
        m_Getter = getter;
        m_Setter = setter;
        SetName( name );
    }

    virtual int     GetTypeID   () const { return ::GetTypeID<PropT>(); }
    virtual int     GetSizeOf   () const { return sizeof( PropT ); }
    virtual bool    Get         ( const void* pThis, JString& str, int tag = -1 ) const 
    {
        PropT val;
        if (!Get( pThis, &val, tag )) return false;
        if (cast( str, val )) return true;
        return false;
    }

    virtual bool Set( void* pThis, const JString& str, int tag = -1 ) 
    {
        PropT val;
        if (!cast( val, str )) return false;
        return Set( pThis, &val, tag );
    }

    virtual bool Get( const void* pThis, void* pVal, int tag = -1 ) const 
    {
        if (!m_Getter || !pThis || !pVal) return false;
        PropT* pObj = new ( pVal )PropT;
        const BaseT* pHost = reinterpret_cast<const BaseT*>( pThis );
        *pObj = (pHost->*m_Getter)( tag );
        return true;
    }

    virtual bool Set( void* pThis, const void* pVal, int tag = -1 ) 
    {
        if (!m_Setter || !pThis || !pVal) return false;
        BaseT* pHost = reinterpret_cast<BaseT*>( pThis );
        (pHost->*m_Setter)( tag, *((const PropT*)pVal) );
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

}; // class JTagPropertyAttr


#endif //__JTAGPROPERTYATTR_H__