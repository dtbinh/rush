//****************************************************************************/
//  File:   JMathTypeCast.h
//  Desc:   There are declared math-needed typecasts 
//****************************************************************************/
#ifndef __JMATHTYPECAST_H__
#define __JMATHTYPECAST_H__

#include "Vec2.h"
#include "Vec3.h"
#include "Mat3.h"
#include "Mat4.h"
#include "Frame.h"
#include "Plane.h"
#include "Date.h"
#include "PolyLine2.h"
#include "Ramp.h"
#include "ColorRamp.h"

#include "JObject.h"
#include "JTypeCast.h"

typedef const char* PCChar;
inline bool extractFloat( PCChar& pFrom, float& val )
{
    pFrom += strspn( pFrom, "{ \t\n\r," );
    if (*pFrom == 0) return false;
    bool bRes = (sscanf( pFrom, "%f", &val ) != 0);
    pFrom += strcspn( pFrom, ", \t\n\r" );
    return bRes;
} // extractFloat

//****************************************************************************/
/*	JString <=> Vec2
//****************************************************************************/
template <>
inline bool cast<JString, Vec2>( JString& to, const Vec2& from )
{
    char buf[c_ConvBufSize];
    static JString strX, strY;
    cast( strX, from.x );
    cast( strY, from.y );

    sprintf( buf, "{ %s, %s }", strX.c_str(), strY.c_str() );
    to = buf;
    return true;
}

template <>
inline bool cast<Vec2, JString>( Vec2& to, const JString& from )
{
    const char* pFrom = from.c_str();
    if (!extractFloat( pFrom, to.x )) return false;
    if (!extractFloat( pFrom, to.y )) return false;
    return true;
}

template <> inline const char* type_name<Vec2>() { return "Vec2"; }


//****************************************************************************/
/*	JString <=> Vec3
//****************************************************************************/
template <>
inline bool cast<JString, Vec3>( JString& to, const Vec3& from )
{
    char buf[c_ConvBufSize];
    static JString strX, strY, strZ;
    cast( strX, from.x );
    cast( strY, from.y );
    cast( strZ, from.z );

    sprintf( buf, "{ %s, %s, %s }", strX.c_str(), strY.c_str(), strZ.c_str() );
    to = buf;
    return true;
}

template <>
inline bool cast<Vec3, JString>( Vec3& to, const JString& from )
{
    const char* pFrom = from.c_str();
    if (!extractFloat( pFrom, to.x )) return false;
    if (!extractFloat( pFrom, to.y )) to.y = 0.0f;
    if (!extractFloat( pFrom, to.z )) to.z = 0.0f;
    return true;
}

template <> inline const char* type_name<Vec3>() { return "Vec3"; }


//****************************************************************************/
/*	JString <=> Vec4
//****************************************************************************/
template <>
inline bool cast<JString, Vec4>( JString& to, const Vec4& from )
{
    char buf[c_ConvBufSize];
    static JString strX, strY, strZ, strW;
    cast( strX, from.x );
    cast( strY, from.y );
    cast( strZ, from.z );
    cast( strW, from.w );

    sprintf( buf, "{ %s, %s, %s, %s }", strX.c_str(), strY.c_str(), strZ.c_str(), strW.c_str() );
    to = buf;
    return true;
}

template <>
inline bool cast<Vec4, JString>( Vec4& to, const JString& from )
{
    const char* pFrom = from.c_str();
    if (!extractFloat( pFrom, to.x )) return false;
    if (!extractFloat( pFrom, to.y )) return false;
    if (!extractFloat( pFrom, to.z )) return false;
    if (!extractFloat( pFrom, to.w )) return false;
    return true;
}

template <> inline const char* type_name<Vec4>() { return "Vec4"; }


//****************************************************************************/
/*	JString <=> Mat3
//****************************************************************************/
template <>
inline bool cast<JString, Mat3>( JString& to, const Mat3& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "{ %f, %f, %f,   %f, %f, %f,   %f, %f, %f }", 
                    from.e00, from.e01, from.e02,
                    from.e10, from.e11, from.e12,
                    from.e20, from.e21, from.e22 );
    to = buf;
    return true;
}

