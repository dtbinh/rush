//****************************************************************************/
//  File:  VertexDeclaration.h
//****************************************************************************/
#ifndef __VERTEXDECLARATION_H__
#define __VERTEXDECLARATION_H__

#include "VertexType.h"

const int c_MaxTextureCoordinates   = 8;  
const int c_MaxBlendWeights         = 4;
//****************************************************************************/
/*  Enum:   VertexComponent
//  Desc:   Describes semantics of the vertex component value
//****************************************************************************/
enum VertexComponent
{
    VertexComponent_Unknown         = 0x0,

    VertexComponent_Position        = 0x00000001,    //  vertex position    
    VertexComponent_PositionRHW     = 0x00000002,    //  vertex position with reciprocal homogeneous w component
   
    VertexComponent_Blend0          = 0x00000004,    //  blending weights
    VertexComponent_Blend1          = 0x00000008,
    VertexComponent_Blend2          = 0x00000010,
    VertexComponent_Blend3          = 0x00000020,
    VertexComponent_BlendIdx        = 0x00000040,    //  grouped blend indices      
    VertexComponent_BlendIdx0       = 0x00000080,           
    VertexComponent_BlendIdx1       = 0x00000100,           
    VertexComponent_BlendIdx2       = 0x00000200,           
    VertexComponent_BlendIdx3       = 0x00000400,       
    
    VertexComponent_Normal          = 0x00000800,    //  normal at the vertex 
    VertexComponent_Diffuse         = 0x00001000,    //  diffuse color component 
    VertexComponent_Specular        = 0x00002000,    //  specular color component 
    
    VertexComponent_TexCoor0        = 0x00004000,    //  texture coordinates
    VertexComponent_TexCoor1        = 0x00008000,
    VertexComponent_TexCoor2        = 0x00010000,
    VertexComponent_TexCoor3        = 0x00020000,
    VertexComponent_TexCoor4        = 0x00040000,
    VertexComponent_TexCoor5        = 0x00080000,
    VertexComponent_TexCoor6        = 0x00100000,
    VertexComponent_TexCoor7        = 0x00200000,
    
    VertexComponent_Binormal        = 0x00300000,    //  binormal (oY axis)
    VertexComponent_Tangent         = 0x00400000,    //  tangent  (oX axis)
    
    VertexComponent_Blend           = 0x00800000,    //  grouped blend weights
    VertexComponent_Custom          = 0x01000000,    //  something defined by user

}; // enum VertexComponent

//****************************************************************************/
/*  Enum:    VCompType
//  Desc:   Describes type of the vertex component value
//****************************************************************************/
enum  VCompType
{
     VCompType_Unknown      = 0,
     VCompType_Float1       = 1,
     VCompType_Float2       = 2,
     VCompType_Float3       = 3,
     VCompType_Float4       = 4,
     VCompType_Color        = 5,
     VCompType_Short2       = 6,
     VCompType_Short4       = 7,
     VCompType_UByte4       = 8,
}; // enum  VCompType 

inline int GetVertexElementSize( VCompType type )
{
    switch (type)
    {
    case  VCompType_Float1: return 4;
    case  VCompType_Float2: return 8;
    case  VCompType_Float3: return 12;
    case  VCompType_Float4: return 16;
    case  VCompType_Color:  return 4;   
    case  VCompType_Short2: return 4;
    case  VCompType_Short4: return 8;
    case  VCompType_UByte4: return 4;
    }
    return 0;
} // GetVertexElementSize

inline VCompType GetDefaultElementType( VertexComponent usage )
{
    switch (usage)
    {
        case VertexComponent_Position:      return  VCompType_Float3;
        case VertexComponent_PositionRHW:   return  VCompType_Float4;
        case VertexComponent_Blend:         return  VCompType_UByte4;
        case VertexComponent_Blend0:        return  VCompType_Float1;
        case VertexComponent_Blend1:        return  VCompType_Float1;
        case VertexComponent_Blend2:        return  VCompType_Float1;
        case VertexComponent_Blend3:        return  VCompType_Float1;
        case VertexComponent_BlendIdx:      return  VCompType_UByte4;
        case VertexComponent_BlendIdx0:     return  VCompType_UByte4;    
        case VertexComponent_BlendIdx1:     return  VCompType_UByte4;    
        case VertexComponent_BlendIdx2:     return  VCompType_UByte4;    
        case VertexComponent_BlendIdx3:     return  VCompType_UByte4;    
        case VertexComponent_Normal:        return  VCompType_Float3;
        case VertexComponent_Diffuse:       return  VCompType_Color;
        case VertexComponent_Specular:      return  VCompType_Color;    
        case VertexComponent_TexCoor0:      return  VCompType_Float2;   
        case VertexComponent_TexCoor1:      return  VCompType_Float2;   
        case VertexComponent_TexCoor2:      return  VCompType_Float2;   
        case VertexComponent_TexCoor3:      return  VCompType_Float2;   
        case VertexComponent_TexCoor4:      return  VCompType_Float2;   
        case VertexComponent_TexCoor5:      return  VCompType_Float2;   
        case VertexComponent_TexCoor6:      return  VCompType_Float2;   
        case VertexComponent_TexCoor7:      return  VCompType_Float2;   
        case VertexComponent_Binormal:      return  VCompType_Float3;   
        case VertexComponent_Tangent:       return  VCompType_Float3;   
        case VertexComponent_Custom:        return  VCompType_Float3;  
    }
    return  VCompType_Unknown;
} // GetDefaultElementType

