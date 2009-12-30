//****************************************************************************/
//  File:  JFieldAttr.h
//****************************************************************************/
#ifndef __JFIELDATTR_H__
#define __JFIELDATTR_H__

#include "stream.h"

//****************************************************************************/
//  Class:	JFieldAttr
//****************************************************************************/
template <class FieldT>
class JFieldAttr : public JClassAttr
{
    int		        m_Offset;

public:
    JFieldAttr( const char* name, int offset )
    {
        SetName( name );
        m_Offset    = offset;
        m_Type      = type_name<FieldT>();
    }

    virtual int     GetTypeID   () const
    {
        return ::GetTypeID<FieldT>();
    }

    virtual int     GetSizeOf   () const { return sizeof( FieldT ); }

    virtual bool Get( const void* pThis, JString& str, int tag = -1 ) const
    {
        FieldT val;
        if (!Get( pThis, &val )) return false;
        return cast( str, val );
    }

    virtual bool Set( void* pThis, const JString& str, int tag = -1 )
    {
        FieldT val;
        if (!cast( val, str )) return false;
        return Set( pThis, &val );
    }

    virtual bool Get( const void* pThis, void* pVal, int tag = -1 ) const
    {
        if (!pThis || !pVal) return false;
        FieldT* pObj = new ( pVal )FieldT;
        *pObj = FieldRef( pThis );
        return true;
    }

    virtual bool Set( void* pThis, const void* pVal, int tag = -1 )
    {
        if (!pVal || !pThis) return false;
        FieldRef( pThis ) = *((const FieldT*)pVal);
        return true;
    }

    virtual int Serialize( void* pThis, OutStream& os )
    {
        if (!pThis) return false;
        os << FieldRef( pThis );
        return sizeof( FieldT );
    }

    virtual int Unserialize( void* pThis, InStream& is )
    {
        if (!pThis) return false;
        is >> FieldRef( pThis );
        return sizeof( FieldT );
    }

    virtual bool Enumerate( JStringList& lst )
    {
        return enumerate<FieldT>( lst );
    }

protected:
    FieldT&	FieldRef( void* pThis )
    {
        return *((FieldT*)((unsigned char*)pThis + m_Offset));
    }

    const FieldT& FieldRef( const void* pThis ) const
    {
        return *((FieldT*)((unsigned char*)pThis + m_Offset));
    }
}; // class JFieldAttr


#endif // __JFIELDATTR_H__


