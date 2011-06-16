//****************************************************************************/
//  File:   RegPool.h
/*  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __REGPOOL_H__
#define __REGPOOL_H__

#include "uniformpool.h"

//****************************************************************************/
//  Class:  RegPool
//  Desc:
//****************************************************************************/
template <class TElem>
class RegPool
{
    typedef UniformPool<TElem>              PoolType;
    typedef hash_map_t<int, TElem*>         RegType;

    PoolType    m_Pool;
    RegType     m_Reg;
    int         m_LastID;

public:
    RegPool()
    {
        m_LastID = 0;
    }

    ~RegPool()
    {
    }

    int reserve_ids( int nID )
    {
        int cID = m_LastID;
        m_LastID += nID;
        return cID;
    }

    int alloc()
    {
        int id = m_LastID;
        m_LastID++;
        m_Reg[id] = m_Pool.Allocate();
        return id;
    }

    int size() const
    {
        return m_Pool.GetNElem();
    }

    bool remove( int id )
    {
        typename RegType::iterator it = m_Reg.find( id );
        if (it == m_Reg.end()) return false;
        m_Pool.Free( (*it).second );
        m_Reg.erase( it );
        return true;
    }

    TElem*  operator[]( int id )
    {
        typename RegType::iterator it = m_Reg.find( id );
        if (it == m_Reg.end()) return NULL;
        return (*it).second;
    }

    void clear()
    {
        m_Pool.Clear();
        m_Reg.clear();
    }


    class iterator
    {
        typename RegType::iterator it;
    public:
        iterator( typename RegType::iterator _it )
        {
            it = _it;
        }

        int id() const
        {
            return (*it).first;
        }

        TElem* operator *() const
        {
            return (*it).second;
        }

        bool operator == ( const iterator& _it ) const
        {
            return (it == _it.it);
        }

        bool operator != ( const iterator& _it ) const
        {
            return !(it == _it.it);
        }

        void operator ++()
        {
            ++it;
        }

        void operator ++( int t )
        {
            ++it;
        }
    }; // class iterator

    iterator begin()
    {
        return iterator( m_Reg.begin() );
    }

    iterator end()
    {
        return iterator( m_Reg.end() );
    }
}; // class RegPool

#endif // __REGPOOL_H__
