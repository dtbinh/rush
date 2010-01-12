#include "precompile.h"
#include "jobject.h"
#include "jstring.h"
#include "jreflect.h"
#include "jmetaclass.h"
#include "jobjectserver.h"

#define struct_beg( CName ) namespace CName##_Reg{ typedef CName MyType;\
                                void Expose(){\
                                JMetaClass& mc = *g_pObjectServer->RegClass( #CName );\
                                CName templ;\

#define sfield( PName, Val ) mc.Field( PName, templ.Val, int( &((MyType*)NULL)->Val ) );

#define struct_end }}

#define link_struct( CName ) void CName##_Reg::Expose(); CName##_Reg::Expose();
//
//struct_beg(Vec2)
//    sfield( "x", x );
//    sfield( "y", y );
//struct_end
//
//struct_beg(Vec3)
//    sfield( "x", x );
//    sfield( "y", y );
//    sfield( "z", z );
//struct_end
//
//struct_beg(Vec4)
//    sfield( "x", x );
//    sfield( "y", y );
//    sfield( "z", z );
//    sfield( "w", w );
//struct_end
//
//struct_beg(Frame)
//    sfield( "x", x );
//    sfield( "y", y );
//    sfield( "w", w );
//    sfield( "h", h );
//struct_end
//
//struct_beg(Mat3)
//    sfield( "e00", e00 );
//    sfield( "e01", e01 );
//    sfield( "e02", e02 );
//    sfield( "e10", e10 );
//    sfield( "e11", e11 );
//    sfield( "e12", e12 );
//    sfield( "e20", e20 );
//    sfield( "e21", e21 );
//    sfield( "e22", e22 );
//struct_end
//
//struct_beg(Mat4)
//    sfield( "e00", e00 );
//    sfield( "e01", e01 );
//    sfield( "e02", e02 );
//    sfield( "e03", e03 );
//    sfield( "e10", e10 );
//    sfield( "e11", e11 );
//    sfield( "e12", e12 );
//    sfield( "e13", e13 );
//    sfield( "e20", e20 );
//    sfield( "e21", e21 );
//    sfield( "e22", e22 );
//    sfield( "e23", e23 );
//struct_end
//
//TEST( PODTypesAreDeclared )
//{
//    link_struct( Vec2 );
//    link_struct( Vec3 );
//    link_struct( Vec4 );
//    link_struct( Frame );
//    link_struct( Mat3 );
//    link_struct( Mat4 );
//
//    JMetaClass* pMetaVec2 = g_pObjectServer->FindClass( "vec2" );
//    JMetaClass* pMetaVec3 = g_pObjectServer->FindClass( "vec3" );
//    JMetaClass* pMetaVec4 = g_pObjectServer->FindClass( "vec4" );
//    JMetaClass* pMetaMat3 = g_pObjectServer->FindClass( "Mat3" );
//    JMetaClass* pMetaMat4 = g_pObjectServer->FindClass( "Mat4" );
//
//    CHECK( pMetaVec2 != NULL );
//    CHECK( pMetaVec3 != NULL );
//    CHECK( pMetaVec4 != NULL );
//    CHECK( pMetaMat3 != NULL );
//    CHECK( pMetaMat4 != NULL );
//
//    CHECK( pMetaVec2 != pMetaVec3 );
//    CHECK( pMetaVec3 != pMetaVec4 );
//    CHECK( pMetaVec4 != pMetaMat3 );
//    CHECK( pMetaMat3 != pMetaMat4 );
//    CHECK( pMetaMat4 != pMetaVec2 );
//    
//    JClassAttr* pX = pMetaVec2->FindAttr( "x" );
//    JClassAttr* pY = pMetaVec2->FindAttr( "y" );
//    JClassAttr* pZ = pMetaVec2->FindAttr( "z" );
//    CHECK( pX != NULL );
//    CHECK( pY != NULL );
//    CHECK( pZ == NULL );
//
//    pX = pMetaVec3->FindAttr( "x" );
//    pY = pMetaVec3->FindAttr( "y" );
//    pZ = pMetaVec3->FindAttr( "z" );
//    CHECK( pX != NULL );
//    CHECK( pY != NULL );
//    CHECK( pZ != NULL );
//}
//
//
//class Helicopter : public JObject
//{
//public:
//    Mat3        m_RotorTM;
//    Mat4        m_TM;
//    Vec3        m_Velocity;
//    Frame       m_ScreenExt;
//
//    Helicopter() 
//    {
//        m_RotorTM   = Mat3( -1.0f,  2.0f, -3.0f, 
//                             4.0f, -5.0f,  6.0f, 
//                            -7.0f,  8.0f, -9.0f );
//        m_TM        = Mat4( 1.0f, 0.0f, 0.0f, 0.0f, 
//                            0.0f, 1.0f, 0.0f, 0.0f, 
//                            0.0f, 0.0f, 1.0f, 0.0f, 
//                            1.0f, 2.0f, 3.0f, 1.0f );
//        m_Velocity  = Vec3( 100.0f, 200.0f, 300.0f );
//        m_ScreenExt = Frame( 10.0f, -1.0f, 5.0f, 6.0f );
//    }
//
//    expose(Helicopter)
//    {
//        parent(JObject);
//        field( "RotorTM",   m_RotorTM   );
//        field( "TM",        m_TM        );
//        field( "Velocity",  m_Velocity  );
//        field( "ScreenExt", m_ScreenExt );
//    }
//};
//decl_class(Helicopter);
//
//TEST( PODTypesAreGet )
//{
//    link_class(Helicopter);
//
//    Helicopter* pHeli = new Helicopter();
//
//    JString valx, valy, valz;
//    bool bResx, bResy, bResz, bResa, bResb;
//    
//    bResx = pHeli->GetProperty( "velocity.x", valx );
//    bResy = pHeli->GetProperty( "velocity.y", valy );
//    bResz = pHeli->GetProperty( "velocity.z", valz );
//    bResa = pHeli->GetProperty( "velocity.a", valz );
//    bResb = pHeli->GetProperty( "pelocity.z", valz );
//    
//    CHECK_EQUAL( true, bResx );
//    CHECK_EQUAL( true, bResy );
//    CHECK_EQUAL( true, bResz );
//    CHECK_EQUAL( false, bResa );
//    CHECK_EQUAL( false, bResb );
//    
//    Vec3 vel = Vec3::null;
//    bResx = cast( vel.x, valx );
//    bResy = cast( vel.y, valy );
//    bResz = cast( vel.z, valz );
//
//    CHECK_EQUAL( true, bResx );
//    CHECK_EQUAL( true, bResy );
//    CHECK_EQUAL( true, bResz );
//    
//    CHECK_EQUAL( 100.0f, vel.x );
//    CHECK_EQUAL( 200.0f, vel.y );
//    CHECK_EQUAL( 300.0f, vel.z );
//    
//    pHeli->Release();
//}
//
//TEST( PODTypesAreSet )
//{
//    link_class(Helicopter);
//
//    Helicopter* pHeli = new Helicopter();
//    
//    bool bResx = pHeli->SetProperty( "ScreenExt.x", "-3.0f" );
//    bool bResy = pHeli->SetProperty( "screenExt.y", "2.5" );
//    bool bResw = pHeli->SetProperty( "Screenext.w", "-1" );
//    bool bResh = pHeli->SetProperty( "Screenext.h", "15" );
//    
//    CHECK_EQUAL( true, bResx );
//    CHECK_EQUAL( true, bResy );
//    CHECK_EQUAL( true, bResh );
//    CHECK_EQUAL( true, bResw );
//    
//    CHECK_EQUAL( -3.0f, pHeli->m_ScreenExt.x );
//    CHECK_EQUAL( 2.5,   pHeli->m_ScreenExt.y );
//    CHECK_EQUAL( -1,    pHeli->m_ScreenExt.w );
//    CHECK_EQUAL( 15,    pHeli->m_ScreenExt.h );
//
//    bool bRes = pHeli->SetProperty( "ScreenExt", "{ 10, 2, 0, 1 }" );
//    CHECK_EQUAL( true, bRes );
//    
//    CHECK_EQUAL( 10, pHeli->m_ScreenExt.x );
//    CHECK_EQUAL( 2,  pHeli->m_ScreenExt.y );
//    CHECK_EQUAL( 0,  pHeli->m_ScreenExt.w );
//    CHECK_EQUAL( 1,  pHeli->m_ScreenExt.h );
//
//    pHeli->Release();
//}
