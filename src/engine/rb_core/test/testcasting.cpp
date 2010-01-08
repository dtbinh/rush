#include "precompile.h"
#include "JReflect.h"

enum Fruit
{
    Fruit_None      = 0,
    Fruit_Apple     = 1,
    Fruit_Orange    = 2,
    Fruit_Pear      = 3,
    Fruit_Plum      = 4,
    Fruit_Banana    = 5
}; // enum Fruit

enum_beg(Fruit)
    enum_val( "None",   Fruit_None    ),
    enum_val( "Apple",  Fruit_Apple   ),
    enum_val( "Orange", Fruit_Orange  ),
    enum_val( "Pear",   Fruit_Pear    ),
    enum_val( "Plum",   Fruit_Plum    ),
    enum_val( "Banana", Fruit_Banana  )
enum_end(Fruit)


TEST( BaseCast )
{
    int nVal = 5;
    bool bRes = false;
    JString strVal;

    bRes = cast( strVal, nVal );
    CHECK_EQUAL( true, bRes );
    CHECK( !strcmp( strVal.c_str(), "5" ) );

    strVal = "-18";
    bRes = cast( nVal, strVal );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( nVal, -18 );

    float fVal = 707.0f;
    bRes = cast( nVal, fVal );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( nVal, 707 );

    double dVal = -57.0;
    bRes = cast( fVal, dVal );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( fVal, -57.0f );

    fVal = 3.0f;
    bRes = cast( nVal, fVal );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( nVal, 3 );

    int nVal1 = -1917;
    bRes = cast( nVal, nVal1 );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( nVal, -1917 );

    bRes = cast( fVal, nVal );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( fVal, -1917.0f );
}

TEST( EnumCast )
{
    int nVal = 0;
    bool bRes = cast( nVal, Fruit_Banana );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( nVal, 5 );
    
    JString strVal;
    bRes = cast( strVal, Fruit_Orange );
    CHECK_EQUAL( true, bRes );
    CHECK( !strcmp( strVal.c_str(), "Orange" ) );

    nVal = 3;
    Fruit enVal = Fruit_None;
    bRes = cast( enVal, nVal );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( enVal, Fruit_Pear );

    strVal = "apple";
    bRes = cast( enVal, strVal );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( enVal, Fruit_Apple );

    JStringList strList;
    bRes = cast( strList, enVal );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( 6, strList.size() );
    CHECK( !strcmp( strList[5].c_str(), "Banana" ) );
}

TEST( MathCast )
{
    int nVal = 0;
    JString strVal;
    bool bRes = false;

    Frame frameVal( 1.0f, 2.0f, 3.0f, 4.0f );
    strVal = "";
    bRes = cast( strVal, frameVal );
    CHECK_EQUAL( true, bRes );
    frameVal = Frame::null;
    bRes = cast( frameVal, strVal );
    CHECK_EQUAL( true, bRes );
    CHECK( frameVal.x == 1.0f && frameVal.y == 2.0f && frameVal.w == 3.0f && frameVal.h == 4.0f );

    Vec2 vec2Val( 121.0f, 122.0f );
    strVal = "";
    bRes = cast( strVal, vec2Val );
    CHECK_EQUAL( true, bRes );
    vec2Val = Vec2::null;
    bRes = cast( vec2Val, strVal );
    CHECK_EQUAL( true, bRes );
    CHECK( vec2Val.x == 121.0f && vec2Val.y == 122.0f );

    Vec3 vec3Val( 11.0f, 12.0f, 13.0f );
    strVal = "";
    bRes = cast( strVal, vec3Val );
    CHECK_EQUAL( true, bRes );
    vec3Val = Vec3::null;
    bRes = cast( vec3Val, strVal );
    CHECK_EQUAL( true, bRes );
    CHECK( vec3Val.x == 11.0f && vec3Val.y == 12.0f && vec3Val.z == 13.0f );

    Vec4 vec4Val( 11.0f, 12.0f, 13.0f, 14.0f );
    strVal = "";
    bRes = cast( strVal, vec4Val );
    CHECK_EQUAL( true, bRes );
    vec4Val = Vec4::null;
    bRes = cast( vec4Val, strVal );
    CHECK_EQUAL( true, bRes );
    CHECK( vec4Val.x == 11.0f && vec4Val.y == 12.0f && vec4Val.z == 13.0f && vec4Val.w == 14.0f);

    Mat4 mat4Val( 11.0f, 12.0f, 13.0f, 14.0f,
                  21.0f, 22.0f, 23.0f, 24.0f,
                  31.0f, 32.0f, 33.0f, 34.0f,
                  41.0f, 42.0f, 43.0f, 44.0f );
    strVal = "";
    bRes = cast( strVal, mat4Val );
    CHECK_EQUAL( true, bRes );
    mat4Val = Mat4::null;
    bRes = cast( mat4Val, strVal );
    CHECK_EQUAL( true, bRes );
    CHECK( mat4Val.e00 == 11.0f && mat4Val.e01 == 12.0f && mat4Val.e02 == 13.0f && mat4Val.e03 == 14.0f &&
           mat4Val.e10 == 21.0f && mat4Val.e11 == 22.0f && mat4Val.e12 == 23.0f && mat4Val.e13 == 24.0f &&
           mat4Val.e20 == 31.0f && mat4Val.e21 == 32.0f && mat4Val.e22 == 33.0f && mat4Val.e23 == 34.0f &&
           mat4Val.e30 == 41.0f && mat4Val.e31 == 42.0f && mat4Val.e32 == 43.0f && mat4Val.e33 == 44.0f );
}

TEST( StringListCast )
{
    JStringList strList;
    strList.push_back( "blueberry" );
    strList.push_back( "strawberry" );
    strList.push_back( "raspberry" );
    strList.push_back( "gooseberry" );
    strList.push_back( "watermelon" );

    JString str;
    cast( str, strList );
    CHECK( str.size() > 0 );

    JStringList strList1;
    cast( strList1, str );

    CHECK( strList == strList1 );
}