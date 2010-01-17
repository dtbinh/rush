//****************************************************************************/
//  File:  JObjCast.h
//****************************************************************************/
#ifndef __JOBJCAST_H__
#define __JOBJCAST_H__

#include "jmetaclass.h"

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

#endif // __JOBJCAST_H__


