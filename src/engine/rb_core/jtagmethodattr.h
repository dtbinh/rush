/***********************************************************************************/
//  File:   JTagMethodAttr.h
//  Date:   28.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JTAGMETHODATTR_H__
#define __JTAGMETHODATTR_H__

/***********************************************************************************/
//  Class:  JTagMethodAttr
//  Desc:   
/***********************************************************************************/
template <class BaseT>
class JTagMethodAttr : public JClassAttr
{
    typedef void (BaseT::*FnProcess)(int tag);
    FnProcess       m_fnProcess;

public:
    JTagMethodAttr( const char* name, FnProcess call ) : m_fnProcess( call ) 
    {
        SetName( name );   
        m_Type = "method";
    }

    virtual bool Call( void* pThis, int tag = -1 ) 
    {
        if (!m_fnProcess) return false;
        (reinterpret_cast<BaseT*>( pThis )->*m_fnProcess)( tag );
        return true;
    }

    virtual bool    Get( void* pThis, JString& str, int tag = -1 ) const 
    { 
        str = "true";
        return true; 
    }

}; // class JTagMethodAttr


#endif //__JTAGMETHODATTR_H__