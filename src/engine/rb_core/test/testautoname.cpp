#include "precompile.h"
#include "jreflect.h"
#include "jgroup.h"

class Cherokee : public JObject
{
public:
    expose(Cherokee)
    {
        parent(JObject);
    }
};
decl_class(Cherokee);

TEST( AutoName )
{
    link_class( Cherokee );
    Cherokee* pObj1 = new Cherokee();
    Cherokee* pObj2 = new Cherokee();
    Cherokee* pObj3 = new Cherokee();

    pObj1->SetName( "" );
    pObj2->SetName( "" );
    pObj3->SetName( "" );

    CHECK( !strcmp( pObj1->GetName(), "cherokee00" ) );
    CHECK( !strcmp( pObj2->GetName(), "cherokee01" ) );
    CHECK( !strcmp( pObj3->GetName(), "cherokee02" ) );
    delete pObj3;
    Cherokee* pObj4 = new Cherokee();
    CHECK( !strcmp( pObj4->GetName(), "" ) );
    pObj4->SetName( "" ); 
    CHECK( !strcmp( pObj4->GetName(), "cherokee02" ) );

    JGroup* pGroup = new JGroup();
    pGroup->SetName( "" );
    CHECK( !strncmp( pGroup->GetName(), "group", strlen( "group" ) ) );

    delete pObj1;
    delete pObj2;
    delete pObj4;
    delete pGroup;
}