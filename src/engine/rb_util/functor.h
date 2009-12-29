//****************************************************************************/
//  File:  Functor.h
//****************************************************************************/
#ifndef __FUNCTOR_H__
#define __FUNCTOR_H__


//****************************************************************************/
//  Class:  Functor
//  Desc:    
//****************************************************************************/
class Functor
{
protected:
    void*       m_pObj;
    DWORD       m_pFunc;
public:
    Functor() : m_pObj(NULL), m_pFunc(0) {}
    virtual void operator()() {}
    
    static Functor  null;
}; // class Functor

template <class T>
class TFunctor : public Functor
{
    typedef void (T::*TFunc)();
public:
    TFunctor( T* pObj, TFunc pFunc ) 
    {
        m_pObj = (void*)pObj;
        *((TFunc*)&m_pFunc) = pFunc;
    }
    virtual void operator()() 
    {
        if (!m_pObj || !m_pFunc) return;
        TFunc f = *((TFunc*)&m_pFunc);
        (reinterpret_cast<T*>( m_pObj )->*f)();

    }
}; // class TFunctor

#endif // __FUNCTOR_H__


