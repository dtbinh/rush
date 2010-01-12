#include "precompile.h"
#include "RegPool.h"

TEST( RegPool )
{
    RegPool<std::string> pool;
    pool.reserve_ids( 1 );
    int id1 = pool.alloc();
    *(pool[id1]) = "first";
    CHECK_EQUAL( 1, id1 );
    CHECK_EQUAL( 1, pool.size() );
    CHECK( NULL == pool[5] );
    CHECK( !strcmp( pool[id1]->c_str(), "first" ) );

    int id2 = pool.alloc();
    *(pool[id2]) = "second";
    CHECK( !strcmp( pool[id1]->c_str(), "first" ) );
    CHECK( !strcmp( pool[id2]->c_str(), "second" ) );
    CHECK_EQUAL( 2, pool.size() );
    CHECK_EQUAL( 2, id2 );

    bool bRes = pool.remove( id1 );
    bool bRes1 = pool.remove( 5 );
    bool bRes2 = pool.remove( id1 );
    CHECK_EQUAL( 1, pool.size() );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( false, bRes1 );
    CHECK_EQUAL( false, bRes2 );
    CHECK( NULL == pool[id1] );

    int id3 = pool.alloc();
    *(pool[id3]) = "third";
    CHECK( !strcmp( pool[id3]->c_str(), "third" ) );
    CHECK( !strcmp( pool[id2]->c_str(), "second" ) );
    CHECK( NULL == pool[5] );
    CHECK_EQUAL( 2, pool.size() );

    RegPool<std::string>::iterator it = pool.begin();
    CHECK( it != pool.end() );
    CHECK( !strcmp( (*it)->c_str(), "second" ) );
    ++it;
    CHECK( !strcmp( (*it)->c_str(), "third" ) );
    ++it;
    CHECK( it == pool.end() );

    pool.clear();
    CHECK( NULL == pool[id3] );
    CHECK( NULL == pool[id2] );
    CHECK( NULL == pool[5] );
    CHECK_EQUAL( 0, pool.size() );
}