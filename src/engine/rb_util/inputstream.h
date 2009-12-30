//****************************************************************************/
//  File:  InputStream.h
//  Desc:  
//****************************************************************************/
#ifndef __INPUTSTREAM_H__
#define __INPUTSTREAM_H__

//****************************************************************************/
//  Class:  IStreamReader
//  Desc:   Abstract interface for the stream reader
//****************************************************************************/
class IStreamReader
{
public:
    virtual void    Close       () = 0;
    virtual int     ReadStream  ( void* buf, int nuint8_ts ) = 0;
    virtual bool    IsValid     () const = 0;
    virtual int     GetTotalSize() const = 0;
}; // class IStreamReader

//****************************************************************************/
//  Class:  InputStream
//  Desc:   
//****************************************************************************/
class InputStream
{
protected:
    int             m_TotalSize;
    int             m_Readuint8_ts;
    bool            m_bDeleteReader;
    IStreamReader*  m_pReader;

public:
    InputStream() :   
      m_TotalSize       ( 0 ), 
      m_Readuint8_ts       ( 0 ), 
      m_pReader         ( NULL ),
      m_bDeleteReader   ( false )
    {}

    ~InputStream() 
    {
        if (m_bDeleteReader)
        {
            delete m_pReader;
        }
    }

    void SetReader( IStreamReader* pReader, bool bDeleteReader )
    {
        m_pReader = pReader;
        m_bDeleteReader = bDeleteReader;
        if (pReader)
        {
            m_TotalSize = m_pReader->GetTotalSize();
        }
        else
        {
            m_TotalSize = 0;
        }
        m_Readuint8_ts = 0;
    }

    int Read( void* buf, int nuint8_ts ) 
    { 
        if (m_pReader == NULL) return 0;
        int nRead = m_pReader->ReadStream( buf, nuint8_ts ); 
        m_Readuint8_ts += nRead;
        return nRead;
    }

    int Rewind( int nuint8_ts ) 
    { 
        if (m_pReader == NULL) return 0;
        int nRead = m_pReader->ReadStream( NULL, nuint8_ts ); 
        m_Readuint8_ts += nRead;
        return nRead;
    }

    operator bool()
    {
        return  m_pReader && 
                m_pReader->IsValid() && 
                (m_Readuint8_ts < m_TotalSize);
    }

    template <class T> InputStream& operator >>( std::vector<T>& arr ) 
    { 
        int nV = 0;
        Read( &nV, sizeof( int ) );
        arr.resize( nV );
        for (int i = 0; i < nV; i++) 
        {
            operator>>( arr[i] );
        }
        return *this; 
    }

    InputStream& operator >>( const char* &val ) 
    { 
        const int c_BufSize = 2048;
        static char buf[c_BufSize];
        int nCh = 0;
        Read( &nCh, sizeof( int ) );
        if (nCh >= c_BufSize) nCh = c_BufSize;
        Read( (void*)buf, nCh );
        buf[nCh] = 0;
        val = buf;
        return *this; 
    }

    InputStream& operator >>( std::string& val ) 
    { 
        int nCh = 0;
        Read( &nCh, sizeof( int ) );
        val.resize( nCh );
        Read( (void*)val.c_str(), nCh );
        return *this; 
    }

    template <class T> InputStream& operator >>( T& val ) 
    { 
        Read( &val, sizeof( val ) ); 
        return *this; 
    }

    int     GetTotalSize() const { return m_TotalSize; }
    int     GetReaduint8_ts() const { return m_Readuint8_ts; }

    void Close() 
    { 
        assert( m_pReader );
        m_pReader->Close(); 
    }
}; // class InputStream

#endif // __INPUTSTREAM_H__


