//****************************************************************************/
//  File:	MatMN.cpp
//  Desc:	Variable-sized matrix
/*	Author:	Ruslan Shestopalyuk
/*	Date:	17-03-2005
//****************************************************************************/
#include "precompile.h"
#include "scalar.h"
#include "vecn.h"
#include "matmn.h"
#include <malloc.h>

//****************************************************************************/
/*  MatMN implementation
//****************************************************************************/
void MatMN::Resize( int w, int h )
{
    if (m_NCols*m_NRows == w*h)
    {
        m_NCols = w;
        m_NRows = h;
        return;
    }
    if (m_bOwnBuf) delete []m_Buf;
    m_Buf       = new real[w*h];
    m_NCols     = w;
    m_NRows     = h;
    m_bOwnBuf   = true;
} // MatMN::Resize

void MatMN::I()
{
    assert( m_NCols == m_NRows );
    real* pRes = m_Buf;
    for (int j = 0; j < m_NRows; j++)
    {
        for (int i = 0; i < m_NCols; i++)
        {
            pRes[i] = (i == j) ? 1.0 : 0.0;
        }
        pRes += m_NCols;
    }
} // MatMN::I

void MatMN::addI()
{
    assert( m_NCols == m_NRows );
    real* pRes = m_Buf;
    for (int j = 0; j < m_NRows; j++)
    {
        for (int i = 0; i < m_NCols; i++)
        {
            if (i == j) m_Buf[i] += 1.0;
        }
        pRes += m_NCols;
    }
} // MatMN::addI

void MatMN::VtV( const VecN& v )
{
    real* pRes = m_Buf;
    for (int j = 0; j < m_NRows; j++)
    {
        for (int i = 0; i < m_NCols; i++)
        {
            m_Buf[i] = v[i]*v[j];
        }
        pRes += m_NCols;
    }
} // MatMN::VtV

void MatMN::operator *=( real w )
{
    int nE = m_NRows*m_NCols;
    for (int i = 0; i < nE; i++) m_Buf[i] *= w;
} // MatMN::operator *=

void MatMN::operator +=( const MatMN& m )
{
    assert( m_NCols == m.m_NCols && m_NRows == m.m_NRows );
    int nE = m_NRows*m_NCols;
    for (int i = 0; i < nE; i++) m_Buf[i] += m.m_Buf[i];
} // MatMN::operator +=

void MatMN::operator -=( const MatMN& m )
{
    assert( m_NCols == m.m_NCols && m_NRows == m.m_NRows );
    int nE = m_NRows*m_NCols;
    for (int i = 0; i < nE; i++) m_Buf[i] -= m.m_Buf[i];
} // MatMN::operator -=

void MatMN::operator =( const MatMN& m )
{
    Resize( m.m_NCols, m.m_NRows );
    memcpy( m_Buf, m.m_Buf, m_NCols*m_NRows*sizeof( real ) );
} // MatMN::operator =

void MatMN::Mul( const MatMN& a, const MatMN& b )
{
    assert( b.m_NRows == a.m_NCols );
    Resize( b.m_NCols, a.m_NRows );
    real* pRes = m_Buf;
    const real* pB = b.m_Buf;
    for (int j = 0; j < m_NRows; j++)
    {
        for (int i = 0; i < m_NCols; i++)
        {
            real s = 0.0;
            const real* pA = a.m_Buf + j*a.m_NCols;
            int cB = i;
            for (int k = 0; k < a.m_NCols; k++)
            {
                s += pA[k]*pB[cB];
                cB += b.m_NCols;
            }
            pRes[i] = s;
        }
        pRes += m_NCols;
    }
} // MatMN::Mul

void MatMN::Random( real minV, real maxV )
{
    int sz = m_NCols*m_NRows;
    for (int i = 0; i < sz; i++)
    {
        real v = ((real)rand())/((real)RAND_MAX);
        v = minV + v*(maxV - minV);
        m_Buf[i] = v;
    }
} // MatMN::Random

