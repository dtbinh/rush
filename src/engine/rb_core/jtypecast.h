//****************************************************************************/
//  File:  JTypeCast.h
//****************************************************************************/
#ifndef __JTYPECAST_H__
#define __JTYPECAST_H__

#include <string.h>


#include "stream.h"
#include "jstring.h"

template <class TTo, class TFrom>
inline bool cast( TTo& to, const TFrom& from )
{
    to = from;
    return true;
}

template <class T> inline const char* type_name() { return "unknown"; }
template <class T> inline bool enumerate( JStringList& valList ) { return false; }


//****************************************************************************/
/*	std::string <=> JString
//****************************************************************************/
template <>
inline bool cast<std::string, JString>( std::string& to, const JString& from )
{
    to = from;
    return true;
}

template <>
inline bool cast<JString, std::string>( JString& to, const std::string& from )
{
    to = from.c_str();
    return true;
}

inline OutStream& operator <<( OutStream& os, const JString& val )
{
    int nCh = val.size();
    os.Write( (uint8_t*)&nCh, sizeof( int ) );
    os.Write( (uint8_t*)val.c_str(), nCh );
    return os;
}

inline InStream& operator >>( InStream& is, JString& val )
{
   int nCh = 0;
   is.Read( &nCh, sizeof( int ) );
   val.resize( nCh );
   is.Read( (void*)val.c_str(), nCh );
   return is;
}

//  FIXME
const int c_ConvBufSize = 2048;
//****************************************************************************/
/*	JString <=> int
//****************************************************************************/
template <>
inline bool cast<JString, int>( JString& to, const int& from )
{
    char buf[c_ConvBufSize];
    char* pEnd = _itoa( from, buf, 10 );
    to = buf;
    return (pEnd != NULL);
}

template <>
inline bool cast<int, JString>( int& to, const JString& from )
{
    char* pEnd = NULL;
    to = strtol( from.c_str(), &pEnd, 10 );
    return (*pEnd == 0);
}

template <> inline const char* type_name<int>() { return "int"; }

//****************************************************************************/
/*	JString <=> const char*
//****************************************************************************/
typedef const char* PConstChar;
template <>
inline bool cast<JString, PConstChar>( JString& to, const PConstChar& from )
{
    to = from;
    return true;
}

template <>
inline bool cast<PConstChar, JString>( PConstChar& to, const JString& from )
{
    to = from.c_str();
    return true;
}

template <> inline const char* type_name<const char*>() { return "string"; }

//****************************************************************************/
/*	std::wstring <=> int
//****************************************************************************/
template <>
inline bool cast<std::wstring, int>( std::wstring& to, const int& from )
{
    wchar_t buf[c_ConvBufSize];
    swprintf( buf, L"%d", from );
    to = buf;
    return true;
}

template <>
inline bool cast<int, std::wstring>( int& to, const std::wstring& from )
{
    int val = 0;
    if (swscanf( from.c_str(), L"%d", &val ) == 0) return false;
    to = val;
    return true;
}

//****************************************************************************/
/*	JString <=> uint32_t
//****************************************************************************/
template <>
inline bool cast<JString, uint32_t>( JString& to, const uint32_t& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "%#x", from );
    to = buf;
    return true;
}

template <>
inline bool cast<uint32_t, JString>( uint32_t& to, const JString& from )
{
    int val = 0;
    if (sscanf( from.c_str(), "%X", &val ) == 0) return false;
    to = val;
    return true;
}

template <> inline const char* type_name<uint32_t>() { return "uint32_t"; }

//****************************************************************************/
/*	JString <=> uint16_t
//****************************************************************************/
template <>
inline bool cast<JString, uint16_t>( JString& to, const uint16_t& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "%#x", from );
    to = buf;
    return true;
}

template <>
inline bool cast<uint16_t, JString>( uint16_t& to, const JString& from )
{
    int val = 0;
    if (sscanf( from.c_str(), "%X", &val ) == 0) return false;
    to = val;
    return true;
}

template <> inline const char* type_name<uint16_t>() { return "uint16_t"; }

