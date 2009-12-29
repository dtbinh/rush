#include "stdafx.h"
#include "JObjectInspector.h"

class Melon : public JObject
{
public:
    Melon()
        :   m_Color (0xFFFFFF00),
            m_Radius(10.0f)
    {
    }

    expose(Melon)
    {
        parent(JObject);
        field( "color",     m_Color  );
        field( "radius",    m_Radius );
    }

    DWORD       m_Color;
    float       m_Radius;
};
decl_class(Melon);

TEST( ObjectInspector )
{    
    JObjectInspector* pInspector = new JObjectInspector();
    CHECK( pInspector != NULL );
    pInspector->Init();

    link_class( Melon );
    Melon* pMelon = new Melon();
    pInspector->SetObject( pMelon );
    CHECK_EQUAL( pInspector->GetObject(), pMelon );

    pInspector->Release();
    pMelon->Release();
}
