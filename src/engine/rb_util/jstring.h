//****************************************************************************/
//  File:  JString.h
//****************************************************************************/
#ifndef __JSTRING_H__
#define __JSTRING_H__

#include <stdio.h>
#include <string.h>
#include <vector>

inline int ToLower( int c )
{
    if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
    return c;
} // ToLower

const int c_ExpectedMaxStringLen = 31;
//****************************************************************************/
//  Class:  JString
//  Desc:   String value wrapper, used as hashing key
/*          Also it keeps short names locally
//****************************************************************************/
class JString
{
    char        m_Pool[c_ExpectedMaxStringLen + 1];
    char*       m_pBuf;
    int         m_Size;

public:
    JString() : m_pBuf( m_Pool ), m_Size( 0 ) { m_Pool[0] = 0; }
    JString( const char* pStr, int nChar = -1 )
    {
        m_pBuf    = m_Pool;
        m_Size    = 0;
        m_Pool[0] = 0;
        set( pStr, nChar );
    }

    JString( const JString& key )
    {
        m_pBuf    = m_Pool;
        m_Size    = 0;
        m_Pool[0] = 0;
        set( key.m_pBuf );
    }

    ~JString()
    {
        if (m_pBuf != m_Pool)
        {
            delete []m_pBuf;
            m_pBuf = m_Pool;
        }
    }

    operator char*() const { return m_pBuf; }

    void clear()
    {
        if (m_pBuf != m_Pool)
        {
            delete []m_pBuf;
        }
        m_pBuf    = m_Pool;
        m_Size    = 0;
        m_Pool[0] = 0;
    }

    inline const char* c_str() const
    {
        return m_pBuf;
    }

    inline int size() const
    {
        return m_Size;
    }

    void resize( int nChar )
    {
        if (m_Size == nChar) return;
        m_Size = nChar;
        if (m_Size > c_ExpectedMaxStringLen)
        {
            m_pBuf = new char[m_Size + 1];
        }
        else
        {
            m_pBuf = m_Pool;
        }
        m_pBuf[m_Size] = 0;
    }

    void set( const char* pStr, int nChar = -1 )
    {
        if (m_pBuf != m_Pool)
        {
            delete []m_pBuf;
            m_pBuf    = m_Pool;
            m_Size    = 0;
            m_pBuf[0] = 0;
        }
        if (!pStr)
        {
            m_pBuf    = m_Pool;
            m_Size    = 0;
            m_pBuf[0] = 0;
            return;
        }
        if (nChar == -1)
        {
            nChar = strlen( pStr );
        }
        resize( nChar );
        if (nChar > 0)
        {
            strncpy( m_pBuf, pStr, nChar );
            m_pBuf[nChar] = 0;
        }
    }

    void operator += ( const JString& str )
    {
        operator +=( str.c_str() );
    }

    void operator += ( const char* text )
    {
        if (!text || text[0] == 0)
        {
            return;
        }

        int addSize = strlen( text );
        int newSize = m_Size + addSize;
        if (newSize > c_ExpectedMaxStringLen)
        {
            char* pNewBuf = new char[newSize + 1];
            memcpy( pNewBuf, m_pBuf, m_Size );
            if (m_pBuf != m_Pool)
            {
                delete []m_pBuf;
            }
            m_pBuf = pNewBuf;
        }

        memcpy( m_pBuf + m_Size, text, addSize );
        m_Size = newSize;
        m_pBuf[m_Size] = 0;
    }

    void to_lower()
    {
        for (int i = 0; i < m_Size; i++)
        {
            m_pBuf[i] = ToLower( m_pBuf[i] );
        }
    }

    inline void operator =( const JString& str )
    {
        set( str.m_pBuf );
    }

    inline void operator =( const char* text )
    {
        set( text );
    }

    inline bool operator ==( const char* str ) const
    {
        if (str == m_pBuf) return true;
        if (!m_pBuf || !str) return false;
        const char* pStr1 = m_pBuf;
        if (!m_pBuf) return true;
        int nChar = m_Size;

        int i = 0;
        for (; i < nChar, str[i] != 0; i++)
        {
            if (pStr1[i] != str[i]) return false;
        }
        return (str[i] == pStr1[i]);
    }

    inline bool is_equal_ci( const char* str ) const
    {
        if (str == m_pBuf) return true;
        if (!m_pBuf || !str) return false;
        const char* pStr1 = m_pBuf;
        if (!m_pBuf) return true;
        int nChar = m_Size;

        int i = 0;
        for (; i < nChar, str[i] != 0; i++)
        {
            if (ToLower( pStr1[i] ) != ToLower( str[i] )) return false;
        }
        return (str[i] == pStr1[i]);
    }

    void insert( int pos, const char* substr )
    {
        int nNewChar = strlen( substr );
        if (nNewChar == 0) return;
        if (pos < 0) pos = 0;
        if (pos > m_Size) pos = m_Size;
        JString temp;
        temp.set( c_str(), pos );
        temp += substr;
        temp += c_str() + pos;
        *this = temp;
    }

    void erase( int pos, int nChar )
    {
        JString temp;
        if (pos < 0)
        {
            nChar += pos;
            pos = 0;
        }
        if (pos + nChar > m_Size)
        {
            nChar = m_Size - pos;
        }
        if (nChar <= 0) return;
        if (pos > 0) temp.set( c_str(), pos );
        if (pos + nChar > m_Size)
        {
            nChar = m_Size - pos;
        }
        else
        {
            temp += c_str() + pos + nChar;
        }
        set( temp.c_str() );
    }

    inline bool operator ==( const JString& key ) const
    {
        return operator ==( key.m_pBuf );
    }

    inline bool operator !=( const JString& key ) const
    {
        return !(operator ==( key.m_pBuf ));
    }

    inline bool operator !=( const char* str ) const
    {
        return !(operator ==( str ));
    }

    //  hash function
    operator size_t() const
    {
        if (!m_pBuf) return 0;
        unsigned long hash = 5381;
        for (int i = 0; i < m_Size; i++)
        {
            hash = ((hash << 5) + hash) + m_pBuf[i];
        }
        return hash;
    } // operator size_t

    inline bool operator <( const JString& key ) const
    {
        if (!m_pBuf) return true;
        if (!key.m_pBuf) return false;
        const char* pStr1 = m_pBuf;
        const char* pStr2 = key.m_pBuf;
        int nChar = m_Size < key.m_Size ? m_Size : key.m_Size;

        for (int i = 0; i < nChar; i++)
        {
            if (pStr1[i] < pStr2[i]) return true;
            if (pStr1[i] > pStr2[i]) return false;
        }
        if (nChar < key.m_Size) return true;
        return false;
    }

private:
    JString& operator +( const JString& rhs );
}; // class JString

typedef std::vector<JString> JStringList;

inline JString ToString( int val )
{
    char buf[64];
    sprintf( buf, "%d", val );
    return JString( buf );
}


#endif // __JSTRING_H__


