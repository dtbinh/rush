//****************************************************************************/
//  File:  ISignalServer.h
//****************************************************************************/
#ifndef __ISIGNALSERVER_H__
#define __ISIGNALSERVER_H__

class JObject;

//****************************************************************************/
//  Class:  ISignalServer 
//  Desc:
//****************************************************************************/
class ISignalServer  
{
public:
    virtual int  Connect( JObject* pObj ) = 0;
    virtual bool Connect( JObject* pSrcObj, const char* pSrcAttr, 
        JObject* pDstObj, const char* pDstAttr ) = 0;
}; // class ISignalServer

extern ISignalServer* g_pSignalServer;

#endif // __ISIGNALSERVER_H__