//****************************************************************************/
/*  Struct: VertexElement
//  Desc:   Describes single element of the vertex stream layout
//****************************************************************************/
struct VertexElement
{
    int                 m_Stream;   
    int                 m_Offset;
    VertexComponent     m_Usage;
    VCompType           m_Type;
    
    VertexElement() : 
        m_Stream    (0), 
        m_Offset    (0), 
        m_Usage     ( VertexComponent_Unknown ), 
        m_Type      ( VCompType_Unknown )
    {
    }

    VertexElement( VertexComponent usage,  VCompType type, int stream = 0 )
    {
        m_Usage     = usage;
        m_Offset    = 0;
        m_Type      = type;
        m_Stream    = stream;
    }
}; // struct VertexElement

const int c_MaxVertDeclElements = 16;
//****************************************************************************/
/*  Struct: VertexDeclaration
//  Desc:   Declaration of the vertex stream data mapping
//****************************************************************************/
struct VertexDeclaration
{
    VertexElement       m_Element[c_MaxVertDeclElements];   //  array of the vertex elements
    int                 m_NElements;                        //  number of vertex elements
    int                 m_VertexSize;                       //  size of the vertex, in bytes
    int                 m_TypeID;                           //  id in the render system vtype registry

    VertexDeclaration() : m_NElements(0), m_VertexSize(0), m_TypeID(-1) {}
    VertexDeclaration& operator <<( const VertexElement& el ) 
    {
        assert( m_NElements < c_MaxVertDeclElements );
        m_Element[m_NElements].m_Usage  = el.m_Usage;
        m_Element[m_NElements].m_Type   = el.m_Type;
        m_Element[m_NElements].m_Stream = el.m_Stream;
        m_Element[m_NElements].m_Offset = m_VertexSize;
        m_VertexSize += GetVertexElementSize( el.m_Type );
        m_NElements++;
        return *this;
    } // operator <<

    VertexDeclaration& operator <<( VertexComponent usage ) 
    {
        return operator <<( VertexElement( usage, GetDefaultElementType( usage ), 0 ) );
    } // operator <<

    bool operator ==( const VertexDeclaration& v ) const
    {
        if (m_NElements != v.m_NElements || m_VertexSize != v.m_VertexSize) return false;
        for (int i = 0; i < m_NElements; i++) 
        {
            if (m_Element[i].m_Type   != v.m_Element[i].m_Type  || 
                m_Element[i].m_Usage  != v.m_Element[i].m_Usage ||
                m_Element[i].m_Stream != v.m_Element[i].m_Stream) return false;
        }
        return true;
    } // operator ==

    int GetElementIdx( VertexComponent vcomp ) const
    {
        for (int i = 0; i < m_NElements; i++)
        {
            if (m_Element[i].m_Usage == vcomp)
            {
                return i;
            }
        }
        return -1;
    }
    
    bool GetElementParam( VertexComponent vcomp, int& offset, int& size ) const
    {
        int idx = GetElementIdx( vcomp );
        if (idx == -1) return false;
        offset = m_Element[idx].m_Offset;
        size   = GetVertexElementSize( m_Element[idx].m_Type );
        return true;
    } // GetElementParam
    
    void Clear() { m_NElements = 0; m_VertexSize = 0; m_TypeID = -1; }
}; // struct VertexDeclaration

inline VertexDeclaration GetStockVDecl( VertexType vtype )
{
    VertexDeclaration vd;
    switch (vtype)
    {
    case VertexType_XYZDUV2  : vd << VertexComponent_Position << VertexComponent_Diffuse << VertexComponent_TexCoor0 << VertexComponent_TexCoor1; break;      
    case VertexType_XYZWDSUV : vd << VertexComponent_PositionRHW << VertexComponent_Diffuse << VertexComponent_Specular << VertexComponent_TexCoor0; break;   
    case VertexType_XYZDUV   : vd << VertexComponent_Position << VertexComponent_Diffuse << VertexComponent_TexCoor0; break;     
    case VertexType_XYZNUV   : vd << VertexComponent_Position << VertexComponent_Normal << VertexComponent_TexCoor0;  break;
    case VertexType_XYZUV    : vd << VertexComponent_Position << VertexComponent_TexCoor0; break;
    case VertexType_XYZD     : vd << VertexComponent_Position << VertexComponent_Diffuse;  break;
    case VertexType_XYZDNUV  : vd << VertexComponent_Position << VertexComponent_Diffuse << VertexComponent_Normal << VertexComponent_TexCoor0;  break;
    case VertexType_XYZNUVW4 : vd << VertexComponent_Position << VertexComponent_Normal << VertexComponent_TexCoor0 << 
                                   VertexComponent_Blend << VertexComponent_BlendIdx;  break;
    
    }

    vd.m_TypeID = (int)vtype;
    return vd;
} // GetStockVDecl

#endif // __VERTEXDECLARATION_H__


