//****************************************************************************/
//  File:  JPersistServer.h
//****************************************************************************/
#ifndef __JPERSISTSERVER_H__
#define __JPERSISTSERVER_H__

#include "ipersistserver.h"

#include "jobject.h"
#include "jmetaclass.h"
#include "jstring.h"
#include "jweakref.h"

class JMLNode;
class TiXmlElement;

enum JAttrSaveMode
{
    SaveMode_SaveAll = 0,
    SaveMode_uint8_tIdx = 1,
    SaveMode_uint16_tIdx = 2
}; // enum JAttrSaveMode

struct ScriptResource
{
    JString             m_Name;
    JString             m_Path;
    JWeakRef<JObject>   m_pRoot;
    uint32_t               m_Hash;
    JPersistFormat      m_Format;
};

typedef std::vector<ScriptResource> ScriptResReg;

//****************************************************************************/
//  Class:  JPersistServer
//  Desc:
//****************************************************************************/
class JPersistServer : public JObject, public IPersistServer
{
public:
                JPersistServer  ();
    virtual     ~JPersistServer ();

    JObject*    LoadObject      ( const char* fname, JObject* pSrc = NULL );
    bool        SaveObject      ( JObject* pObject, const char* fname, JPersistFormat fmt = PersistFmt_JML );

    JObject*    Load            ( InStream& is, JPersistFormat fmt = PersistFmt_Unknown, JObject* pSrc = NULL, uint32_t* pHash = NULL );
    bool        Save            ( JObject* pObject, OutStream& os, JPersistFormat fmt = PersistFmt_JML );
    bool        Save            ( JObject* pObject );

    JObject*    LoadBin         ( InStream& is, JObject* pSrc = NULL );
    bool        SaveBin         ( OutStream& os, const JObject* pObject, bool bWithChildren = true, bool bSaveSignals = true );

    void        SaveMetaTable   ( OutStream& os ) const;
    void        ReloadScripts   ();

    void        SaveScripts     ();

    expose( JPersistServer )
    {
        parent( JObject );
        method( "ReloadScripts", ReloadScripts );
    }

private:

    JObject*        LoadJML ( char* text, JObject* pSrc = NULL );
    JObject*        FromJML ( const JMLNode& jml, JObject* pSrc = NULL, JObject* pParent = NULL );
    JMLNode*        ToJML   ( const JObject* pObject, bool bRoot = false );
    bool            SaveJML ( OutStream& os, const JObject* pObject );

    JObject*        LoadXML ( char* text, JObject* pSrc = NULL );
    TiXmlElement*   ToXML   ( const JObject* pObject, bool bRoot = false );
    JObject*        FromXML ( const TiXmlElement* pNode, JObject* pSrc );
    bool            SaveXML ( OutStream& os, const JObject* pObject );

    int             LoadSignalsBin  ( JObject* pObj, InStream& is );
    int             SaveSignalsJML  ( const JObject* pObj, JMLNode* pNode );
    int             SaveSignalsXML  ( const JObject* pObj, TiXmlElement* pNode );
    int             SaveSignalsBin  ( const JObject* pObj, OutStream& os );

    ScriptResReg    m_ScriptResReg;

    ScriptResource* FindScriptResource( const char* name );
    ScriptResource* FindScriptResource( const JObject* pObject );

}; // class JPersistServer

#endif // __JPERSISTSERVER_H__