template <>
inline bool cast<Mat3, JString>( Mat3& to, const JString& from )
{
    const char* pFrom = from.c_str();

    if (!extractFloat( pFrom, to.e00 )) return false;
    if (!extractFloat( pFrom, to.e01 )) return false;
    if (!extractFloat( pFrom, to.e02 )) return false;
    if (!extractFloat( pFrom, to.e10 )) return false;
    if (!extractFloat( pFrom, to.e11 )) return false;
    if (!extractFloat( pFrom, to.e12 )) return false;
    if (!extractFloat( pFrom, to.e20 )) return false;
    if (!extractFloat( pFrom, to.e21 )) return false;
    if (!extractFloat( pFrom, to.e22 )) return false;

    return true;
}

template <> inline const char* type_name<Mat3>() { return "Mat3"; }


//****************************************************************************/
/*	JString <=> Mat4
//****************************************************************************/
template <>
inline bool cast<JString, Mat4>( JString& to, const Mat4& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "{ %f, %f, %f, %f,   %f, %f, %f, %f,   %f, %f, %f, %f,   %f, %f, %f, %f }", 
                    from.e00, from.e01, from.e02, from.e03,
                    from.e10, from.e11, from.e12, from.e13,
                    from.e20, from.e21, from.e22, from.e23,
                    from.e30, from.e31, from.e32, from.e33 );
    to = buf;
    return true;
}

template <>
inline bool cast<Mat4, JString>( Mat4& to, const JString& from )
{
    const char* pFrom = from.c_str();

    if (!extractFloat( pFrom, to.e00 )) return false;
    if (!extractFloat( pFrom, to.e01 )) return false;
    if (!extractFloat( pFrom, to.e02 )) return false;
    if (!extractFloat( pFrom, to.e03 )) return false;
    if (!extractFloat( pFrom, to.e10 )) return false;
    if (!extractFloat( pFrom, to.e11 )) return false;
    if (!extractFloat( pFrom, to.e12 )) return false;
    if (!extractFloat( pFrom, to.e13 )) return false;
    if (!extractFloat( pFrom, to.e20 )) return false;
    if (!extractFloat( pFrom, to.e21 )) return false;
    if (!extractFloat( pFrom, to.e22 )) return false;
    if (!extractFloat( pFrom, to.e23 )) return false;
    if (!extractFloat( pFrom, to.e30 )) return false;
    if (!extractFloat( pFrom, to.e31 )) return false;
    if (!extractFloat( pFrom, to.e32 )) return false;
    if (!extractFloat( pFrom, to.e33 )) return false;
    return true;
}

template <> inline const char* type_name<Mat4>() { return "Mat4"; }

//****************************************************************************/
/*	JString <=> Frame
//****************************************************************************/
template <>
inline bool cast<JString, Frame>( JString& to, const Frame& from )
{
    char buf[c_ConvBufSize];
    static JString strX, strY, strW, strH;
    cast( strX, from.x );
    cast( strY, from.y );
    cast( strW, from.w );
    cast( strH, from.h );

    sprintf( buf, "{ %s, %s, %s, %s }", strX.c_str(), strY.c_str(), strW.c_str(), strH.c_str() );
    to = buf;
    return true;
}

template <>
inline bool cast<Frame, JString>( Frame& to, const JString& from )
{
    const char* pFrom = from.c_str();
    if (!extractFloat( pFrom, to.x )) return false;
    if (!extractFloat( pFrom, to.y )) return false;
    if (!extractFloat( pFrom, to.w )) return false;
    if (!extractFloat( pFrom, to.h )) return false;
    return true;
}

template <> inline const char* type_name<Frame>() { return "Frame"; }

//****************************************************************************/
/*	JString <=> Plane
//****************************************************************************/
template <>
inline bool cast<JString, Plane>( JString& to, const Plane& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "{ %f, %f, %f, %f }", from.a, from.b, from.c, from.d );
    to = buf;
    return true;
}

