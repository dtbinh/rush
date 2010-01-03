//****************************************************************************/
//  File:  JWeakRef.h
//****************************************************************************/
#ifndef __JWEAKREF_H__
#define __JWEAKREF_H__

#include "jweakrefreg.h"

//****************************************************************************/
//  Class:  JWeakRefHolder
//  Desc:
//****************************************************************************/
class JWeakRefHolder
{
public:
                JWeakRefHolder();

    void*       GetPtr  () const;
    void        Release ();
    void        AddRef  ();

private:
    void*       m_pPtr;
    int         m_NRef;

    friend class JWeakRefReg;
}; // class JWeakRefHolder

//****************************************************************************/
//  Class:  JWeakRef
//  Desc:   Weak-referencing pointer
//****************************************************************************/
template <class T>
class JWeakRef
{
public:
                JWeakRef    ( T* pObject = NULL );
                JWeakRef    ( const JWeakRef& ref );
                ~JWeakRef   ();
                operator T* () const;
    T&          operator *  ();
    bool        operator !  () const;
    T*          operator -> ();
    const T*    operator -> () const;
    JWeakRef&   operator =  ( const JWeakRef<T> &pObject );
    JWeakRef&   operator =  ( T* pObject );

private:
    JWeakRefHolder*      m_pHolder;
}; // class JWeakRef

//*****************************************************************************
//	JWeakRefHolder inlines
//*****************************************************************************
inline JWeakRefHolder::JWeakRefHolder()
    :   m_pPtr( NULL ),
        m_NRef( 0 )
{
}

inline void* JWeakRefHolder::GetPtr() const
{
    return m_pPtr;
}

inline void JWeakRefHolder::AddRef()
{
    m_NRef++;
}

inline void JWeakRefHolder::Release()
{
    m_NRef--;
    if (m_NRef <= 0)
    {
        JWeakRefReg::s_pInstance->DeleteHolder( this );
    }
}

//*****************************************************************************
//	JWeakRef inlines
//*****************************************************************************
template <class T>
inline JWeakRef<T>::JWeakRef( T* pObject = NULL )
{
    m_pHolder = JWeakRefReg::s_pInstance->GetHolder( pObject );
    m_pHolder->AddRef();
}

template <class T>
inline JWeakRef<T>::JWeakRef( const JWeakRef& ref )
{
    m_pHolder = ref.m_pHolder;
    m_pHolder->AddRef();
}

template <class T>
inline JWeakRef<T>::~JWeakRef()
{
    m_pHolder->Release();
}

template <class T>
inline JWeakRef<T>::operator T*() const
{
    return ((T*)(m_pHolder->GetPtr()));
}

template <class T>
inline T& JWeakRef<T>::operator *()
{
    return *((T*)(m_pHolder->GetPtr()));
}

template <class T>
inline bool JWeakRef<T>::operator !() const
{
    return (m_pHolder->GetPtr() == NULL);
}

template <class T>
inline T* JWeakRef<T>::operator ->()
{
    return ((T*)(m_pHolder->GetPtr()));
}

template <class T>
inline const T* JWeakRef<T>::operator ->() const
{
    return ((T*)(m_pHolder->GetPtr()));
}

template <class T>
inline JWeakRef<T>& JWeakRef<T>::operator = ( const JWeakRef<T> &pObject )
{
    return operator = ((T*) pObject.m_pHolder->GetPtr());
}

template <class T>
inline JWeakRef<T>& JWeakRef<T>::operator = ( T* pObject )
{
    if (m_pHolder->GetPtr() == pObject)
    {
        return *this;
    }

    JWeakRefHolder* pNewHolder = JWeakRefReg::s_pInstance->GetHolder( pObject );

    pNewHolder->AddRef();
    m_pHolder->Release();
    m_pHolder = pNewHolder;

    return *this;
}

#endif // __JWEAKREF_H__