void MatMN::Symmetric_random( real minV, real maxV )
{
    for (int i = 0; i < m_NRows; i++)
    {
        for (int j = 0; j < m_NCols; j++)
        {
            real v = ((real)rand())/((real)RAND_MAX);
            v = minV + v*(maxV - minV);
            m_Buf[i + j*m_NCols] = v;
            m_Buf[j + i*m_NCols] = v;
        }
    }
} // MatMN::Symmetric_random

void MatMN::AddUtV( const VecN& u, const VecN& v )
{
    assert( u.Size() == m_NRows && v.Size() == m_NCols );
    real* pB = m_Buf;
    for (int j = 0; j < m_NRows; j++)
    {
        for (int i = 0; i < m_NCols; i++)
        {
            pB[i] += v[i]*u[j];
        }
        pB += m_NCols;
    }
} // MatMN::AddUtV

bool MatMN::FactorCholesky()
{
    assert( m_NCols == m_NCols && m_NCols > 0 && m_Buf );

    int     n     = m_NCols;
    real*   recip = STACK_ALLOC( real, n );
    real*   aa    = m_Buf;
    real    sum;
    real*   a;
    real*   b;
    for (int i = 0; i < n; i++)
    {
        real* bb = m_Buf;
        real* cc = m_Buf + i*n;
        for (int j = 0; j < i; j++)
        {
            sum = *cc;
            a   = aa;
            b   = bb;
            for (int k = j; k; k--)
            {
                sum -= (*(a++))*(*(b++));
            }
            *cc = sum * recip[j];
            bb += n;
            cc++;
        }
        sum = *cc;
        a = aa;
        for (int k = i; k > 0; k--, a++)
        {
            sum -= (*a)*(*a);
        }
        if (sum <= 0.0) return false;
        *cc = sqrt( sum ) ;
        recip[i] = 1.0/(*cc);
        aa += n;
    }
    return true;
} // MatMN::FactorCholesky

void MatMN::SolveCholesky( const VecN& b )
{
    assert( m_NCols == m_NCols && m_NCols > 0 && m_Buf && b.m_Buf );
    int     n   = m_NCols;
    real*   pB  = b.m_Buf;
    real*   y   = STACK_ALLOC( real, n );
    real*   L   = m_Buf;

    for (int i = 0; i < n; i++)
    {
        real sum = 0;
        for (int k = 0; k < i; k++)
        {
            sum += L[i*n + k]*y[k];
        }
        y[i] = (pB[i] - sum)/L[i*n + i];
    }

    for (int i = n - 1; i >= 0; i--)
    {
        real sum = 0;
        for (int k = i + 1; k < n; k++)
        {
            sum += L[k*n + i]*pB[k];
        }
        pB[i] = (y[i] - sum)/L[i*n + i];
    }
}

bool MatMN::IsPosDefinite() const
{
    if (m_NCols != m_NRows || m_NCols == 0) return false;
    int n = m_NCols;
    MatMN m;
    real *M = STACK_ALLOC( real, n*n );
    memcpy( M, m_Buf, n*n*sizeof( real ) );
    m.m_Buf     = M;
    m.m_bOwnBuf = false;
    m.m_NCols   = n;
    m.m_NRows   = n;
    return m.FactorCholesky();
}

bool MatMN::PDInverse( const MatMN& m )
{
    assert( m.m_NRows == m.m_NCols );
    int n = m.m_NRows;
    Resize( n, n );
    assert( n > 0 );
    real* pL = STACK_ALLOC( real, n*n );
    real* px = STACK_ALLOC( real, n );
    memcpy( pL, m.m_Buf, n*n*sizeof( real ) );
    MatMN L( pL, n, n );
    VecN  x( px, n );
    if (!L.FactorCholesky()) return false;
    Zero();
    for (int i = 0; i < n; i++)
    {
        x.Zero();
        x[i] = 1.0;
        L.SolveCholesky( x );
        for (int j = 0; j < n; j++)
        {
            m_Buf[j*n + i] = x[j];
        }
    }
    return true;
}