template <>
inline bool cast<Plane, JString>( Plane& to, const JString& from )
{
    const char* pFrom = from.c_str();
    if (!extractFloat( pFrom, to.a )) return false;
    if (!extractFloat( pFrom, to.b )) return false;
    if (!extractFloat( pFrom, to.c )) return false;
    if (!extractFloat( pFrom, to.d )) return false;
    return true;
}

template <> inline const char* type_name<Plane>() { return "Plane"; }

typedef std::vector<Vec3> JVec3List;
//****************************************************************************/
/*	JString <=> JVec3List
//****************************************************************************/
template <>
inline bool cast<JString, JVec3List>( JString& to, const JVec3List& from )
{
    int nPt = from.size();
    if (nPt == 0) 
    {
        to = "{}";
        return true;
    }
    char buf[c_ConvBufSize];
    static JString strX, strY, strZ;
    to = "{";
    for (int i = 0; i < nPt - 1; i++)
    {
        const Vec3& pt = from[i];
        cast( strX, pt.x );
        cast( strY, pt.y );
        cast( strZ, pt.z );
        sprintf( buf, "{%s, %s, %s}, ", strX.c_str(), strY.c_str(), strZ.c_str() );
        to += buf;
    }
    const Vec3& pt = from[nPt - 1];
    cast( strX, pt.x );
    cast( strY, pt.y );
    cast( strZ, pt.z );
    sprintf( buf, "{%s, %s, %s}}", strX.c_str(), strY.c_str(), strZ.c_str() );
    to += buf;
    return true;
}

template <>
inline bool cast<JVec3List, JString>( JVec3List& to, const JString& from )
{
    int cChar = 0;
    int len = from.size();
    const char* pFrom = from.c_str();
    const char* pC = pFrom;
    float val = 0;
    to.clear();
    while (pC - pFrom < len)
    {
        Vec3 v;
        pC += strspn( pC, "{ \t\n\r" );
        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &v.x ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );

        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &v.y ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );

        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &v.z ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );

        to.push_back( v );
    }
    return true;
}

inline OutStream& operator <<( OutStream& os, const JVec3List& lst )
{
    os << lst.size();
    for (int i = 0; i < lst.size(); i++)
    {
        os << lst[i];
    }
    return os;
}

inline InStream& operator >>( InStream& is, JVec3List& lst )
{
    int nElem = 0;
    is >> nElem;
    lst.resize( nElem );
    for (int i = 0; i < nElem; i++)
    {
        is >> lst[i];
    }
    return is;
}
template <> inline const char* type_name<JVec3List>() { return "JVec3List"; }

//****************************************************************************/
/*	JString <=> Date
//****************************************************************************/
template <>
inline bool cast<JString, Date>( JString& to, const Date& from )
{
    char buf[c_ConvBufSize];
    sprintf( buf, "%d.%d.%d %d:%d:%d", from.GetDay(), from.GetMonth(), from.GetYear(), 
                    from.GetHour(), from.GetMinutes(), from.GetSeconds() );
    to = buf;
    return true;
}

