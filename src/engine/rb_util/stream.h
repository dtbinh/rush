//****************************************************************************/
//  File:  Stream.h
//  Desc:  
//****************************************************************************/
#ifndef __STREAM_H__
#define __STREAM_H__

const int c_OutStreamBufferSize = 4096;
//****************************************************************************/
//  Class:  OutStream
//  Desc:   
//****************************************************************************/
class OutStream
{
    BYTE        m_Buf[c_OutStreamBufferSize];
    int         m_NTotalBytes;

protected:
    int         m_NBytes;

public:
                OutStream() : m_NBytes(0), m_NTotalBytes(0) {}
    virtual     ~OutStream();
    
    void        Close() { FlushStream(); OnClose(); }
    void        FlushStream()
    {
        Flush( m_Buf, m_NBytes );
        m_NTotalBytes += m_NBytes;
        m_NBytes = 0;
    }

    int GetTotalSize() const { return m_NTotalBytes; }

    void Write( const BYTE* buf, int nBytes ) 
    { 
        if (nBytes == 0) return;
        if (m_NBytes + nBytes > c_OutStreamBufferSize) FlushStream();
        if (nBytes > c_OutStreamBufferSize)
        {
            if (m_NBytes > 0) Flush( m_Buf, m_NBytes );
            Flush( buf, nBytes );
            m_NTotalBytes += nBytes + m_NBytes;
            m_NBytes = 0;
            return;
        }
        memcpy( &(m_Buf[m_NBytes]), buf, nBytes );
        m_NBytes        += nBytes;
    } // Write

    template <class T> OutStream& operator <<( const std::vector<T>& arr ) 
    { 
        int nV = arr.size();
        Write( (BYTE*)&nV, sizeof( int ) );
        for (int i = 0; i < nV; i++) operator<<( arr[i] );
        return *this; 
    }

    OutStream& operator <<( const char* val ) 
    { 
        int nCh = strlen( val );
        Write( (BYTE*)&nCh, sizeof( int ) );
        Write( (BYTE*)val, nCh );
        return *this; 
    }

    OutStream& operator <<( const std::string& val ) 
    { 
        int nCh = val.size();
        Write( (BYTE*)&nCh, sizeof( int ) );
        Write( (BYTE*)val.c_str(), nCh );
        return *this; 
    }
    
    operator bool() { return IsValid(); }
    template <class T> OutStream& operator <<( const T& val ) 
    { 
        Write( (BYTE*)&val, sizeof( val ) ); 
        return *this; 
    }


private:
    virtual void        OnClose     () {}
    virtual int         Flush       ( const void* buf, int nBytes ) { return 0; }
    virtual bool        IsValid     () const { return false; } 
}; // class OutStream

//****************************************************************************/
//  Class:  InStream
//  Desc:   
//****************************************************************************/
class InStream
{
protected:
    int         m_TotalSize;
    int         m_ReadBytes;

public:
    InStream() : m_TotalSize(0), m_ReadBytes(0) {}

    bool Read( void* buf, int nBytes ) 
    { 
        const int nRead = ReadStream( buf, nBytes );
        m_ReadBytes += nRead;
        return (nRead == nBytes);
    }

    bool Rewind( int nBytes ) 
    { 
        const int nRead = ReadStream( NULL, nBytes );
        m_ReadBytes += nRead; 
        return (nRead == nBytes);
    }

    operator bool()
    {
        return IsValid() && (m_ReadBytes < m_TotalSize);
    }

    template <class T> InStream& operator >>( std::vector<T>& arr ) 
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

    InStream& operator >>( const char* &val ) 
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

    InStream& operator >>( std::string& val ) 
    { 
        int nCh = 0;
        Read( &nCh, sizeof( int ) );
        val.resize( nCh );
        Read( (void*)val.c_str(), nCh );
        return *this; 
    }

    template <class T> InStream& operator >>( T& val ) 
    { 
        Read( &val, sizeof( val ) ); 
        return *this; 
    }

    int     GetTotalSize() const { return m_TotalSize; }
    int     GetReadBytes() const { return m_ReadBytes; }

    virtual void        Close  () {}
    virtual const char* GetName() const { return ""; }

private:
    virtual int     ReadStream  ( void* buf, int nBytes ) { return 0; }
    virtual bool    IsValid     () const { return false; } 

}; // class InStream



#endif // __STREAM_H__


