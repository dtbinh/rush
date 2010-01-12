#include "precompile.h"
#include "jreflect.h"

class Clown : public JObject
{
public:
    bool        m_bFunny;
    bool        m_bSmart;
    DWORD       m_bHairColor;
    int         m_Salary;
    float       m_Weight;
    JString     m_NickName;

    Clown() 
        :   m_bFunny( false ),
            m_bSmart( true ),
            m_bHairColor( 0xFF000000 ),
            m_Salary( 400 ),
            m_Weight( 65.6f ),
            m_NickName( "Junkie" )
    {
    }

    expose(Clown)
    {
        parent(JObject);
        field( "Funny",     m_bFunny    );
        field( "Smart",     m_bSmart    );
        field( "HairColor", m_bHairColor );
        field( "Salary",    m_Salary    );
        field( "Height",    m_Weight    );
        field( "NickName",  m_NickName  );
    }
};
decl_class(Clown);

TEST( ObjectsAreShallowClonedProperly )
{
    link_class(Clown);
    Clown* pA = new Clown();
    pA->m_bFunny = true;
    pA->m_bHairColor = 0xFFFFFF00;
    pA->m_Weight = 70.1f;
    Clown* pB = (Clown*)pA->Clone( NULL, "" );
    pA->m_Salary = 200;
    CHECK( pB != NULL );
    CHECK_EQUAL( pB->m_bFunny, true );
    CHECK_EQUAL( pB->m_bHairColor, 0xFFFFFF00 );
    CHECK_EQUAL( pB->m_Weight, 70.1f );
    CHECK_EQUAL( pB->m_Salary, 400 );
    CHECK_EQUAL( pA->m_Salary, 200 );
    pB->m_Weight = 20.0f;
    CHECK_EQUAL( pB->m_Weight, 20.0f );
    CHECK_EQUAL( pA->m_Weight, 70.1f );
    pB->Release();
    pA->Release();
}

TEST( ObjectsAreDeepClonedProperly )
{
    Clown* pA  = new Clown();
    pA->m_NickName = "A";

    Clown* pAA = new Clown();
    pAA->m_NickName = "AA";

    Clown* pAB = new Clown();
    pAB->m_NickName = "AB";

    Clown* pAC = new Clown();
    pAC->m_NickName = "AC";

    Clown* pACA = new Clown();
    pACA->m_NickName = "ACA";

    Clown* pACB = new Clown();
    pACB->m_NickName = "ACB";

    Clown* pACC = new Clown();
    pACC->m_NickName = "ACC";

    Clown* pAD = new Clown();
    pAD->m_NickName = "AD";

    pA->AddChild( pAA );
    pA->AddChild( pAB );
    pA->AddChild( pAC );
    pA->AddChild( pAD );
    pAC->AddChild( pACA );
    pAC->AddChild( pACB );
    pAC->AddChild( pACC );
    
    Clown* pB = (Clown*)pA->Clone( NULL, "" );
    CHECK( pB != NULL );
    CHECK( pA != NULL );
    CHECK_EQUAL( 4, pA->GetNChildren() );
    CHECK_EQUAL( 4, pB->GetNChildren() );
    CHECK( pB->m_NickName == "A" );
    Clown* pBC = (Clown*)pB->GetChild( 2 );
    CHECK( pBC->m_NickName == "AC" );
    CHECK_EQUAL( 3, pBC->GetNChildren() );

    pA->Release();
    pB->Release();
}