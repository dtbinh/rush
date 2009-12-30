//****************************************************************************/
//  File:  JReflect.h
//****************************************************************************/
#ifndef __JREFLECT_H__
#define __JREFLECT_H__

#include "jenum.h"
#include "jtypecast.h"
#include "jmetaclass.h"

#define expose( CName )             public: virtual const char* ClassName() const { return s_pMetaClass ? s_pMetaClass->GetName() : #CName; } \
                                    static JMetaClass* s_pMetaClass;\
                                    virtual JMetaClass* GetMetaClass() const { return s_pMetaClass; } \
                                    typedef CName MyType;\
                                    virtual void Expose( JMetaClass& mc )

//  decl_class macro should be used for the class we want to register
//    at the object factory - insert it after class declaration
#define decl_class( CName ) JObject* __##CName##Creator() { return (JObject*)(new CName); } \
    JMetaClass* CName##::s_pMetaClass = NULL;\
    int __##CName##Registrator() { CName##::s_pMetaClass = g_pObjectServer->RegClass( #CName, __##CName##Creator );\
        return CName##::s_pMetaClass ? 1 : 0; }

//  link_class macro should be paired with the decl_class macro. Its purpose
//    is to explicitly link class registration code to assure linker wont
//    optimize it out
#define link_class( CName ) int __##CName##Registrator(); printf( "", #CName, __##CName##Registrator() );

#define parent( CName )             mc.SetParent( #CName );
#define prop( PName, Get, Set )     mc.Property( PName, &MyType::Get, &MyType::Set );
#define rprop( PName, Get )         mc.Property( PName, &MyType::Get );
#define field( PName, Val )         mc.Field( PName, MyType::Val, int( &((MyType*)NULL)->Val ) );
#define method( PName, Fun )        mc.Method( PName, &MyType::Fun );

#define import(PackageName) void PackageName##_import();\
    PackageName##_import();

#define export(PackageName) void PackageName##_import()

template <class DObj>
inline bool is_a( JObject* pObj )
{
    if (!pObj) return false;
    const JMetaClass* pObjMeta = pObj->GetMetaClass();
    const JMetaClass* pDstMeta = DObj::s_pMetaClass;
    if (!pObjMeta || !pDstMeta)
    {
        return false;
    }
    do
    {
        if (pObjMeta == pDstMeta)
        {
            return true;
        }
        pObjMeta = pObjMeta->GetParent();
    } while (pObjMeta != NULL);
    return false;
}

inline bool is_a( JObject* pObj, const char* typeName )
{
    if (!pObj) return false;
    const JMetaClass* pObjMeta = pObj->GetMetaClass();
    if (!pObjMeta)
    {
        return false;
    }
    do
    {
        if (!stricmp( pObjMeta->GetName(), typeName ))
        {
            return true;
        }
        pObjMeta = pObjMeta->GetParent();
    } while (pObjMeta != NULL);
    return false;
}

inline bool is_a( JObject* pObj, const JMetaClass* pMeta )
{
    if (!pObj) return false;
    const JMetaClass* pObjMeta = pObj->GetMetaClass();
    if (!pObjMeta || !pMeta)
    {
        return false;
    }
    do
    {
        if (pObjMeta == pMeta)
        {
            return true;
        }
        pObjMeta = pObjMeta->GetParent();
    } while (pObjMeta != NULL);
    return false;
}

template <class DObj>
inline bool is_exact( JObject* pObj )
{
    if (!pObj) return false;
    const JMetaClass* pObjMeta = pObj->GetMetaClass();
    const JMetaClass* pDstMeta = DObj::s_pMetaClass;
    return (pObjMeta == pDstMeta);
}

inline bool is_exact( JObject* pObj, const char* typeName )
{
    if (!pObj) return false;
    const JMetaClass* pObjMeta = pObj->GetMetaClass();
    return pObjMeta && (!stricmp( pObjMeta->GetName(), typeName ));
}

inline bool is_exact( JObject* pObj, const JMetaClass* pMeta )
{
    if (!pObj) return false;
    const JMetaClass* pObjMeta = pObj->GetMetaClass();
    return (pObjMeta == pMeta);
}

template <class DObj>
inline DObj* obj_cast( JObject* pObj )
{
    return is_a<DObj>( pObj ) ? (DObj*)pObj : NULL;
}

#endif // __JREFLECT_H__


