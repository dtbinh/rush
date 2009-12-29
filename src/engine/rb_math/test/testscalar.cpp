#include "stdafx.h"
#include "Scalar.h"

TEST( TestFabs )
{
    CHECK( _fabs( 6.17f ) == 6.17f );
    CHECK( _fabs( -0.31f ) == 0.31f );
    CHECK( _fabs( -0.0f ) == 0.0f );
}

TEST( EqualF )
{
    CHECK( equalf( 0.0f, 0.0f ) );
    CHECK( equalf( 0.01f, 0.01f ) );
    CHECK( equalf( 100.0f, 100.0f ) );
    CHECK( !equalf( 1.0f, 0.0f ) );
    CHECK( !equalf( 0.01f, 0.02f ) );
    CHECK( !equalf( 1.0f, -1.0f ) );
}

