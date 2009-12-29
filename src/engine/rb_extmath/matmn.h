//****************************************************************************/
//  File:	mMatMN.h
//  Desc:	Variable-sized matrix
/*	Author:	Ruslan Shestopalyuk
/*	Date:	17-03-2005
//****************************************************************************/
#ifndef __MATMN_H__
#define __MATMN_H__

typedef double real;
class VecN;
//****************************************************************************/
//  Class:  MatMN
//  Desc:   Variable-sized matrix
//****************************************************************************/
class MatMN
{
    int             m_NCols;
    int             m_NRows;
    real*           m_Buf;
    bool            m_bOwnBuf;
public:

    MatMN() : m_NCols(0), m_NRows(0), m_Buf(0), m_bOwnBuf(true) {}
    MatMN( real* pBuf, int w, int h) : m_NCols(w), m_NRows(h), m_Buf(pBuf), m_bOwnBuf(false) {}

    ~MatMN()
    {
        if (m_bOwnBuf) delete []m_Buf;
    }
 
    int             NRows           () const { return m_NRows; }
    int             NCols           () const { return m_NCols; }
    real*           GetBuf          () { return m_Buf; }
    void            Resize          ( int w, int h );
    void            Random          ( real minV = -1.0f, real maxV = 1.0f );
    void            Symmetric_random( real minV, real maxV );
    void            I               ();
    void            addI            ();
    void            Zero            ();
    void            AddUtV          ( const VecN& u, const VecN& v );
    void            VtV             ( const VecN& v );
    void            Mul             ( const MatMN& a, const MatMN& b );
    void            operator *=     ( real w ); 
    void            operator +=     ( const MatMN& m );
    void            operator -=     ( const MatMN& m );
    void            operator =      ( const MatMN& m );
    bool            Inverse         () { return GaussElimination(); }
    void            Transpose       ( const MatMN& m );
    void            Transpose       ();
    bool            PDInverse       ( const MatMN& m );
    bool            PseudoInverse   ( const MatMN& m );
    bool            IsPosDefinite   () const;

    void            SwapRows        ( int r1, int r2 );
    void            SwapCols        ( int c1, int c2 );
    bool            GetRow          ( int r, VecN& row );
    bool            GetCol          ( int c, VecN& col );
    bool            SetRow          ( int r, VecN& row );
    bool            SetCol          ( int c, VecN& col );
    void            Clamp           ( real minv, real maxv );

    void            SetBuf          ( real* pBuf, int w, int h ) 
    {
        m_NCols     = w;
        m_NRows     = h;
        m_Buf       = pBuf;
        m_bOwnBuf   = false;
    }

    inline real     operator()( int x, int y ) const
    { 
        assert( x >= 0 && y >= 0 && x < m_NCols && y <= m_NRows ); 
        return m_Buf[x + y*m_NCols]; 
    }


    inline real&    operator()( int x, int y ) 
    { 
        assert( x >= 0 && y >= 0 && x < m_NCols && y <= m_NRows ); 
        return m_Buf[x + y*m_NCols]; 
    }

    friend class VecN;

//protected:
    bool            FactorCholesky  ();
    void            SolveCholesky   ( const VecN& b );
    bool            GaussElimination();

}; // class MatMN

#include <malloc.h>

#pragma warning ( disable : 4311 )
#pragma warning ( disable : 4312 )

#define STACK_ALLOC(A,B) (A*)_alloca( B*sizeof( A ) );

#endif // __MMATMN_H__
