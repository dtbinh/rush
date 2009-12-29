#include "stdafx.h"
#include "commandline.h"

TEST(CommandLine)
{
    CommandLine cl( "some crap blah-blah" );
    CHECK_EQUAL( 0, cl.GetNParam() );

    CommandLine cl1( "some crap --blah --blah1=foo --blah3=\"c:/path\" --somepath=c:/path" );
    CHECK_EQUAL( 4, cl1.GetNParam() );

    CHECK( cl1.GetValue( "blah" ) != NULL );
    CHECK( cl1.GetValue( "blah1" ) != NULL );
    CHECK( cl1.GetValue( "blah3" ) != NULL ); 
    CHECK( cl1.GetValue( "somepath" ) != NULL ); 
    CHECK( strlen( cl1.GetValue( "blah" ) ) == 0 );
    CHECK( !strcmp( cl1.GetValue( "blah1" ), "foo" ) );
    CHECK( !strcmp( cl1.GetValue( "blah3" ), "c:/path" ) );
    CHECK( !strcmp( cl1.GetValue( "blah3" ), cl1.GetValue( "somepath" ) ) );
}