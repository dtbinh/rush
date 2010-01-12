#include "precompile.h"
#include "JString.h"

TEST( String )
{
    JString str;
    CHECK_EQUAL( str.c_str()[0], 0 );
    CHECK_EQUAL( str.size(), 0 );
    str += "";
    CHECK_EQUAL( str.c_str()[0], 0 );
    CHECK_EQUAL( str.size(), 0 );
    str += "SomeString";
    CHECK( str == "SomeString" );
    CHECK( str != "somestring" ); 
    CHECK_EQUAL( str.size(), strlen( "SomeString" ) );
    CHECK( str != "AnotherString" );
    CHECK( str != "SomeStrin" );
    CHECK( str != "SomeStringg" );
    str = "AnotherString";
    CHECK( str == "AnotherString" );
    CHECK( str.size() == strlen( "AnotherString" ) );
    str += "123";
    str += "456789012345678901234567890";
    str.to_lower();
    CHECK( !strcmp( str.c_str(), "anotherstring123456789012345678901234567890" ) );

    str = JString( "anotherstring123456789012345678901234567890", 9 );
    CHECK( str == "anotherst" );
    CHECK_EQUAL( 9, str.size() );
    str.erase( 3, 3 );
    CHECK( str == "anorst" );
    str.insert( 0, "aaa" );
    CHECK( str == "aaaanorst" );
    str.insert( 6, "bbb" );
    CHECK( str == "aaaanobbbrst" );
    str.insert( 12, "ccc" );
    CHECK( str == "aaaanobbbrstccc" );
    str.insert( -2, "1234" );
    CHECK( str == "1234aaaanobbbrstccc" );
    str.insert( str.size() + 5, "1234" );
    CHECK( str == "1234aaaanobbbrstccc1234" );
    str.erase( 4, 15 );
    CHECK( str == "12341234" );
    str.erase( -10, 4 );
    CHECK( str == "12341234" );
    str.erase( 4, 0 );
    CHECK( str == "12341234" );
    str.erase( -2, 4 );
    CHECK( str == "341234" );

    int strSize = sizeof( JString );
}