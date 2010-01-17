//****************************************************************************/
//  File:  JSignalServer.h
//****************************************************************************/
#ifndef __JSIGNALSERVER_H__
#define __JSIGNALSERVER_H__

#include "isignalserver.h"
#include "jsignal.h"
#include "jobject.h"
#include "uniformpool.h"


class JMLNode;

typedef UniformPool<JSignal> JSignalPool;
typedef hash_multimap_t<JObject*, JSignal*> JSignalReg;
//****************************************************************************/
//  Class:  JSignalServer, singleton
//  Desc:   Tracks and manages connections between object's slots.
/*              Any class attribute declared in by class' expose macro could be
/*              used as slot.
//****************************************************************************/
class JSignalServer : public JObject, public ISignalServer
{
    JSignalPool                 m_SignalPool;
    JSignalReg                  m_SignalReg;

    uint32_t                    m_CurToken;
    JObject*                    m_pCurSrcObj;
    JString                     m_CurSrcAttr;

    bool                        m_bLogSignals;


public:
                                JSignalServer   ();
    virtual                     ~JSignalServer  ();

    virtual void                Render          ();

    bool                        Connect         ( JObject* pSrcObj, JClassAttr* pSrcAttr,
                                                  JObject* pDstObj, JClassAttr* pDstAttr );

    bool                        Connect         ( JObject* pSrcObj, const char* pSrcAttr,
                                                  JObject* pDstObj, const char* pDstAttr );

    //!  tries to connect all registered in/out signals for the given object
    virtual int                 Connect         ( JObject* pObj );

    bool                        ConnectSrc      ( JObject* pSrcObj, JClassAttr* pSrcAttr, const char* dst );
    bool                        ConnectDst      ( JObject* pDstObj, JClassAttr* pDstAttr, const char* src );
    bool                        SendSignal      ( JObject* pSrcObj, const char* pSrcAttr = NULL );
    bool                        PollSlot        ( JObject* pDstObj, const char* pDstAttr = NULL );

    int                         GetNumSignals   () const { return m_SignalPool.GetNElem(); }
    int                         GetNumObjEntries() const { return m_SignalReg.size(); }


    int                         ClearSignals    ( JObject* pObj );
    void                        CloneSignals    ( JObject* pSrc, JObject* pDst );

    JObject*                    GetSourceObject () const { return m_pCurSrcObj; }
    const char*                 GetSourceAttr   () const { return m_CurSrcAttr.c_str(); }

    void                        DumpConnections ();
    JStringList                 GetSignalList   () const;

    void                        GetSignalList   ( const JObject* pObj, std::vector<JSignal*>& sigList ) const;

    static JSignalServer*       s_pInstance;

    expose( JSignalServer )
    {
        parent( JObject );
        rprop( "SignalList", GetSignalList );
        rprop( "NumSignals", GetNumSignals );
        field( "LogSignals", m_bLogSignals );
        method( "DumpConnections", DumpConnections );
    }

protected:
    JSignal*        FindSignal      ( const JSignal* pSig );
    bool            RegSignal       ( JSignal* pSignal, bool bSource );

    bool            Connect         ( JSignal* pSignal );
    bool            LocateSlot      ( const char* symPath, JObject* &pObj, JClassAttr* &pAttr, JObject* pContext );

}; // class JSignalServer



#endif // __JSIGNALSERVER_H__


