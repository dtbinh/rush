#include "stdafx.h"
#include "UniformPool.h"

const int c_Constructed = 13;
const int c_Destructed  = 14;

struct Jaguar
{
    std::string     m_Color;
    int             m_State;

    Jaguar()  
    {
        assert( m_State != c_Constructed );
        m_State = c_Constructed;
    }
    ~Jaguar() 
    { 
        assert( m_State == c_Constructed ); 
        m_State = c_Destructed; 
    }
};

TEST( UniformPool )
{
    UniformPool<Jaguar> pool;
    CHECK_EQUAL( 0, pool.GetNElem() );
    
    Jaguar* p1 = pool.Allocate();
    p1->m_Color = "brown";
    CHECK_EQUAL( c_Constructed, p1->m_State );

    Jaguar* p2 = pool.Allocate();
    p2->m_Color = "green";
    CHECK_EQUAL( c_Constructed, p2->m_State );

    CHECK_EQUAL( 2, pool.GetNElem() );

    Jaguar* p3 = pool.Allocate();
    p3->m_Color = "black";
    CHECK_EQUAL( c_Constructed, p3->m_State );

    CHECK_EQUAL( "brown", p1->m_Color );
    CHECK_EQUAL( "green", p2->m_Color );

    pool.Free( p1 );
    CHECK_EQUAL( 2, pool.GetNElem() );

    Jaguar* p4 = pool.Allocate();
    p4->m_Color = "red";
    CHECK_EQUAL( c_Constructed, p4->m_State );

    CHECK_EQUAL( "green", p2->m_Color );
    CHECK_EQUAL( "black", p3->m_Color );
    CHECK_EQUAL( "red", p4->m_Color );

    CHECK_EQUAL( 3, pool.GetNElem() );
}