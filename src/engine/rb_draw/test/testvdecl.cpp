#include "precompile.h"
#include "vertexdeclaration.h"

TEST( VDeclIsCreated )
{
    VertexDeclaration vdecl = GetStockVDecl( VertexType_XYZNUV );
    CHECK_EQUAL( 32, vdecl.m_VertexSize );

    vdecl.Clear();
    vdecl << VertexComponent_TexCoor0 << VertexComponent_TexCoor1;
    CHECK_EQUAL( 16, vdecl.m_VertexSize );

    vdecl.Clear();
    vdecl << VertexElement( VertexComponent_Position, VCompType_Float3 ) <<
             VertexElement( VertexComponent_TexCoor0, VCompType_Float4 ) <<
             VertexComponent_TexCoor1;
    CHECK_EQUAL( 36, vdecl.m_VertexSize );

    int offset = 0;
    int size   = 0;
    bool bRes = vdecl.GetElementParam( VertexComponent_TexCoor0, offset, size );
    CHECK_EQUAL( true, bRes );
    CHECK_EQUAL( 12, offset );
    CHECK_EQUAL( 16, size );

    vdecl.Clear();
    vdecl << VertexComponent_Position <<
             VertexComponent_Diffuse  <<
             VertexComponent_Normal   <<
             VertexComponent_TexCoor0 <<
             VertexElement( VertexComponent_Blend,    VCompType_Float4 ) <<
             VertexElement( VertexComponent_BlendIdx, VCompType_UByte4 );

}
