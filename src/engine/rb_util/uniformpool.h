//****************************************************************************/
//  File:   UniformPool.h
/*  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __UNIFORMPOOL_H__
#define __UNIFORMPOOL_H__

//****************************************************************************/
//  Class:  UniformPoolPage
//  Desc:
//****************************************************************************/
template <class TElem, int PageSize>
struct UniformPoolPage
{
    typedef UniformPoolPage<TElem, PageSize> TPage;

    UniformPoolPage() : m_pNextPage( NULL ) {}
    TElem*  GetElem( int idx )
    {
        return &(((TElem*)m_Elem)[idx]);
    }

    void SetNextPage( TPage* pPage )
    {
        m_pNextPage = pPage;
    }

    TPage* GetNextPage() const
    {
        return m_pNextPage;
    }

private:

    uint8_t    m_Elem[PageSize*sizeof(TElem)];
    TPage*  m_pNextPage;
}; // class UniformPoolPage

const int c_DefaultPageSize = 128;
//****************************************************************************/
//  Class:  UniformPool
//  Desc:   Pooled memory allocator for elements of the same type.
/*          Has following properties:
/*           - resizable, grows PageSize elements at time
/*           - elements are never moved around since allocation
//****************************************************************************/
template <class TElem, int PageSize = c_DefaultPageSize>
class UniformPool
{
    typedef UniformPoolPage<TElem, PageSize> TPage;

    TPage*      m_pFirstPage;
    TElem*      m_pFirstFree;
    int         m_NElem;

public:
    UniformPool() : m_pFirstPage( NULL ), m_pFirstFree( NULL ), m_NElem(0)
    {
        assert( sizeof( TElem ) >= sizeof( TElem* ) &&
                PageSize > 1 );
    }

    ~UniformPool()
    {
        Clear();
    }
    inline int GetNElem() const { return m_NElem; }

    inline TElem* Allocate()
    {
        if (m_pFirstFree)
        {
            TElem* pFree = m_pFirstFree;
            m_pFirstFree = GetPtrAt( pFree );
            m_NElem++;
            new (pFree)TElem();
            return pFree;
        }
        //  allocate new page
        TPage* pPage = new TPage;
        pPage->SetNextPage( m_pFirstPage );
        m_pFirstPage = pPage;
        const int LastElem = PageSize - 1;
        for (int i = 1; i < LastElem; i++)
        {
            SetPtrAt( pPage->GetElem( i ), pPage->GetElem( i + 1 ) );
        }
        m_pFirstFree = pPage->GetElem( 1 );
        SetPtrAt( pPage->GetElem( LastElem ), NULL );
        m_NElem++;
        TElem* pFree = pPage->GetElem( 0 );
        new (pFree)TElem();
        return pFree;
    } // Allocate

    inline void Free( TElem* pElem )
    {
        if (pElem != NULL)
        {
            pElem->~TElem();
            SetPtrAt( pElem, m_pFirstFree );
            m_pFirstFree = pElem;
            m_NElem--;
        }
    } // Free

    void Clear()
    {
        TPage* pPage = m_pFirstPage;
        while (pPage)
        {
            TPage* pNextPage = pPage->GetNextPage();
            delete pPage;
            pPage = pNextPage;
        }
        m_pFirstPage = NULL;
        m_pFirstFree = NULL;
        m_NElem = 0;
    } // Clear

protected:
    inline TElem* GetPtrAt( TElem* pElem ) const
    {
        return *((TElem**)pElem);
    }

    inline void SetPtrAt( TElem* pElem, TElem* ptr )
    {
        *((TElem**)pElem) = ptr;
    }
}; // class UniformPool

//****************************************************************************/
//  Class:  up_allocator
//  Desc:   std compliant uniform pool allocator
//****************************************************************************/
template<class T>
class up_allocator
{
    static UniformPool<T>       s_Pool;
public:
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef value_type&         reference;
    typedef const value_type*   const_pointer;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef size_t              difference_type;

    template<class _Other>
    struct rebind
    {
        typedef up_allocator<_Other> other;
    };

    pointer address(reference _Val) const
        {   // return address of mutable _Val
        return (&_Val);
        }

    const_pointer address(const_reference _Val) const
        {   // return address of nonmutable _Val
        return (&_Val);
        }

    up_allocator()
        {   // construct default up_allocator (do nothing)
        }

    up_allocator(const up_allocator<T>&)
        {   // construct by copying (do nothing)
        }

    template<class _Other>
        up_allocator(const up_allocator<_Other>&)
        {   // construct from a related up_allocator (do nothing)
        }

    template<class _Other>
        up_allocator<T>& operator=(const up_allocator<_Other>&)
        {   // assign from a related up_allocator (do nothing)
        return (*this);
        }

    void deallocate(pointer _Ptr, size_type)
        {   // deallocate object at _Ptr, ignore size
            s_Pool.Free( _Ptr );
        }

    pointer allocate(size_type _Count)
        {   // allocate array of _Count elements
            assert( _Count == 1 );
            return s_Pool.Allocate();
        }

    pointer allocate(size_type _Count, const void*)
        {   // allocate array of _Count elements, ignore hint
        return (allocate(_Count));
        }

    void construct(pointer _Ptr, const T& _Val)
        {   // construct object at _Ptr with value _Val
        _Construct(_Ptr, _Val);
        }

    void destroy(pointer _Ptr)
        {   // destroy object at _Ptr
        _Destroy(_Ptr);
        }

    size_t max_size() const
        {   // estimate maximum array size
        size_t _Count = (size_t)(-1) / sizeof (T);
        return (0 < _Count ? _Count : 1);
        }
}; // up_allocator

template <class T1, class T2>
inline bool operator == ( const up_allocator<T1>&, const up_allocator<T2>& )
{
    return true;
}

template <class T1, class T2>
inline bool operator != ( const up_allocator<T1>&, const up_allocator<T2>& )
{
    return false;
}

#endif // __UNIFORMPOOL_H__
