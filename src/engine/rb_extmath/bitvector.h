//****************************************************************************/
//  File:  BitVector.h
//  Desc:  
//****************************************************************************/
#ifndef __BITVECTOR_H__
#define __BITVECTOR_H__
#include "BitOps.h"

//****************************************************************************/
//  Class:  BitVector
//****************************************************************************/
template <int Size>
class BitVector
{
    static const int c_NumDW = Size/32 + 1;

    uint32_t m_DW[c_NumDW];

public:
    BitVector() {}

    inline bool operator ==( const BitVector<Size>& bb ) const
    {
        for (int i = 0; i < c_NumDW; i++)
        {
            if (m_DW[i] != bb.m_DW[i]) return false;
        } 
        return true;
    } // operator ==

    inline int GetSize() const { return Size; }

    inline void Clear() 
    { 
        memset( &m_DW, 0, sizeof( uint32_t )*c_NumDW ); 
    }

    inline void SetBit( int idx ) 
    { 
        assert( idx/32 >= 0 && idx/32 < c_NumDW );
        m_DW[idx>>5] |= (0x1 << (idx&31)); 
    }
    
    inline void UnsetBit( int idx ) 
    { 
        m_DW[idx>>5] &= ~(0x1 << (idx&31)); 
    }

    inline bool GetBit( int idx ) const 
    { 
        return (m_DW[idx>>5] & (0x1 << (idx&31))) != 0; 
    }

    inline uint32_t GetNBits() const
    {
        uint32_t nB = 0;
        for (int i = 0; i < c_NumDW; i++) nB += CountBits( m_DW[i] );
        return nB;
    }
    
    inline bool IsEmpty() const
    {
        for (int i = 0; i < c_NumDW; i++) if (m_DW[i] != 0) return false;
        return true;
    }

    inline BitVector operator &( const BitVector &b ) const
    {
        BitVector res( *this );    
        for (int i = 0; i < c_NumDW; i++) res.m_DW[i] = m_DW[i] & b.m_DW[i]; 
        return res;
    }

    inline BitVector operator |( const BitVector &b ) const
    {
        BitVector res( *this );    
        for (int i = 0; i < c_NumDW; i++) res.m_DW[i] = m_DW[i] | b.m_DW[i];
        return res;
    }

    inline BitVector& operator |=( const BitVector &b )
    {
        for (int i = 0; i < c_NumDW; i++) m_DW[i] |= b.m_DW[i];
        return *this;
    }

    inline BitVector& operator &=( const BitVector &b )
    {
        for (int i = 0; i < c_NumDW; i++) m_DW[i] &= b.m_DW[i];
        return *this;
    }

    inline BitVector& operator ^=( const BitVector &b )
    {
        for (int i = 0; i < c_NumDW; i++) m_DW[i] ^= b.m_DW[i];
        return *this;
    }

    inline BitVector operator ^( const BitVector &b ) const
    {
        BitVector res( *this );    
        for (int i = 0; i < c_NumDW; i++) res.m_DW[i] = m_DW[i] ^ b.m_DW[i];
        return res;
    }

    inline BitVector operator ~() const
    {
        BitVector res( *this );    
        for (int i = 0; i < c_NumDW; i++) res.m_DW[i] = ~m_DW[i];
        return res;
    }

    inline BitVector operator =( const BitVector &b )
    {
        memcpy( m_DW, b.m_DW, sizeof( uint32_t )*c_NumDW );
        return *this;
    }

}; // class BitVector

#endif // __BITVECTOR_H__


