//****************************************************************************/
//  File:  JPtr.h
//****************************************************************************/
#ifndef __JPTR_H__
#define __JPTR_H__

//****************************************************************************/
//  Class:  JPtr
//  Desc:   Smart pointer to some object of JObject-derived class  
//****************************************************************************/
template <class T>
class JPtr  
{
    T*      m_pObject;
public:
    JPtr( T* pObject = NULL ) : m_pObject( pObject ) 
    { 
        if (m_pObject)
        {
            m_pObject->AddRef(); 
        }
    }
    ~JPtr() 
    { 
        if (m_pObject)
        {
            m_pObject->Release(); 
        }
    }

    operator T*() 
    { 
        return m_pObject; 
    }

    T& operator *() 
    { 
        return *m_pObject; 
    }

    bool operator !() const
    { 
        return (m_pObject == NULL); 
    }

    T* operator ->() 
    { 
        return m_pObject; 
    }

    const T* operator ->() const
    { 
        return m_pObject; 
    }

    JPtr& operator = ( JPtr<T> &pObject )
    {
        return operator = ((T*) pObject);
    }

    JPtr& operator = ( T* pObject ) 
    {
        if (m_pObject)
        {
            m_pObject->Release(); 
        }
        m_pObject = pObject; 

        if (m_pObject)
        {
            m_pObject->AddRef(); 
        }
        return *this;
    }

}; // class JPtr

#endif // __JPTR_H__


