//****************************************************************************/
//  File:  Buffer.h
//  Desc:
//****************************************************************************/
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "stream.h"

//****************************************************************************/
//  Class:  Buffer
//  Desc:
//****************************************************************************/
class Buffer
{
public:
                Buffer      ();
                Buffer      ( int size );
                Buffer      ( const uint8_t* pData, int size );
                ~Buffer     ();

    const uint8_t* GetData     () const;
    uint8_t*       GetData     ();
    int         GetSize     () const;

    void        SetData     ( const uint8_t* pData, int size );
    void        AddData     ( const uint8_t* pData, int size );
    void        Clear       ();

    void        Write       ( OutStream& os ) const;
    bool        Read        ( InStream& is );
    void        Resize      ( int size );

private:
    uint8_t*       m_pData;
    int         m_Size;
}; // class Buffer

//****************************************************************************/
//  Buffer inlines
//****************************************************************************/
inline Buffer::Buffer()
    : m_pData(0), m_Size(0)
{}

inline Buffer::Buffer( int size )
{
    m_pData = NULL;
    Resize( size );
}

inline Buffer::Buffer( const uint8_t* pData, int size )
{
    m_pData = NULL;
    SetData( pData, size );
}

inline Buffer::~Buffer()
{
    delete []m_pData;
}

inline const uint8_t* Buffer::GetData() const
{
    return m_pData;
}

inline uint8_t* Buffer::GetData()
{
    return m_pData;
}

inline int Buffer::GetSize() const
{
    return m_Size;
}

inline void Buffer::SetData( const uint8_t* pData, int size )
{
    Resize( size );
    memcpy( m_pData, pData, size );
}

inline void Buffer::AddData( const uint8_t* pData, int size )
{
    uint8_t* pNewData = new uint8_t[m_Size + size];
    memcpy( pNewData, m_pData, m_Size );
    memcpy( pNewData + m_Size, pData, size );
    m_Size += size;
    delete []m_pData;
    m_pData = pNewData;
}

inline void Buffer::Clear()
{
    delete []m_pData;
    m_pData = NULL;
    m_Size = 0;
}

inline void Buffer::Write( OutStream& os ) const
{
    os << m_Size;
    os.Write( m_pData, m_Size );
}

inline bool Buffer::Read( InStream& is )
{
    int size;
    if (!is) return false;
    is >> size;
    Resize( size );
    is.Read( m_pData, size );
    return (is != false);
}

inline void Buffer::Resize( int size )
{
    delete []m_pData;
    m_pData = new uint8_t[size];
    m_Size = size;
}

#endif // __BUFFER_H__


