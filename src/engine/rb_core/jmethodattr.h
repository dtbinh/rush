//****************************************************************************/
//  File:  JMethodAttr.h
//****************************************************************************/
#ifndef __JMETHODATTR_H__
#define __JMETHODATTR_H__

//****************************************************************************/
//  Class:  JMethodAttr
//****************************************************************************/
template <class BaseT>
class JMethodAttr : public JClassAttr
{
    typedef void (BaseT::*FnProcess)();
    FnProcess       m_fnProcess;

public:
    JMethodAttr( const char* name, FnProcess call ) : m_fnProcess( call ) 
    {
        SetName( name );   
        m_Type = "method";
    }

    virtual bool Call( void* pThis, int tag = -1 ) 
    {
        if (!m_fnProcess) return false;
        (reinterpret_cast<BaseT*>( pThis )->*m_fnProcess)();
        return true;
    }

    virtual bool    Get( const void* pThis, JString& str, int tag = -1 ) const 
    { 
        return cast<JString, int>( str, tag );
    }
}; // class JMethodAttr

#endif // __JMETHODATTR_H__