//****************************************************************************/
/*	JString <=> uint8_t
//****************************************************************************/
template <>
inline bool cast<JString, uint8_t>( JString& to, const uint8_t& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "%#x", from );
    to = buf;
    return true;
}

template <>
inline bool cast<uint8_t, JString>( uint8_t& to, const JString& from )
{
    int val = 0;
    if (sscanf( from.c_str(), "%X", &val ) == 0) return false;
    to = val;
    return true;
}

template <> inline const char* type_name<uint8_t>() { return "uint8_t"; }

//****************************************************************************/
/*	JString <=> float
//****************************************************************************/
template <>
inline bool cast<JString, float>( JString& to, const float& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "%f", from );
    to = buf;
    //  kill trailing zeros
    char* pTrail = &to[to.size() - 1];
    while (*pTrail == '0')
    {
        *pTrail = 0;
        pTrail--;
    }
    if (*pTrail == '.') *pTrail = 0;
    return true;
}

template <>
inline bool cast<float, JString>( float& to, const JString& from )
{
    char* pEnd = NULL;
    float val = float( strtod( from.c_str(), &pEnd ) );
    if (pEnd && *pEnd != 'f' && *pEnd != 'F' &&
        pEnd - from.c_str() < from.size())
    {
        return false;
    }
    to = val;
    return true;
}

template <> inline const char* type_name<float>() { return "float"; }

//****************************************************************************/
/*	JString <=> double
//****************************************************************************/
template <>
inline bool cast<JString, double>( JString& to, const double& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "%f", from );
    to = buf;
    return true;
}

template <>
inline bool cast<double, JString>( double& to, const JString& from )
{
    int val = 0;
    if (sscanf( from.c_str(), "%f", &val ) == 0) return false;
    to = val;
    return true;
}

template <> inline const char* type_name<double>() { return "double"; }

//****************************************************************************/
/*	JString <=> bool
//****************************************************************************/
template <>
inline bool cast<JString, bool>( JString& to, const bool& from )
{
    to = from ? "true" : "false";
    return true;
}

template <>
inline bool cast<bool, JString>( bool& to, const JString& from )
{
    if (!stricmp( from.c_str(), "true" ) || !strcmp( from.c_str(), "1" )) { to = true; return true; }
    if (!stricmp( from.c_str(), "false") || !strcmp( from.c_str(), "0" )) { to = false; return true; }
    return false;
}

template <> inline const char* type_name<bool>() { return "bool"; }


//****************************************************************************/
/*	JString <=> JStringList
//****************************************************************************/
template <>
inline bool cast<JString, JStringList>( JString& to, const JStringList& from )
{
    to = "{ ";
    int lastS = from.size() - 1;
    for (int i = 0; i < lastS; i++)
    {
        to += "\"";
        to += from[i];
        to += "\", ";
    }
    if (lastS >= 0)
    {
        to += "\"";
        to += from[lastS];
        to += "\"";
    }
    to += " }";
    return true;
}

template <>
inline bool cast<JStringList, JString>( JStringList& to, const JString& from )
{
    int cChar = 0;
    int len = from.size();
    const char* pFrom = from.c_str();
    const char* pC = pFrom;
    pC += strspn( pC, " {\n\t\"" );
    to.clear();
    while (pC - pFrom < len)
    {
        int nCh = strcspn( pC, ",\n\t\"}" );
        to.push_back( JString( pC, nCh ) );
        pC += nCh;
        pC += strspn( pC, " ,\n\t\"}" );
    }
    return true;
}

inline OutStream& operator <<( OutStream& os, const JStringList& lst )
{
    int nElem = lst.size();
    os << nElem;
    for (int i = 0; i < nElem; i++)
    {
        os << lst[i];
    }
    return os;
}

inline InStream& operator >>( InStream& is, JStringList& lst )
{
    int nElem = 0;
    is >> nElem;
    lst.resize( nElem );
    if (nElem == 0) return is;
    for (int i = 0; i < nElem; i++)
    {
        is >> lst[i];
    }
    return is;
}

template <> inline const char* type_name<JStringList>() { return "stringlist"; }