template <>
inline bool cast<Date, JString>( Date& to, const JString& from )
{
    int cChar = 0;
    int len = from.size();
    const char* pFrom = from.c_str();
    const char* pC = pFrom;
    int val = 0;

    pFrom += strspn( pFrom, "{ \t\n\r" );
    if (pC - pFrom >= len) return false;

    if (sscanf( pFrom, "%d", &val ) == 0) return false;
    to.SetDay( val );
    pFrom += strcspn( pFrom, ",. \t\n\r" );
    pFrom += strspn( pFrom, "{,. \t\n\r" );
    if (pC - pFrom >= len) return false;

    if (sscanf( pFrom, "%d", &val ) == 0) return false;
    to.SetMonth( val );
    pFrom += strcspn( pFrom, ",. \t\n\r" );
    pFrom += strspn( pFrom, "{,. \t\n\r" );
    if (pC - pFrom >= len) return false;

    if (sscanf( pFrom, "%d", &val ) == 0) return false;
    to.SetYear( val );
    pFrom += strcspn( pFrom, ",. \t\n\r" );
    pFrom += strspn( pFrom, "{,. \t\n\r" );
    if (pC - pFrom >= len) return false;

    if (sscanf( pFrom, "%d", &val ) == 0) return false;
    to.SetHour( val );
    pFrom += strcspn( pFrom, ",: \t\n\r" );
    pFrom += strspn( pFrom, "{,: \t\n\r" );
    if (pC - pFrom >= len) return false;

    if (sscanf( pFrom, "%d", &val ) == 0) return false;
    to.SetMinutes( val );
    pFrom += strcspn( pFrom, ",: \t\n\r" );
    pFrom += strspn( pFrom, "{,: \t\n\r" );
    if (pC - pFrom >= len) return false;

    if (sscanf( pFrom, "%d", &val ) == 0) return false;
    to.SetSeconds( val );
    pFrom += strcspn( pFrom, ",: \t\n\r" );
    pFrom += strspn( pFrom, "{,: \t\n\r" );
    if (pC - pFrom >= len) return false;

    return true;
}

template <> inline const char* type_name<Date>() { return "Date"; }

//****************************************************************************/
/*	JString <=> PolyLine2
//****************************************************************************/
template <>
inline bool cast<JString, PolyLine2>( JString& to, const PolyLine2& from )
{
    int nPt = from.GetNPoints();
    if (nPt == 0) 
    {
        to = "{}";
        return true;
    }
    char buf[c_ConvBufSize];
    static JString strX, strY;
    to = "{";
    for (int i = 0; i < nPt - 1; i++)
    {
        const Vec2& pt = from.GetPoint( i );
        cast( strX, pt.x );
        cast( strY, pt.y );
        sprintf( buf, "{%s, %s}, ", strX.c_str(), strY.c_str() );
        to += buf;
    }
    const Vec2& pt = from.GetPoint( nPt - 1 );
    cast( strX, pt.x );
    cast( strY, pt.y );
    sprintf( buf, "{%s, %s}}", strX.c_str(), strY.c_str() );
    to += buf;
    return true;
}

template <>
inline bool cast<PolyLine2, JString>( PolyLine2& to, const JString& from )
{
    int cChar = 0;
    int len = from.size();
    const char* pFrom = from.c_str();
    const char* pC = pFrom;
    float val = 0;
    to.Clear();
    while (pC - pFrom < len)
    {
        Vec2 v;
        pC += strspn( pC, "{ \t\n\r" );
        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &v.x ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );
        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &v.y ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );
        to.AddPoint( v );
    }
    return true;
}

inline OutStream& operator <<( OutStream& os, const PolyLine2& pl )
{
    int nPt = pl.GetNPoints();
    os << nPt;
    for (int i = 0; i < nPt; i++)
    {
        os << pl.GetPoint( i );
    }
    return os;
}

inline InStream& operator >>( InStream& is, PolyLine2& pl )
{
    int nElem = 0;
    is >> nElem;
    pl.Clear();
    Vec2 pt;
    for (int i = 0; i < nElem; i++)
    {
        is >> pt;
        pl.AddPoint( pt );
    }
    return is;
}

template <> inline const char* type_name<PolyLine2>() { return "PolyLine2"; }

//****************************************************************************/
/*	JString <=> Ramp
//****************************************************************************/
template <>
inline bool cast<JString, Ramp>( JString& to, const Ramp& from )
{
    int nPt = from.GetNKeys();
    if (nPt == 0) 
    {
        to = "{}";
        return true;
    }
    char buf[c_ConvBufSize];
    static JString strX, strY;
    to = "{";
    int i;
    for (i = 0; i < nPt - 1; i++)
    {
        float cT = from.GetTime ( i );
        float cV = from.GetValue( i );
        cast( strX, cT );
        cast( strY, cV );
        sprintf( buf, "{%s, %s}, ", strX.c_str(), strY.c_str() );
        to += buf;
    }
    float cT = from.GetTime ( i );
    float cV = from.GetValue( i );
    cast( strX, cT );
    cast( strY, cV );
    sprintf( buf, "{%s, %s}}", strX.c_str(), strY.c_str() );
    to += buf;
    return true;
}

