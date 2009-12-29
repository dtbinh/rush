/***********************************************************************************/
//  File:   Track.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TRACK_H__
#define __TRACK_H__

#include <vector>
#include <algorithm>
#include "mathconst.h"

/***********************************************************************************/
//  Class:  TrackTraits
//  Desc:   Default track value type traits
/***********************************************************************************/
template <class T>
struct TrackTraits
{
    static inline float defaultVal() 
    {
        return 0.0f;
    }

    static inline float norm( const T& val ) 
    {
        return _fabs( val );
    }

    static inline T interpolate( const T& a, const T& b, float t )
    {
        return a + (b - a)*t;
    }
}; // struct TrackTraits

typedef std::vector<float> FloatVec;
/***********************************************************************************/
//  Class:  Track
//  Desc:   
/***********************************************************************************/
template <class T, class Traits = TrackTraits<T> >
class Track
{
protected:
    typedef std::vector<T> TVec;

    FloatVec        m_KeyTime;
    TVec            m_KeyVal;

public:
    typedef T ValType;

    int         GetNKeys() const { return m_KeyTime.size(); }

    void        SetNKeys( int nKeys )
    {
        m_KeyTime.resize( nKeys );
        m_KeyVal.resize( nKeys );
    }
    void        Clear   ()
    {
        m_KeyTime.clear();
        m_KeyVal.clear();
    }
    void        AddKey  ( float keyTime, const T& keyVal ) 
    { 
        m_KeyTime.push_back( keyTime ); 
        m_KeyVal.push_back( keyVal ); 
    }

    void        DeleteKey ( int idx )
    {
        m_KeyTime.erase( m_KeyTime.begin() + idx );
        m_KeyVal.erase( m_KeyVal.begin() + idx );
    }

    const float*    GetTimes() const { return m_KeyTime.size() > 0 ? &m_KeyTime[0] : NULL; }
    const T*        GetValues() const { return m_KeyVal.size() > 0 ? &m_KeyVal[0] : NULL; }

    float       GetTime ( int keyIdx ) const { return m_KeyTime[keyIdx]; }
    T           GetValue( int keyIdx ) const { return m_KeyVal[keyIdx]; }
    void        SetKey  ( int idx, float keyTime, const T& keyVal )
    {
        m_KeyTime[idx] = keyTime;
        m_KeyVal[idx]  = keyVal;
    }

    bool operator ==( const Track& ramp ) const
    {
        if (GetNKeys() != ramp.GetNKeys()) return false;
        int nKeys = GetNKeys();
        for (int i = 0; i < nKeys; i++)
        {
            if (_fabs( GetTime( i ) - ramp.GetTime( i ) ) > c_FltEpsilon) 
            {
                return false;
            }
            if (Traits::norm( GetValue( i ) - ramp.GetValue( i ) ) > c_FltEpsilon) 
            {
                return false;
            }
        }
        return true;
    }

    T GetValue( float cTime ) const
    {
        if (m_KeyTime.size() == 0) return Traits::defaultVal();
        FloatVec::const_iterator it = std::lower_bound( m_KeyTime.begin(), m_KeyTime.end(), cTime );
        if (it == m_KeyTime.end()) 
        {
            return m_KeyVal.back();
        }
        if (it == m_KeyTime.begin()) 
        {
            return m_KeyVal[0];
        }
        int pos = it - m_KeyTime.begin();
        const float at = m_KeyTime[pos - 1];
        const float bt = m_KeyTime[pos];
        const T&    a  = m_KeyVal [pos - 1];
        const T&    b  = m_KeyVal [pos];
        return Traits::interpolate( a, b, (cTime - at)/(bt - at) ); 
    }

    bool IsConstant( float eps = c_FltEpsilon) const
    {
        int nKeys = m_KeyVal.size();
        for (int i = 1; i < nKeys; i++)
        {
            if (Traits::norm( m_KeyVal[i] - m_KeyVal[i - 1] ) > eps)
            {
                return false;
            }
        }
        return true;
    }

    void Flatten()
    {
        if (m_KeyVal.size() == 0) return;
        m_KeyVal.erase( m_KeyVal.begin() + 1, m_KeyVal.end() );
        m_KeyTime.erase( m_KeyTime.begin() + 1, m_KeyTime.end() );
    }

    void Reduce( float eps = c_FltEpsilon )
    {
        if (m_KeyTime.size() <= 1) return;
        int nRemoved = 1;
        while (nRemoved > 0 && m_KeyTime.size() > 2)
        {
            nRemoved    = 0;
            int curKey  = 0;
            while (curKey + 2 < m_KeyTime.size())
            {
                T inVal = Traits::interpolate( m_KeyVal[curKey], m_KeyVal[curKey + 2], 
                    (m_KeyTime[curKey + 1] - m_KeyTime[curKey])/(m_KeyTime[curKey + 2] - m_KeyTime[curKey]) );
                if (Traits::norm( inVal - m_KeyVal[curKey + 1] ) < eps)
                {
                    //  remove redundant key
                    m_KeyTime.erase( m_KeyTime.begin() + curKey + 1 );
                    m_KeyVal.erase( m_KeyVal.begin() + curKey + 1 );
                    nRemoved++;
                }
                else
                {
                    curKey++;
                }
            }
        }
        
        if (m_KeyTime.size() == 2 && 
            Traits::norm( m_KeyVal[1] - m_KeyVal[0] ) < eps)
        {
            Flatten();
        }
    }
}; // class Track

#endif //__Track_H__