#include "jobject.h"
//****************************************************************************/
/*	JString <=> JObject
//****************************************************************************/
#include "jobjectserver.h"
typedef JObject* PObject;
template <>
inline bool cast<JString, PObject>( JString& to, const PObject& from )
{
    if (!from)
    {
        to = "";
    }
    else
    {
        from->GetPath( to );
    }
    return true;
}

template <>
inline bool cast<JObject*, JString>( PObject& to, const JString& from )
{
    to = g_pObjectServer->FindObject( from.c_str() );
    return (to != NULL);
}

template <> inline const char* type_name<JString>() { return "string"; }


typedef std::vector<float> JFloatList;
//****************************************************************************/
/*	JString <=> JFloatList
//****************************************************************************/
template <>
inline bool cast<JString, JFloatList>( JString& to, const JFloatList& from )
{
    int nPt = from.size();
    if (nPt == 0)
    {
        to = "{}";
        return true;
    }
    char buf[c_ConvBufSize];
    static JString str;
    to = "{";
    for (int i = 0; i < nPt - 1; i++)
    {
        float val = from[i];
        cast( str, val );
        sprintf( buf, "%s, ", str.c_str() );
        to += buf;
    }
    float val = from[nPt - 1];
    cast( str, val );
    sprintf( buf, "%s}", str.c_str() );
    to += buf;
    return true;
}

template <>
inline bool cast<JFloatList, JString>( JFloatList& to, const JString& from )
{
    int cChar = 0;
    int len = from.size();
    const char* pFrom = from.c_str();
    const char* pC = pFrom;
    float val = 0;
    to.clear();
    while (pC - pFrom < len)
    {
        float v;
        pC += strspn( pC, "{ \t\n\r" );
        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &v ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );
        to.push_back( v );
    }
    return true;
}

inline OutStream& operator <<( OutStream& os, const JFloatList& lst )
{
    int nElem = lst.size();
    os << nElem;
    os.Write( (uint8_t*)&lst[0], nElem*sizeof( float ) );
    return os;
}

inline InStream& operator >>( InStream& is, JFloatList& lst )
{
    int nElem = 0;
    is >> nElem;
    float val = 0.0f;
    lst.clear();
    if (nElem == 0) return is;
    lst.resize( nElem );
    is.Read( &lst[0], nElem*sizeof( float ) );
    return is;
}

template <> inline const char* type_name<JFloatList>() { return "JFloatList"; }

typedef std::vector<int> JIntList;
//****************************************************************************/
/*	JString <=> JIntList
//****************************************************************************/
template <>
inline bool cast<JString, JIntList>( JString& to, const JIntList& from )
{
    int nPt = from.size();
    if (nPt == 0)
    {
        to = "{}";
        return true;
    }
    char buf[c_ConvBufSize];
    static JString str;
    to = "{";

    for (int i = 0; i < nPt - 1; i++)
    {
        int val = from[i];
        cast( str, val );
        sprintf( buf, "%s, ", str.c_str() );
        to += buf;
    }
    int val = from[nPt - 1];
    cast( str, val );
    sprintf( buf, "%s}", str.c_str() );
    to += buf;
    return true;
}

template <>
inline bool cast<JIntList, JString>( JIntList& to, const JString& from )
{
    int cChar = 0;
    int len = from.size();
    const char* pFrom = from.c_str();
    const char* pC = pFrom;
    int val = 0;
    to.clear();
    while (pC - pFrom < len)
    {
        int v;
        pC += strspn( pC, "{ \t\n\r" );
        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%d", &v ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );
        to.push_back( v );
    }
    return true;
}

inline OutStream& operator <<( OutStream& os, const JIntList& lst )
{
    int nElem = lst.size();
    os << nElem;
    os.Write( (uint8_t*)&lst[0], nElem*sizeof( int ) );
    return os;
}

inline InStream& operator >>( InStream& is, JIntList& lst )
{
    int nElem = 0;
    is >> nElem;
    int val = 0.0f;
    lst.clear();
    if (nElem == 0) return is;
    lst.resize( nElem );
    is.Read( &lst[0], nElem*sizeof( int ) );
    return is;
}

template <> inline const char* type_name<JIntList>() { return "JIntList"; }

#include "jmathtypecast.h"

#endif // __JTYPECAST_H__

