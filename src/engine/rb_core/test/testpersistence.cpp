#include "stdafx.h"
#include "MemStream.h"
#include "JCore.h"
#include "JObjectServer.h"
#include "JSignalServer.h"
#include "IPersistServer.h"

TEST( Persistence )
{
    /*
    int nSig = JSignalServer::s_pInstance->GetNumSignals();
    JObject* pObj = obj_cast<JObject>( g_pPersistServer->LoadObject( "test_persistence" ) );
    CHECK( pObj != NULL );
    pObj->InitTree();

    int nSig1 = JSignalServer::s_pInstance->GetNumSignals();
    int nSigDelta = nSig1 - nSig;

    JObject* pBtn = g_pObjectServer->FindObject( "btnCompound", pObj );
    CHECK( pBtn != NULL );
    
    JObject* pNewObj = NULL;
    int nSig2 = 0;

    MemOutStream osJML;
    g_pPersistServer->Save( pObj, osJML, PersistFmt_JML );
    osJML << 0;
    osJML.Close();
    MemInStream isJML( osJML.GetBuffer(), osJML.GetTotalSize() );
    pNewObj = g_pPersistServer->Load( isJML, PersistFmt_JML );
    CHECK( pNewObj && pNewObj != pObj );
    pNewObj->InitTree();
    CHECK_EQUAL( true, g_pObjectServer->AreObjectsEqual( pObj, pNewObj, true ) );  
    nSig2 = JSignalServer::s_pInstance->GetNumSignals();
    CHECK_EQUAL( nSig1 + nSigDelta, nSig2 );

    MemOutStream osBin;
    g_pPersistServer->Save( pObj, osBin, PersistFmt_Bin );
    osBin.Close();
    MemInStream isBin( osBin.GetBuffer() );
    pNewObj = g_pPersistServer->Load( isBin, PersistFmt_Bin );
    CHECK( pNewObj && pNewObj != pObj );
    pNewObj->InitTree();
    CHECK_EQUAL( true, g_pObjectServer->AreObjectsEqual( pObj, pNewObj, true ) ); 
    int nSig3 = JSignalServer::s_pInstance->GetNumSignals();
    CHECK_EQUAL( nSig2 + nSigDelta, nSig3 );

    MemOutStream osXML;
    g_pPersistServer->Save( pObj, osXML, PersistFmt_XML );
    osXML << 0;
    osXML.Close();
    MemInStream isXML( osXML.GetBuffer(), osXML.GetTotalSize() );
    pNewObj = g_pPersistServer->Load( isXML, PersistFmt_XML );
    CHECK( pNewObj && pNewObj != pObj );
    pNewObj->InitTree();
    CHECK_EQUAL( true, g_pObjectServer->AreObjectsEqual( pObj, pNewObj, true ) ); 
    int nSig4 = JSignalServer::s_pInstance->GetNumSignals();
    CHECK_EQUAL( nSig3 + nSigDelta, nSig4 );

    pObj->Release();
    */
}
