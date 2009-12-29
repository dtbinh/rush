#include "stdafx.h"
#include "JReflect.h"

class Parachute : public JObject
{
public:
    float       m_Diameter;
    JString     m_Type;

    Parachute() 
        :   m_Diameter( 10.0f ),
            m_Type( "Square" )
    {
    }

    expose(Parachute)
    {
        parent(JObject);
        field( "Diameter", m_Diameter );
        field( "Type", m_Type );
    }
};
decl_class(Parachute);


TEST( RTTI )
{
    link_class(Parachute);
    JMetaClass* pObjMeta = JObject::s_pMetaClass;
    CHECK( pObjMeta != NULL );
    CHECK( !stricmp( pObjMeta->GetName(), "object" ) );

    JMetaClass* pPMeta = Parachute::s_pMetaClass;
    CHECK( pPMeta != NULL );
    CHECK( pPMeta != pObjMeta );
    CHECK( !stricmp( pPMeta->GetName(), "Parachute" ) );
    CHECK( pPMeta->GetParent() == pObjMeta );

    JObject* pObj = new JObject();
    Parachute* pPara = new Parachute();

    CHECK_EQUAL( true,  is_a<JObject>( pObj ) );
    CHECK_EQUAL( true,  is_a<JObject>( pPara ) );
    CHECK_EQUAL( false, is_a<Parachute>( pObj ) );
    CHECK_EQUAL( true,  is_a<Parachute>( pPara ) );

    CHECK_EQUAL( true, is_exact<JObject>( pObj ) );
    CHECK_EQUAL( true, is_exact<Parachute>( pPara ) );
    CHECK_EQUAL( false, is_exact<JObject>( pPara ) );

    CHECK_EQUAL( false, is_a<JObject>( NULL ) );
    CHECK_EQUAL( false, is_exact<JObject>( NULL ) );
    CHECK_EQUAL( false, is_a<Parachute>( NULL ) );
    CHECK_EQUAL( false, is_exact<Parachute>( NULL ) );

    CHECK( NULL == obj_cast<Parachute>( pObj ) );
    JObject* pParaObj = obj_cast<JObject>( pPara );
    CHECK( pParaObj != NULL );
    CHECK( pParaObj->GetMetaClass() == Parachute::s_pMetaClass );

}