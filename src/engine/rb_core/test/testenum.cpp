#include "stdafx.h"
#include "JSignalServer.h"

enum JTestEnum
{
    JTestEnum_Apples    = 0,
    JTestEnum_Oranges   = 1,
    JTestEnum_Bananas   = 2,
}; // enum JTestEnum

enum_beg(JTestEnum)
    enum_val( "Apples",  JTestEnum_Apples  ),
    enum_val( "Oranges", JTestEnum_Oranges ),
    enum_val( "Bananas", JTestEnum_Bananas )
enum_end(JTestEnum)

class JTestEnumA : public JObject
{
    JTestEnum       m_Enum;
public:
    JTestEnumA() : m_Enum(JTestEnum_Bananas) {}
    
    expose(JTestEnumA)
    {
        parent( JObject );
        field( "enum", m_Enum );
    }
}; // class JTestEnumA
decl_class(JTestEnumA);

class JTestEnumB : public JObject
{
    JStringList       m_StrList;
public:
    JTestEnumB() {}

    JStringList     GetList() const { return m_StrList; }
    void            SetList( JStringList lst ) { m_StrList = lst; }
    
    expose(JTestEnumB)
    {
        parent( JObject );
        prop( "strlist", GetList, SetList );
    }
}; // class JTestEnumB
decl_class(JTestEnumB);

TEST(EnumToStringList)
{
    link_class( JTestEnumA );
    link_class( JTestEnumB );

    JTestEnumA a;
    JTestEnumB b;
    
    /*
    JSignalServer::s_pInstance->Connect( &a, "enum", &b, "strlist" );
    a.SendSignal( "enum" );
    
    JStringList lst = b.GetList();
    CHECK_EQUAL( 3, lst.size() );
    */
}