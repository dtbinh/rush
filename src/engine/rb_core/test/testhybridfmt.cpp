#include "stdafx.h"
#include "MemStream.h"
#include "JObject.h"
#include "JObjectServer.h"
#include "IPersistServer.h"

class Tomahawk : public JObject
{
public:
    Tomahawk()
        :   m_Color (0xFFFFFFFF),
            m_Length(10.0f)
    {
        memset( m_Nums, 0, sizeof( m_Nums ) );
    }

    expose(Tomahawk)
    {
        parent(JObject);
        field( "color",     m_Color );
        field( "length",    m_Length );
    }

    virtual void Serialize( OutStream& os ) const
    {
        os.Write( (BYTE*)m_Nums, sizeof( m_Nums ) );
    }

    virtual bool Unserialize( InStream& is )
    {
        is.Read( (BYTE*)m_Nums, sizeof( m_Nums ) );
        return true;
    }

    DWORD       m_Color;
    float       m_Length;
    int         m_Nums[10];
};
decl_class(Tomahawk);

static const char* c_TestScript = "\
group \"g_root\"\n\
{\n\
    tomahawk \"tomaA\"\n\
    {\n\
        group \"nice\"\n\
        {\n\
            tomahawk \"tomaAA\"{}\n\
            group \"tomaAB\"{}\n\
            tomahawk \"tomaAC\"{}\n\
        }\n\
    }\n\
    group \"tomaB\"\n\
    {\n\
        group \"nice\"\n\
        {\n\
            tomahawk \"tomaAAA\"{}\n\
            tomahawk \"tomaAA_\"{}\n\
            group \"tomaBA\"{}\n\
            tomahawk \"tomaBB\"{}\n\
            group \"tomaBC\"{}\n\
        }\n\
    }\n\
}\n";

TEST(HybridFormat)
{
    link_class(Tomahawk);
    JObject* pRoot = g_pObjectServer->CreateFromScript( c_TestScript );
    CHECK( pRoot != NULL );

    Tomahawk* pTomaAA = g_pObjectServer->FindObject<Tomahawk>( "tomaAA", pRoot );
    CHECK( pTomaAA != NULL );

    Tomahawk* pTomaBB = g_pObjectServer->FindObject<Tomahawk>( "tomaBB", pRoot );
    CHECK( pTomaBB != NULL );

    pTomaAA->m_Nums[0] = 11;
    pTomaAA->m_Nums[9] = -3;

    pTomaBB->m_Nums[0] = 8;
    pTomaBB->m_Nums[9] = 5;

    Tomahawk* pCTomaAA = NULL;
    Tomahawk* pCTomaBB = NULL;
    bool bRes = false;

    //  XML
    MemOutStream xos;
    bRes = g_pPersistServer->Save( pRoot, xos, PersistFmt_XML );
    CHECK_EQUAL( true, bRes );
    xos << 0;
    xos.Close();
    
    MemInStream xis( xos.GetBuffer(), xos.GetTotalSize() );
    JObject* pClonedRootXML = g_pPersistServer->Load( xis, PersistFmt_XML );
    CHECK( pClonedRootXML != NULL );

    pCTomaAA = g_pObjectServer->FindObject<Tomahawk>( "tomaAA", pClonedRootXML );
    CHECK( pCTomaAA != NULL );
    CHECK( pCTomaAA != pTomaAA );

    pCTomaBB = g_pObjectServer->FindObject<Tomahawk>( "tomaBB", pClonedRootXML );
    CHECK( pCTomaBB != NULL );
    CHECK( pCTomaBB != pTomaBB );

    CHECK_EQUAL( 11, pCTomaAA->m_Nums[0] );
    CHECK_EQUAL( -3, pCTomaAA->m_Nums[9] );

    CHECK_EQUAL( 8, pCTomaBB->m_Nums[0] );
    CHECK_EQUAL( 5, pCTomaBB->m_Nums[9] );


    //  JML
    MemOutStream jos;
    bRes = g_pPersistServer->Save( pRoot, jos, PersistFmt_JML );
    CHECK_EQUAL( true, bRes );
    jos << 0;
    jos.Close();
    
    MemInStream jis( jos.GetBuffer(), jos.GetTotalSize() );
    JObject* pClonedRootJML = g_pPersistServer->Load( jis, PersistFmt_JML );
    CHECK( pClonedRootJML != NULL );

    pCTomaAA = g_pObjectServer->FindObject<Tomahawk>( "tomaAA", pClonedRootJML );
    CHECK( pCTomaAA != NULL );
    CHECK( pCTomaAA != pTomaAA );

    pCTomaBB = g_pObjectServer->FindObject<Tomahawk>( "tomaBB", pClonedRootJML );
    CHECK( pCTomaBB != NULL );
    CHECK( pCTomaBB != pTomaBB );

    CHECK_EQUAL( 11, pCTomaAA->m_Nums[0] );
    CHECK_EQUAL( -3, pCTomaAA->m_Nums[9] );

    CHECK_EQUAL( 8, pCTomaBB->m_Nums[0] );
    CHECK_EQUAL( 5, pCTomaBB->m_Nums[9] );


    //  Bin
    MemOutStream bos;
    bRes = g_pPersistServer->Save( pRoot, bos, PersistFmt_XML );
    CHECK_EQUAL( true, bRes );
    bos << 0;
    bos.Close();
    
    MemInStream bis( bos.GetBuffer(), bos.GetTotalSize() );
    JObject* pClonedRootBin = g_pPersistServer->Load( bis, PersistFmt_XML );
    CHECK( pClonedRootBin != NULL );

    pCTomaAA = g_pObjectServer->FindObject<Tomahawk>( "tomaAA", pClonedRootBin );
    CHECK( pCTomaAA != NULL );
    CHECK( pCTomaAA != pTomaAA );

    pCTomaBB = g_pObjectServer->FindObject<Tomahawk>( "tomaBB", pClonedRootBin );
    CHECK( pCTomaBB != NULL );
    CHECK( pCTomaBB != pTomaBB );

    CHECK_EQUAL( 11, pCTomaAA->m_Nums[0] );
    CHECK_EQUAL( -3, pCTomaAA->m_Nums[9] );

    CHECK_EQUAL( 8, pCTomaBB->m_Nums[0] );
    CHECK_EQUAL( 5, pCTomaBB->m_Nums[9] );
    
    

    pClonedRootJML->Release();
    pClonedRootXML->Release();
    pClonedRootBin->Release();
    pRoot->Release();
}