//****************************************************************************/
//  File:	VecN.h
//  Desc:	Variable-sized vector
/*	Author:	Ruslan Shestopalyuk
/*	Date:	17-03-2005
//****************************************************************************/
#ifndef __VECN_H__
#define __VECN_H__

typedef double real;
const real c_REpsilon = 1e-20;

class MatMN;
//****************************************************************************/
//  Class:  VecN
//  Desc:   Variable-sized vector
//****************************************************************************/
class VecN
{
    int         m_Size;
    real*       m_Buf;
    bool        m_bOwnBuf;
public:

    VecN() : m_Size(0), m_Buf(0), m_bOwnBuf(true) {}
    VecN( real* pBuf, int n ) : m_Size(n), m_Buf(pBuf), m_bOwnBuf(false) {}

    ~VecN() 
    { 
        if (m_bOwnBuf) delete []m_Buf;
    }
    inline int      Size() const                { return m_Size; }
    inline real     operator[]( int idx ) const { assert( idx >= 0 && idx < m_Size ); return m_Buf[idx]; }
    inline real&    operator[]( int idx )       { assert( idx >= 0 && idx < m_Size ); return m_Buf[idx]; }
    
    real*           GetBuf  () { return m_Buf; }
    const real*     GetBuf  () const { return m_Buf; }

    void            SetBuf          ( real* pBuf, int n )
    {
        m_Size      = n;
        m_Buf       = pBuf;
        m_bOwnBuf   = false;
    }

    void            Resize  ( int Size );
    void            Random  ( real minV = -1.0f, real maxV = 1.0f );
    real            Norm2   () const;
    real            Dot     ( const VecN& v );
    void            Zero    ();
    void            Clamp   ( real minv, real maxv );
    real            Norm    () const { return sqrt( Norm2() ); }
    void            Add     ( const VecN& v, real w );    
    void            Mul     ( const MatMN& m, const VecN& v );
    void            Mul     ( const VecN& v, const MatMN& m );

    void            operator /=( real v );
    void            operator *=( real v );
    void            operator -=( const VecN& v );
    void            operator +=( const VecN& v );
    void            operator *=( const VecN& v );
    void            operator = ( const VecN& v );
    
    friend class    MatMN;
}; // class VecN

#endif // __VECN_H__