void MatMN::Zero()
{
    int sz = m_NCols*m_NRows;
    for (int i = 0; i < sz; i++)
    {
        m_Buf[i] = 0.0;
    }
}

void MatMN::Transpose( const MatMN& m )
{
    Resize( m.m_NRows, m.m_NCols );
    const real* pSrc = m.m_Buf;
    for (int j = 0; j < m_NCols; j++)
    {
        int d = j;
        for (int i = 0; i < m_NRows; i++)
        {
            m_Buf[d] = pSrc[i];
            d += m_NCols;
        }
        pSrc += m_NRows;
    }
}

void MatMN::Transpose()
{
    int sz = m_NRows*m_NCols;
    if (sz == 0) return;
    real* pM = STACK_ALLOC( real, sz );
    memcpy( pM, m_Buf, sz*sizeof(real) );
    MatMN m( pM, m_NCols, m_NRows );
    Transpose( m );
}

bool MatMN::GaussElimination()
{
    if (m_NCols != m_NRows) return false;
    int n = m_NRows;

    int*    nRow = STACK_ALLOC( int, n );
    int*    nCol = STACK_ALLOC( int, n );
    bool*   bPiv = STACK_ALLOC( bool, n );
    real*   A    = m_Buf;

    for (int i = 0; i < n; i++) { bPiv[i] = false; }

    for (int k = 0; k < n; k++)
    {
        //  find maximal element (excluding already processed rows)
        real maxEl  = 0.0;
        int  maxC   = -1;
        int  maxR   = -1;
        int  pC     = 0;
        for (int j = 0; j < n; j++)
        {
            if (bPiv[j]) { pC += m_NCols; continue; }
            for (int i = 0; i < n; i++)
            {
                if (bPiv[i]) { pC++; continue; }
                real el = _fabs( A[pC++] );
                if (el > maxEl)
                {
                    maxEl = el; maxC = i; maxR = j;
                }
            }
        }

        //  matrix is singular
        if (maxEl < c_Epsilon)
        {
            return false;
        }

        //  exchange rows
        if (maxC != maxR)
        {
            int cA = maxC*m_NCols;
            int cB = maxR*m_NCols;
            for (int i = 0; i < n; i++)
            {
                real tmp  = A[cA];
                A[cA] = A[cB];
                A[cB] = tmp;

                cA++; cB++;
            }
        }

        bPiv[maxC]  = true;
        nCol[k]     = maxC;
        nRow[k]     = maxR;

        real invEl = 1.0/A[maxC + maxC*m_NCols];
        //  scale current row
        pC = n*maxC;
        A[maxC + maxC*m_NCols] = 1.0;
        for (int i = 0; i < n; i++)
        {
            A[pC++] *= invEl;
        }

        //  zero elements beneath pivot
        pC = 0;
        for (int j = 0; j < n; j++)
        {
            if (j == maxC) { pC += m_NCols; continue; }
            real tmp = A[j*m_NCols + maxC];
            int pmC  = maxC*m_NCols;
            A[j*m_NCols + maxC] = 0.0;
            for (int i = 0; i < n; i++)
            {
                A[pC]   -= tmp*A[pmC];
                pmC++; pC++;
            }
        }
    }

    for (int i = n - 1; i >= 0; i--)
    {
        if (nCol[i] == nRow[i]) continue;
        for (int j = 0; j < n; j++)
        {
            real& a = A[nRow[i] + j*m_NCols];
            real& b = A[nCol[i] + j*m_NCols];
            real tmp = a;
            a = b; b = tmp;
        }
    }

    return true;
}

void MatMN::SwapRows( int r1, int r2 )
{
    if (r1 < 0 || r2 < 0 || r1 >= m_NRows || r2 >= m_NRows) return;
    real* pR1 = m_Buf + r1*m_NCols;
    real* pR2 = m_Buf + r2*m_NCols;
    real tmp;
    for (int i = 0; i < m_NCols; i++)
    {
        tmp    = pR1[i];
        pR1[i] = pR2[i];
        pR2[i] = tmp;
    }
}

