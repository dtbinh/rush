//****************************************************************************/
//  File:  JObjectServer.h
//****************************************************************************/
#ifndef __JOBJECTSERVER_H__
#define __JOBJECTSERVER_H__

#include <hash_map>

#include "jmetaclass.h"
#include "jweakrefreg.h"
#include "jstring.h"
#include "uniformpool.h"

class JObject;

class JClassAttr;

typedef UniformPool<JMetaClass>                    JClassList;
typedef stdext::hash_map<JString, JMetaClass*>     JClassMap;
typedef stdext::hash_map<JString, JClassAttr*>     JAttrMap;
typedef stdext::hash_multimap<JString, JObject*>   JObjectMap;
//****************************************************************************/
//  Class:  JObjectServer
//  Desc:
//****************************************************************************/
class JObjectServer
{
public:
                            JObjectServer   ();
    virtual                 ~JObjectServer  ();
    JMetaClass*             RegClass        ( const char* cname, ObjCreator creator = NULL );

    JMetaClass*             FindClass       ( const char* cname );
    JClassAttr*             FindClassAttr   ( const char* cname , const char* attr );
    int                     GetNClasses     () const { return m_ClassMap.size(); }
    JMetaClass*             GetClass        ( int idx );

    int                     Register        ( JObject* pObject );
    bool                    Unregister      ( JObject* pObject );

    int                     GetNObjects     () const { return m_NumObjects; }
    int                     GetNDelObjects  () const { return m_NumDeleted; }

    //! finds object with path sympath, located in subtree of pRoot, in scope of pAnchor
    JObject*                FindObject      ( const char* symPath, JObject* pRoot = NULL, JObject* pAnchor = NULL );
    JObject*                Create          ( const char* cname );

    JObject*                CreateFromScript( const char* text, JObject* pSrc = NULL );
    JObject*                CloneObject     ( const JObject* pObj, JObject* pParent, const char* name = NULL,
                                              bool bCloneSignals = false, bool bCloneChildren = true );
    bool                    AreObjectsEqual ( const JObject* pObj1, const JObject* pObj2, bool bWithChildren = true );
    void                    RegAttr         ( const JString& key, JClassAttr* pAttr ) { m_AttrMap[key] = pAttr; }

    void                    DumpTree        ( const JObject* pObj, int indent = 0 );
    void                    DumpObjectMap   ();

    template <class T>
    inline T*               FindObject      ( const char* name, JObject* pRoot = NULL );

    static JObjectServer&   inst();

private:
    JClassList              m_ClassList;
    JClassMap               m_ClassMap;
    JAttrMap                m_AttrMap;
    JObjectMap              m_ObjectMap;
    JWeakRefReg             m_WeakRefReg;

    int                     m_NumObjects;
    int                     m_NumDeleted;

    //!  assigns default name to the given object, for example "button00"
    void                    AutoNameObject  ( JObject* pObject );

}; // class JObjectServer

extern JObjectServer* g_pObjectServer;


template <class T>
inline T* JObjectServer::FindObject( const char* name, JObject* pRoot )
{
    static JString fullName;
    fullName = T::s_pMetaClass ? T::s_pMetaClass->GetName() : "*";
    fullName += ":";
    fullName += name;
    JObject* pObj = FindObject( fullName.c_str(), NULL, pRoot );
    T* pNode = obj_cast<T>( pObj );
    return pNode;
}

#endif // __JOBJECTSERVER_H__


