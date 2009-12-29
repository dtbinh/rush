#include "stdafx.h"
#include "IPersistServer.h"
#include "JObject.h"

class Crudget : public JObject
{
public:
    Crudget() {}
    
    expose(Crudget)
    {
        parent( JObject );
        field( "width",  m_Width  );
        field( "heigth", m_Height );
        field( "color",  m_Color  );
        field( "text",   m_Text   );
    }

    float       m_Width; 
    float       m_Height;
    DWORD       m_Color; 
    JString     m_Text;  
}; // class Crudget
decl_class(Crudget);

TEST( Templates )
{
    link_class(Crudget);
    JObject* pRoot = g_pPersistServer->LoadObject( "test_templates" );
    CHECK( pRoot != NULL );

    JObject* pBtn1 = g_pObjectServer->FindObject( "btn1", pRoot );
    JObject* pBtn2 = g_pObjectServer->FindObject( "btn2", pRoot );

    CHECK( pBtn1 != NULL );
    CHECK( pBtn2 != NULL );

    JString prop;
    CHECK_EQUAL( true, pBtn1->GetProperty( "mycolor", prop ) );
    CHECK( !stricmp( prop.c_str(), "0xFFFF0000" ) );
    CHECK_EQUAL( true, pBtn1->GetProperty( "mytext", prop ) );
    CHECK( !stricmp( prop.c_str(), "mytext_btn1" ) );
    CHECK_EQUAL( true, pBtn1->GetProperty( "mywidth", prop ) );
    CHECK( !stricmp( prop.c_str(), "40" ) );
	CHECK_EQUAL( true, pBtn1->GetProperty( "myheight", prop ) );
    CHECK( !stricmp( prop.c_str(), "20" ) );

    CHECK_EQUAL( true, pBtn2->GetProperty( "mycolor", prop ) );
    CHECK( !stricmp( prop.c_str(), "0xFFFF00FF" ) );
    CHECK_EQUAL( true, pBtn2->GetProperty( "mytext", prop ) );
    CHECK( !stricmp( prop.c_str(), "mytext_btn2" ) );
    CHECK_EQUAL( true, pBtn2->GetProperty( "mywidth", prop ) );
    CHECK( !stricmp( prop.c_str(), "20" ) );
	CHECK_EQUAL( true, pBtn2->GetProperty( "myheight", prop ) );
    CHECK( !stricmp( prop.c_str(), "20" ) );

    JObject* pPic1 = g_pObjectServer->FindObject( "objpic", pBtn1 );
    JObject* pPic2 = g_pObjectServer->FindObject( "objpic", pBtn2 );
    JObject* pTxt1 = g_pObjectServer->FindObject( "objtext", pBtn1 );
    JObject* pTxt2 = g_pObjectServer->FindObject( "objtext", pBtn2 );
    CHECK( pPic1 != NULL && pPic2 != NULL && pTxt1 != NULL && pTxt2 != NULL );

    CHECK_EQUAL( true, pPic1->GetProperty( "color", prop ) );
    CHECK( !stricmp( prop.c_str(), "0xFFFF0000" ) );
    CHECK_EQUAL( true, pTxt1->GetProperty( "text", prop ) );
    CHECK( !stricmp( prop.c_str(), "mytext_btn1" ) );
    
    CHECK_EQUAL( true, pPic2->GetProperty( "color", prop ) );
    CHECK( !stricmp( prop.c_str(), "0xFFFF00FF" ) );
    CHECK_EQUAL( true, pTxt2->GetProperty( "text", prop ) );
    CHECK( !stricmp( prop.c_str(), "mytext_btn2" ) );
}