void MatMN::SwapCols( int c1, int c2 )
{
    if (c1 < 0 || c2 < 0 || c1 >= m_NCols || c2 >= m_NCols) return;
    real* pR1 = m_Buf + c1;
    real* pR2 = m_Buf + c2;
    real tmp;
    for (int i = 0; i < m_NRows; i++)
    {
        tmp  = *pR1;
        *pR1 = *pR2;
        *pR2 = tmp;
        pR1 += m_NCols;
        pR2 += m_NCols;
    }
}

bool MatMN::GetRow( int r, VecN& row )
{
    if (r < 0 || r >= m_NRows) return false;
    row.Resize( m_NCols );
    real* pR = m_Buf + r*m_NCols;
    for (int i = 0; i < m_NCols; i++) row[i] = pR[i];
    return true;
}

bool MatMN::GetCol( int c, VecN& col )
{
    if (c < 0 || c >= m_NCols) return false;
    col.Resize( m_NRows );
    real* pR = m_Buf + c;
    for (int i = 0; i < m_NRows; i++) { col[i] = *pR; pR += m_NCols; }
    return true;
}

bool MatMN::SetRow( int r, VecN& row )
{
    if (r < 0 || r >= m_NRows) return false;
    row.Resize( m_NCols );
    real* pR = m_Buf + r*m_NCols;
    for (int i = 0; i < m_NCols; i++) pR[i] = row[i];
    return true;
}

bool MatMN::SetCol( int c, VecN& col )
{
    if (c < 0 || c >= m_NCols) return false;
    col.Resize( m_NRows );
    real* pR = m_Buf + c;
    for (int i = 0; i < m_NRows; i++) { *pR = col[i]; pR += m_NCols; }
    return true;
}

bool MatMN::PseudoInverse( const MatMN& m )
{
    int   w  = m.m_NCols;
    int   h  = m.m_NRows;
    real* pM = m.m_Buf;

    //  if there are more rows than columns, transpose source matrix
    //  - this tends to reduce errors
    bool bTranspose = (h > w);
    if (bTranspose)
    {
        pM = STACK_ALLOC( real, w*h );
        MatMN mt( pM, w, h );
        mt.Transpose( m );
        w = m.m_NRows;
        h = m.m_NCols;
    }
    Resize( w, h );

    real* pV = STACK_ALLOC( real, h );
    VecN r( pV, w );

    real* pB = m_Buf;
    real* pA = pM;

    VecN a( pA, w );
    VecN b( pB, w );

    //  0th column
    b = a;
    b /= a.Norm2();

    for (int i = 1; i < h; i++)
    {
        pA += w; pB += w;
        a.m_Buf = pA;
        b.m_Buf = pB;
        r.m_Size = i;

        MatMN A( pM, w, i );
        MatMN B( m_Buf, w, i );

        //  r = (I - AA+)a
        r.Mul( B, a );
        b.Mul( r, A );
        b *= -1.0;
        b += a;

        real norm = a.Dot( b );

        const real c_GrevilleEpsilon = 1e-08;
        if (norm > c_GrevilleEpsilon)
        {
            b /= b.Norm2();
            r *= -1.0;
            B.AddUtV( r, b );
        }
        else
        {
            //  r = A+A+'a
            b.Mul( r, B );

            real den = 1.0 + r.Norm2();
            b /= den;
            r *= -1.0;
            B.AddUtV( r, b );
        }
    }

    if (!bTranspose) Transpose();

    return true;
}

void MatMN::Clamp( real minv, real maxv )
{
    int sz = m_NRows*m_NCols;
    for (int i = 0; i < sz; i++)
    {
        if (m_Buf[i] < minv) m_Buf[i] = minv;
        if (m_Buf[i] > maxv) m_Buf[i] = maxv;
    }
}






