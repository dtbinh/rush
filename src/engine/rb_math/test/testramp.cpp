#include "stdafx.h"
#include "ramp.h"

static Ramp ramp;
static float cVal;
TEST( RampZeroFind )
{
    float val = ramp.GetValue( 15.0f );
    CHECK_EQUAL( 0.0f, val );
}

TEST( RampOneFind )
{
    ramp.AddKey( 1.0f, 5.0f );

    cVal = ramp.GetValue( -1.0f );
    CHECK_EQUAL( 5.0f, cVal );

    cVal = ramp.GetValue( 1.0f );
    CHECK_EQUAL( 5.0f, cVal );

    cVal = ramp.GetValue( 10.0f );
    CHECK_EQUAL( 5.0f, cVal );
}

TEST( RampTwoFind )
{
    ramp.AddKey( 6.0f, 10.0f );

    cVal = ramp.GetValue( -1.0f );
    CHECK_EQUAL( 5.0f, cVal );

    cVal = ramp.GetValue( 1.0f );
    CHECK_EQUAL( 5.0f, cVal );

    cVal = ramp.GetValue( 6.0f );
    CHECK_EQUAL( 10.0f, cVal );

    cVal = ramp.GetValue( 60.0f );
    CHECK_EQUAL( 10.0f, cVal );

    cVal = ramp.GetValue( 3.0f );
    CHECK_EQUAL( 7.0f, cVal );
}

TEST( RampThreeFind )
{
    ramp.AddKey( 11.0f, 5.0f );

    cVal = ramp.GetValue( -1.0f );
    CHECK_EQUAL( 5.0f, cVal );

    cVal = ramp.GetValue( 1.0f );
    CHECK_EQUAL( 5.0f, cVal );

    cVal = ramp.GetValue( 6.0f );
    CHECK_EQUAL( 10.0f, cVal );

    cVal = ramp.GetValue( 60.0f );
    CHECK_EQUAL( 5.0f, cVal );

    cVal = ramp.GetValue( 3.0f );
    CHECK_EQUAL( 7.0f, cVal );

    cVal = ramp.GetValue( 11.0f );
    CHECK_EQUAL( 5.0f, cVal );

    cVal = ramp.GetValue( 10.0f );
    CHECK_EQUAL( 6.0f, cVal );
}

TEST( RampDeleteKey )
{
   ramp.DeleteKey( 2 );

   cVal = ramp.GetValue( 60.0f );
   CHECK_EQUAL( 10.0f, cVal );
   CHECK_EQUAL( 2, ramp.GetNKeys() );
}

TEST( RampDuplicateKey )
{
   ramp.AddKey( 11.0f, 5.0f );
   ramp.AddKey( 11.0f, 5.0f );

   cVal = ramp.GetValue( 60.0f );
   CHECK_EQUAL( 5.0f, cVal );

   cVal = ramp.GetValue( 11.0f );
   CHECK_EQUAL( 5.0f, cVal );

   cVal = ramp.GetValue( 10.0f );
   CHECK_EQUAL( 6.0f, cVal );
}
/*
TEST( RampStringCast )
{
    JString strVal;
    cast( strVal, ramp );
    CHECK( strVal.size() != 0 );
    Ramp nramp;
    cast( nramp, strVal );
    CHECK( ramp == nramp );
}

TEST( RampSerialization )
{
    MemOutStream os;
    os << ramp;

    os.Close();

    MemInStream is( os.GetBuffer() );
    Ramp nramp;
    is >> nramp;
    CHECK( ramp == nramp );
}
*/

TEST( RampEquality )
{
    CHECK( ramp == ramp );
}


