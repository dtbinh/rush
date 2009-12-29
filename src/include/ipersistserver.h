//****************************************************************************/
//  File:  IPersistServer.h
//****************************************************************************/
#ifndef __IPERSISTSERVER_H__
#define __IPERSISTSERVER_H__

class JObject;
class JMLNode;
class TiXmlElement;

class InStream;
class OutStream;

enum JPersistFormat
{
    PersistFmt_Unknown = 0,
    PersistFmt_XML     = 1,
    PersistFmt_JML     = 2,
    PersistFmt_Bin     = 3,
    PersistFmt_Model   = 4,
    PersistFmt_Anim    = 5,
}; // enum JPersistFormat

//****************************************************************************/
//  Class:  IPersistServer 
//  Desc:
//****************************************************************************/
class IPersistServer  
{
public:
    virtual             ~IPersistServer () {}
    virtual JObject*    LoadObject      ( const char* fname, JObject* pSrc = NULL ) = 0;
    virtual bool        SaveObject      ( JObject* pObject, const char* fname, JPersistFormat fmt = PersistFmt_JML ) = 0;

    virtual JObject*    Load            ( InStream& is, JPersistFormat fmt = PersistFmt_Unknown, 
        JObject* pSrc = NULL, DWORD* pHash = NULL ) = 0;
    virtual bool        Save            ( JObject* pObject, OutStream& os, JPersistFormat fmt = PersistFmt_JML ) = 0;
    virtual bool        Save            ( JObject* pObject ) = 0;

    virtual JObject*    LoadBin         ( InStream& is, JObject* pSrc = NULL ) = 0;
    virtual bool        SaveBin         ( OutStream& os, const JObject* pObject, bool bWithChildren = true, 
        bool bSaveSignals = true ) = 0;

    virtual void        SaveMetaTable   ( OutStream& os ) const = 0;
    virtual void        ReloadScripts   () = 0;
    virtual void        SaveScripts     () = 0;
}; // class IPersistServer

extern IPersistServer* g_pPersistServer;

#endif // __IPERSISTSERVER_H__




