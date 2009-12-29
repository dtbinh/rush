//****************************************************************************/
//  File:  JSignal.h
//****************************************************************************/
#ifndef __JSIGNAL_H__
#define __JSIGNAL_H__

#include "JString.h"

enum JSignalType
{
    Signal_None    = 0,
    Signal_In      = 1,
    Signal_Out     = 2,
    Signal_InOut   = 3,
}; // enum JSignalType

class JObject;
class JClassAttr;
//****************************************************************************/
//  Class:  JSignal
//  Desc:
//****************************************************************************/
class JSignal
{
public:
    JObject*        m_pSrcObj;
    JObject*        m_pDstObj;

    JClassAttr*     m_pSrcAttr;
    JClassAttr*     m_pDstAttr;

    JString         m_SrcPath;
    JString         m_DstPath;
    JSignalType     m_Type;

    bool            m_bSrcInit;         //  whether connection is initiated by source
    bool            m_bConnected;
    uint32_t           m_Token;

    JSignal() :
        m_pSrcObj   ( NULL ),
        m_pDstObj   ( NULL ),
        m_pSrcAttr  ( NULL ),
        m_pDstAttr  ( NULL ),
        m_Type      ( Signal_None ),
        m_bSrcInit  ( true ),
        m_bConnected( false ),
        m_Token     ( 0 )
    {}

    void            CreateSymbolicNames ();
    JString         CreateSymbolicName  ( JObject* pObj, JClassAttr* pAttr );
    bool            Fire                ();
    bool            operator ==         ( const JSignal& c ) const;

    void            SetSrc  ( JObject* pSrcObj );
    void            SetDst  ( JObject* pDstObj );
    JObject*        GetSrc  () const { return m_pSrcObj; }
    JObject*        GetDst  () const { return m_pDstObj; }

}; // class JSignal

#endif // __JSIGNAL_H__


