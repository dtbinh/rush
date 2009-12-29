#include "stdafx.h"
#include "MemStream.h"
#include "JCore.h"
#include "JObjectServer.h"
#include "JObject.h"

class ClassA : public JObject
{
    int         m_nVal;
    float       m_fVal;
    JString     m_strVal;

public:
    ClassA()
        :   m_nVal(-11),
            m_fVal(0.5f),
            m_strVal("hello!")
    {}

    expose(ClassA)
    {
        parent(JObject);
        field( "nVal", m_nVal );
        field( "fVal", m_fVal );
        field( "strVal", m_strVal );
    }
};
decl_class(ClassA);

class ClassB : public JObject
{
public:
    bool        m_bNonExposedVal;

    void        Toggle() { m_bNonExposedVal = !m_bNonExposedVal; }

    ClassB() : m_bNonExposedVal( false ) {}

    expose(ClassB)
    {
        parent(JObject);
        method( "Toggle", Toggle );
    }
};
decl_class(ClassB);

class ClassBA : public ClassB
{
public:
    bool        m_bVal1;
    bool        m_bVal2;

    ClassBA() 
        :   m_bVal1( true ),
            m_bVal2( false )
    {
    }

    expose(ClassBA)
    {
        parent(ClassB);
        field( "Val1", m_bVal1 );
        field( "Val2", m_bVal2 );
    }
};
decl_class(ClassBA);


TEST( ParentMetaClasses )
{
    link_class(ClassA);
    link_class(ClassB);
    link_class(ClassBA);

    JMetaClass* pMetaA = g_pObjectServer->FindClass( "ClassA" );
    JMetaClass* pMetaB = g_pObjectServer->FindClass( "ClassB" );
    JMetaClass* pMetaBA = g_pObjectServer->FindClass( "ClassBA" );

    CHECK( pMetaA );
    CHECK( pMetaB );
    CHECK( pMetaBA );

    CHECK( !stricmp( pMetaA->GetName(), "ClassA" ) );
    CHECK( !stricmp( pMetaB->GetName(), "ClassB" ) );
    CHECK( !stricmp( pMetaBA->GetName(), "ClassBA" ) );
    CHECK( !stricmp( pMetaBA->GetParentName(), "ClassB" ) );
    CHECK( !stricmp( pMetaB->GetParentName(), "Object" ) );
    CHECK( !stricmp( pMetaA->GetParentName(), "Object" ) );
    CHECK( pMetaA->GetParent() );
    CHECK( pMetaB->GetParent() );
    CHECK( pMetaBA->GetParent() );

    JMetaClass* pObjMeta = g_pObjectServer->FindClass( "JObject" );
    CHECK( pObjMeta );
    CHECK( pObjMeta->GetParent() == NULL );
    CHECK( !strcmp( pObjMeta->GetParentName(), "" ) );
    CHECK( !strcmp( pObjMeta->GetName(), "object" ) );

    int nObjAttr = pObjMeta->GetNAttr();
    int nAttrA = pMetaA->GetNAttr();
    int nAttrB = pMetaB->GetNAttr();
    int nAttrBA = pMetaBA->GetNAttr();

    CHECK( nObjAttr > 0 );
    CHECK_EQUAL( nObjAttr + 3, nAttrA );
    CHECK_EQUAL( nObjAttr + 1, nAttrB );
    CHECK_EQUAL( nAttrB + 2, nAttrBA );

    JAttrIterator it( pMetaBA );    
    int nAttr = 0;
    CHECK( it != false );
    CHECK( (*it) != NULL );
    CHECK( !stricmp( (*it)->GetName(), "Val1" ) );
    ++it;
    CHECK( it != false );
    CHECK( (*it) != NULL );
    CHECK( !stricmp( (*it)->GetName(), "Val2" ) );
    ++it;
    CHECK( it != false );
    CHECK( (*it) != NULL );
    CHECK( !stricmp( (*it)->GetName(), "Toggle" ) );
    ++it;

    CHECK( it != false );
    CHECK( (*it) != NULL );
    CHECK( !stricmp( (*it)->GetName(), "Name" ) );
    for (int i = 0; i < nObjAttr; i++) ++it;
    CHECK( it == false );
    CHECK( (*it) == NULL );
    
    JClassAttr* pAttr1 = pMetaA->FindAttr( "strVal" );
    JClassAttr* pAttr2 = pMetaA->FindAttr( "name" );
    
    CHECK( pAttr1 != NULL );
    CHECK( pAttr2 != NULL );
    CHECK( !stricmp( pAttr1->GetName(), "strVal" ) );
    CHECK( !stricmp( pAttr2->GetName(), "name" ) );

    JClassAttr* pAttr3 = pMetaBA->FindAttr( "name" );
    CHECK_EQUAL( pAttr2, pAttr3 );

}

TEST( ObjectFactory )
{
    ClassBA* pBA  = (ClassBA*)g_pObjectServer->Create( "ClassBA" );
    ClassA*  pA   = (ClassA*)g_pObjectServer->Create( "ClassA" );
    ClassB*  pB   = (ClassB*)g_pObjectServer->Create( "ClassB" );
    JObject* pObj = (JObject*)g_pObjectServer->Create( "JObject" );

    CHECK( pBA  != NULL );
    CHECK( pA   != NULL );
    CHECK( pB   != NULL );
    CHECK( pObj != NULL );

    CHECK( !stricmp( pBA->ClassName(),  "classba" ) );
    CHECK( !stricmp( pA->ClassName(),   "classa" ) );
    CHECK( !stricmp( pB->ClassName(),   "classb" ) );
    CHECK( !stricmp( pObj->ClassName(), "object" ) );

    pBA->Release(); 
    pA->Release();  
    pB->Release();  
    pObj->Release();
}

TEST( MetaClassProperties )
{
    ClassBA* pBA = (ClassBA*)g_pObjectServer->Create( "ClassBA" );
    CHECK( pBA != NULL );

    bool bRes = pBA->SetProperty( "name", "CherryPie" );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( 0, strcmp( pBA->GetName(), "CherryPie" ) );
    
    JString val;
    bRes = pBA->GetProperty( "name", val );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( 0, strcmp( val.c_str(), "CherryPie" ) );

    val = "";
    bRes = pBA->GetProperty( "Name", val );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( 0, strcmp( val.c_str(), "CherryPie" ) );
    pBA->SetName( "ApplePie" );
    bRes = pBA->GetProperty( "Name", val );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( 0, strcmp( val.c_str(), "ApplePie" ) );

    bRes = pBA->GetProperty( "Val1", val );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( 0, strcmp( val.c_str(), "true" ) );
    CHECK_EQUAL( true, pBA->m_bVal1 );
    pBA->m_bVal1 = false;
    bRes = pBA->GetProperty( "Val1", val );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( 0, strcmp( val.c_str(), "false" ) );

    pBA->Release();
}
