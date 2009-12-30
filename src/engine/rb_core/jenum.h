//****************************************************************************/
//  File:  JEnum.h
//****************************************************************************/
#ifndef __JENUM_H__
#define __JENUM_H__

#include "jtypecast.h"

//****************************************************************************/
/*  Struct: JEnumVal
//****************************************************************************/
struct JEnumVal
{
    JString      m_Name;
    int             m_Val;
    JEnumVal( const char* name, int val ) : m_Name(name), m_Val(val){}
}; // struct JEnumVal

inline bool EnumToString( JString& to, const int& from, const JEnumVal* pEnum )
{
    if (!pEnum) return false;
    int cVal = 0;
    while (pEnum[cVal].m_Name.size() != 0)
    {
        if (pEnum[cVal].m_Val == from)
        {
            to = pEnum[cVal].m_Name.c_str();
            return true;
        }
        cVal++;
    }
    return false;
} // EnumToString

inline bool StringToEnum( int& to, const JString& from, const JEnumVal* pEnum )
{
    if (!pEnum) return false;
    if (cast( to, from ))
    {
        return true;
    }
    int cVal = 0;
    while (pEnum[cVal].m_Name.size() != 0)
    {
        if (pEnum[cVal].m_Name.is_equal_ci( from.c_str() ))
        {
            to = pEnum[cVal].m_Val;
            return true;
        }
        cVal++;
    }
    return false;
} // StringToEnum

inline bool EnumValues( JStringList& to, const JEnumVal* pEnum )
{
    if (!pEnum) return false;
    int cVal = 0;
    to.clear();
    while (pEnum[cVal].m_Name.size() != 0)
    {
        to.push_back( pEnum[cVal].m_Name.c_str() );
        cVal++;
    }
    return true;
} // EnumValues

#define enum_beg( TName )      const JEnumVal TName##Values[] = {
#define enum_end( TName )       ,JEnumVal( "", -1 ) };  \
                                template <> inline bool cast<JStringList,TName>( JStringList& to, const TName& from ) { return EnumValues( to, TName##Values ); }\
                                template <> inline bool enumerate<TName>( JStringList& to ) { return EnumValues( to, TName##Values ); }\
                                template <> inline bool cast<int, TName>( int& to, const TName& from ) { to = (int)from; return true; }\
                                template <> inline bool cast<TName, int>( TName& to, const int& from ) { to = (TName)from; return true; }\
                                template <> inline bool cast<JString, TName>( JString& to, const TName& from ) { return EnumToString( to, from, TName##Values ); }\
                                template <> inline bool cast<TName, JString>( TName& to, const JString& from )\
                                { int val = -1; bool res = StringToEnum( val, from, TName##Values ); \
                                to = (TName)val; return res; }\
                                template <> inline const char* type_name<TName>() { return "enum_"#TName; }
typedef JEnumVal enum_val;


#endif // __JENUM_H__


