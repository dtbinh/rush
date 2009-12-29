//****************************************************************************/
//  File:	VecN.cpp
//  Desc:	Variable-sized vector
/*	Author:	Ruslan Shestopalyuk
/*	Date:	17-03-2005
//****************************************************************************/
#include "stdafx.h"
#include "MatMN.h"
#include "VecN.h"

//****************************************************************************/
/*  VecN implementation
//****************************************************************************/
void VecN::Resize( int Size )
{
    if (m_Size == Size) return;
    if (m_bOwnBuf) delete []m_Buf;
    m_Buf       = new real[Size];
    m_Size      = Size;
    m_bOwnBuf   = true;
} // VecN::Resize

void VecN::Random( real minV, real maxV )
{
    for (int i = 0; i < m_Size; i++)
    {
        real v = ((real)rand())/((real)RAND_MAX);
        v = minV + v*(maxV - minV);
        m_Buf[i] = v;
    }
} // VecN::Random

real VecN::Norm2() const
{
    real v = 0.0;
    for (int i = 0; i < m_Size; i++) v += m_Buf[i]*m_Buf[i];
    return v;
} // VecN::Norm2

real VecN::Dot( const VecN& v )
{
    assert( m_Size == v.m_Size );
    real res = 0.0;
    for (int i = 0; i < m_Size; i++) res += m_Buf[i]*v.m_Buf[i];
    return res; 
} // VecN::Dot

void VecN::operator /=( real v )
{
    real inv = 1.0/v;
    for (int i = 0; i < m_Size; i++) m_Buf[i] *= inv;
} // VecN::operator /=

void VecN::operator *=( real v )
{
    for (int i = 0; i < m_Size; i++) m_Buf[i] *= v;

} // VecN::operator *=

void VecN::operator -=( const VecN& v )
{
    assert( m_Size == v.m_Size );
    for (int i = 0; i < m_Size; i++) m_Buf[i] -= v.m_Buf[i];
} // VecN::operator -=

void VecN::operator +=( const VecN& v )
{
    assert( m_Size == v.m_Size );
    for (int i = 0; i < m_Size; i++) m_Buf[i] += v.m_Buf[i];
} // VecN::operator +=

void VecN::operator *=( const VecN& v )
{
    assert( m_Size == v.m_Size );
    for (int i = 0; i < m_Size; i++) m_Buf[i] *= v.m_Buf[i];
} // VecN::operator *=

void VecN::operator =( const VecN& v )
{
    Resize( v.m_Size );
    memcpy( m_Buf, v.m_Buf, m_Size*sizeof(real) );
} // VecN::operator =

void VecN::Zero()
{
     for (int i = 0; i < m_Size; i++) m_Buf[i] = 0.0; 
} // VecN::Zero

void VecN::Add( const VecN& v, real w )
{
    assert( m_Size == v.m_Size );
    for (int i = 0; i < m_Size; i++) m_Buf[i] += w*v.m_Buf[i];
} // VecN::add

void VecN::Mul( const MatMN& m, const VecN& v )
{
    assert( m.m_NCols == v.Size() );
    Resize( m.m_NRows );
    for (int i = 0; i < m_Size; i++)
    {
        real s = 0.0;
        const real* pM = m.m_Buf + i*m.m_NCols;
        for (int j = 0; j < v.m_Size; j++)
        {
            s += v[j]*pM[j];
        }
        m_Buf[i] = s;
    }
} // VecN::Mul

void VecN::Mul( const VecN& v, const MatMN& m )
{
    assert( m.m_NRows == v.Size() );
    int sz = m.m_NCols; 
    int w  = v.m_Size;
    Resize( sz );
    const real* mv = m.m_Buf;
    for (int i = 0; i < sz; i++)
    {
        real s = 0.0;
        int nM = i;
        for (int j = 0; j < w; j++)
        {
            s  += v[j]*mv[nM];
            nM += sz;
        }
        m_Buf[i] = s;
    }
} // VecN::Mul

void VecN::Clamp( real minv, real maxv )
{
    for (int i = 0; i < m_Size; i++)
    {
        if (m_Buf[i] < minv) m_Buf[i] = minv;
        if (m_Buf[i] > maxv) m_Buf[i] = maxv;
    }
} // VecN::Clamp




