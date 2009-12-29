//****************************************************************************/
//  File:  Rand.h
//  Desc:  random values
//****************************************************************************/
#ifndef __RAND_H__
#define __RAND_H__

const uint32_t c_RandMax = 0x7fff;
//****************************************************************************/
//  Class:  RandomGen
//  Desc:   Autonomous uniform number generator. Works the exact way as
/*           CRT rand()
//****************************************************************************/
class RandomGen
{
    uint32_t       m_CurVal;
public:

    RandomGen(){ Reset( 1L ); }

    uint32_t       GetValue( uint32_t minv, uint32_t maxv )
    {
        assert( maxv > minv );
        return minv + RandVal()%(maxv - minv);
    }
    uint32_t       GetValue( uint32_t maxv )
    {
        return RandVal()%maxv;
    }

    float       GetValue( float minv, float maxv )
    {
        return minv + (maxv - minv)*(float( RandVal() )/float( c_RandMax ));
    }

    void        Reset   ( uint32_t seed )
    {
        m_CurVal = seed;
        RandVal();
    }

protected:
    uint32_t       RandVal()
    {
        m_CurVal = m_CurVal*214013L + 2531011L;
        return ((m_CurVal >> 16)&0x7fff);
    }

}; // class RandomGen

inline float frand( float minv, float maxv )
{
    return minv + (maxv - minv)*(float( rand() )/float( RAND_MAX ));
} // frand

inline float frand()
{
    return (float( rand() )/float( RAND_MAX ));
} // frand

#endif // __RAND_H__