template <>
inline bool cast<Ramp, JString>( Ramp& to, const JString& from )
{
    int cChar = 0;
    int len = from.size();
    const char* pFrom = from.c_str();
    const char* pC = pFrom;
    float val = 0;
    to.Clear();
    while (pC - pFrom < len)
    {
        Vec2 v;
        pC += strspn( pC, "{ \t\n\r" );
        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &v.x ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );
        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &v.y ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );
        to.AddKey( v.x, v.y );
    }
    return true;
}

inline OutStream& operator <<( OutStream& os, const Ramp& pl )
{
    int nPt = pl.GetNKeys();
    os << nPt;
    for (int i = 0; i < nPt; i++)
    {
        os << pl.GetTime( i ) << pl.GetValue( i );
    }
    return os;
}

inline InStream& operator >>( InStream& is, Ramp& pl )
{
    int nElem = 0;
    is >> nElem;
    pl.Clear();
    Vec2 pt;
    for (int i = 0; i < nElem; i++)
    {
        is >> pt;
        pl.AddKey( pt.x, pt.y );
    }
    return is;
}

template <> inline const char* type_name<Ramp>() { return "Ramp"; }

//****************************************************************************/
/*	JString <=> ColorRamp
//****************************************************************************/
template <>
inline bool cast<JString, ColorRamp>( JString& to, const ColorRamp& from )
{
    int nPt = from.GetNKeys();
    if (nPt == 0) 
    {
        to = "{}";
        return true;
    }
    char buf[c_ConvBufSize];
    static JString strX, strY;
    to = "{";
    int i;
    for (i = 0; i < nPt - 1; i++)
    {
        float cT = from.GetTime ( i );
        DWORD cV = from.GetValue( i );
        cast( strX, cT );
        cast( strY, cV );
        sprintf( buf, "{%s, %s}, ", strX.c_str(), strY.c_str() );
        to += buf;
    }
    float cT = from.GetTime ( i );
    DWORD cV = from.GetValue( i );
    cast( strX, cT );
    cast( strY, cV );
    sprintf( buf, "{%s, %s}}", strX.c_str(), strY.c_str() );
    to += buf;
    return true;
}

template <>
inline bool cast<ColorRamp, JString>( ColorRamp& to, const JString& from )
{
    int cChar = 0;
    int len = from.size();
    const char* pFrom = from.c_str();
    const char* pC = pFrom;
    float val = 0;
    to.Clear();
    while (pC - pFrom < len)
    {
        float cT;
        DWORD cV;
        pC += strspn( pC, "{ \t\n\r" );
        if (pC - pFrom >= len) return false;
        if (sscanf( pC, "%f", &cT ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );
        if (pC - pFrom >= len) return false;

        if (sscanf( pC, "%X", &cV ) == 0) return false;
        pC += strcspn( pC, ", \t\n\r" );
        pC += strspn( pC, "{, \t\n\r" );

        to.AddKey( cT, cV );
    }
    return true;
}

inline OutStream& operator <<( OutStream& os, const ColorRamp& pl )
{
    int nPt = pl.GetNKeys();
    os << nPt;
    for (int i = 0; i < nPt; i++)
    {
        os << pl.GetTime( i ) << pl.GetValue( i );
    }
    return os;
}

inline InStream& operator >>( InStream& is, ColorRamp& pl )
{
    int nElem = 0;
    is >> nElem;
    pl.Clear();
    float cT;
    DWORD cV;
    for (int i = 0; i < nElem; i++)
    {
        is >> cT >> cV;
        pl.AddKey( cT, cV );
    }
    return is;
}

template <> inline const char* type_name<ColorRamp>() { return "ColorRamp"; }

#endif // __JMATHTYPECAST_H__

