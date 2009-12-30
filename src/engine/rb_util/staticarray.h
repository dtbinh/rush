//****************************************************************************/
//  File:	StaticArray.h
//  Desc:	Static array template class
/*	Author:	Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __STATICARRAY_H__
#define __STATICARRAY_H__

//****************************************************************************/
//  Class:	static_array
//  Desc:	Static linear unordered array 
//****************************************************************************/
template <class TElem, int MaxSz>
class static_array
{
public:
	static_array( int nElem = 0 ) : m_NElem(nElem) {}
	static_array( const static_array<TElem, MaxSz>& arr ) 
    { 
        m_NElem = arr.m_NElem;
        memcpy( m_Buf, arr.m_Buf, m_NElem * sizeof(TElem) ); 
    }

	~static_array() 
    { 
        m_NElem = 0; 
    }

	TElem&	operator[]( int idx ) 
    { 
        assert( idx >= 0 && idx < m_NElem ); 
        return m_Buf[idx]; 
    }

	const TElem& operator[]( int idx ) const 
    { 
        assert( idx >= 0 && idx < m_NElem ); 
        return m_Buf[idx]; 
    }

	const TElem& at     ( int idx ) const 
    { 
        assert( idx >= 0 && idx < m_NElem ); 
        return m_Buf[idx]; 
    }

	int     size_uint8_ts  () const 
    { 
        return MaxSz * sizeof( TElem ) + sizeof( *this ); 
    }

	int	    size        () const { return m_NElem; }
    bool    resize      ( int sz ) 
    { 
        if (sz > MaxSz) return false; 
        m_NElem = sz; 
        return true; 
    }

	TElem&  push_back   ( const TElem& el ) 
    { 
        assert(m_NElem < MaxSz); 
        m_Buf[m_NElem] = el; 
        m_NElem++;
        return m_Buf[m_NElem - 1]; 
    }

    TElem&  back        () 
    { 
        assert( m_NElem > 0); 
        return m_Buf[m_NElem - 1]; 
    }
    
    TElem&  pop         () 
    { 
        assert( m_NElem > 0); 
        m_NElem--; 
        return m_Buf[m_NElem]; 
    }

    TElem&  expand      () 
    { 
        assert( m_NElem < MaxSz ); 
        m_NElem++; 
        return m_Buf[m_NElem - 1]; 
    }

    void    erase       ( int elIdx ) 
    { 
        assert( elIdx >= 0 && elIdx < m_NElem ); 
        m_NElem--; 
        if (elIdx < m_NElem)
        {
            //  move the last element to the gap
            m_Buf[elIdx] = m_Buf[m_NElem]; 
        }
    }
	int     capacity    () const { return MaxSz; }
	void    clear       ()       { m_NElem = 0; }
    bool    full        () const { return (m_NElem >= MaxSz); }

    //****************************************************************************/
    //  Class:  static_array::iterator
    //  Desc:   STL-compatible static_array iterator
    //****************************************************************************/
    class iterator
    {
        TElem*      m_Ptr;      //  current array position
    public:
                    iterator    () : m_Ptr(NULL){}
                    iterator    ( TElem* ptr ) : m_Ptr( ptr ){}
        TElem&      operator*   () const { return *m_Ptr; }
        TElem*      operator->  () const    {	return (&*m_Ptr); }
        iterator&   operator++  ()          { m_Ptr++; return (*this); }
        iterator    operator++  ( int )     { iterator tmp = *this; m_Ptr++; return tmp; }
        iterator&   operator--  ()          { m_Ptr--; return (*this); }
        iterator    operator--  ( int )     { iterator tmp = *this; m_Ptr--; return tmp; }
        iterator&   operator+=  ( int diff ) { m_Ptr += diff; return (*this); }
        iterator    operator+   ( int diff ) const { iterator tmp = *this; return (tmp += diff); }
        iterator&   operator-=  ( int diff ) { m_Ptr -= diff; return (*this); }
        iterator    operator-   ( int diff ) const { iterator tmp = *this; return (tmp -= diff); }
        int         operator-   ( const iterator& r ) const { return (m_Ptr - r.m_Ptr); }
        TElem&      operator[]  ( int diff ) const { return (*(*this + diff)); }
        bool        operator==  ( const iterator& r ) const { return (m_Ptr == r.m_Ptr); }
        bool        operator!=  ( const iterator& r ) const { return (m_Ptr != r.m_Ptr); }
        bool        operator<   ( const iterator& r ) const { return (m_Ptr < r.m_Ptr); }
        bool        operator>   ( const iterator& r ) const { return (m_Ptr > r.m_Ptr); }
        bool        operator<=  ( const iterator& r ) const { return (m_Ptr <= r.m_Ptr); }
        bool        operator>=  ( const iterator& r ) const { return (m_Ptr >= r.m_Ptr); }
        friend iterator operator+( int diff, const iterator& r ) {	return (r + diff); }
    
        typedef typename std::random_access_iterator_tag iterator_category;
        typedef typename TElem          value_type;
        typedef typename int            difference_type;
        typedef typename TElem*         pointer;
        typedef typename TElem&         reference;
    }; // class iterator

    iterator begin  () { return iterator( &m_Buf[0] ); }
    iterator end    () { return iterator( &m_Buf[m_NElem] ); }

private:
	TElem		m_Buf[MaxSz + 1];
	int			m_NElem;
}; // class static_array


#endif // __STATICARRAY_H__