//****************************************************************************/
//  File:  DXUtils.h
//  Desc:  
//****************************************************************************/
#ifndef __DXUTILS_H__
#define __DXUTILS_H__

const char*     GetDXError      ( HRESULT hresult );
D3DCOLORVALUE   ConvertColor    ( DWORD col );

inline D3DFORMAT ConvertDepthFormat( DepthFormat dsFormat )
{
    switch (dsFormat)
    {
        case DepthFormat_D16Lockable:   return D3DFMT_D16_LOCKABLE;
        case DepthFormat_D32:           return D3DFMT_D32;
        case DepthFormat_D15S1:         return D3DFMT_D15S1;
        case DepthFormat_D24S8:         return D3DFMT_D24S8;
        case DepthFormat_D16:           return D3DFMT_D16;
    }
    return D3DFMT_D16;
} // ConvertDepthFormat

inline DepthFormat ConvertDepthFormat( D3DFORMAT dsFormat )
{
    switch (dsFormat) 
    {
        case D3DFMT_D16_LOCKABLE	: return DepthFormat_D16Lockable;
        case D3DFMT_D32			    : return DepthFormat_D32;
        case D3DFMT_D15S1		    : return DepthFormat_D15S1;
        case D3DFMT_D24S8		    : return DepthFormat_D24S8;
        case D3DFMT_D16			    : return DepthFormat_D16;
    }
    return DepthFormat_Unknown;
} // ConvertDepthFormat

inline D3DFORMAT ConvertColorFormat( ColorFormat format )
{
    D3DFORMAT d3dcf = D3DFMT_A4R4G4B4;
    switch (format)
    {
        case ColorFormat_ARGB4444:  d3dcf = D3DFMT_A4R4G4B4;	    break;
        case ColorFormat_XRGB1555:  d3dcf = D3DFMT_X1R5G5B5;	    break;
        case ColorFormat_ARGB8888:  d3dcf = D3DFMT_A8R8G8B8;	    break;
        case ColorFormat_XRGB8888:  d3dcf = D3DFMT_X8R8G8B8;	    break;
        case ColorFormat_RGB888:    d3dcf = D3DFMT_R8G8B8;		    break;
        case ColorFormat_A8:		d3dcf = D3DFMT_A8;		        break;
        case ColorFormat_RGB565:    d3dcf = D3DFMT_R5G6B5;	        break;
        case ColorFormat_V8U8:      d3dcf = D3DFMT_V8U8;	        break;
        // case ColorFormat_R16F:      d3dcf = D3DFMT_R16F;	        break;     
        // case ColorFormat_GR16F:     d3dcf = D3DFMT_G16R16F;	        break;     
        // case ColorFormat_ABGR16F:   d3dcf = D3DFMT_A16B16G16R16F;	break;     
        // case ColorFormat_R32F:      d3dcf = D3DFMT_R32F;		    break;     
        // case ColorFormat_GR32F:     d3dcf = D3DFMT_G32R32F;	        break;     
        // case ColorFormat_ABGR32F:   d3dcf = D3DFMT_A32B32G32R32F;	break;     
        case ColorFormat_Unknown:	d3dcf = D3DFMT_UNKNOWN;	        break;
    }
    return d3dcf;
} // ConvertColorFormat

inline ColorFormat ConvertColorFormat( D3DFORMAT format )
{
    ColorFormat cf = ColorFormat_Unknown;
    switch (format)
    {
        case D3DFMT_A4R4G4B4:	    cf = ColorFormat_ARGB4444; break;
        case D3DFMT_X1R5G5B5:	    cf = ColorFormat_XRGB1555; break;
        case D3DFMT_A8R8G8B8:	    cf = ColorFormat_ARGB8888; break;
        case D3DFMT_X8R8G8B8:	    cf = ColorFormat_XRGB8888; break;
        case D3DFMT_R8G8B8:		    cf = ColorFormat_RGB888;   break;
        case D3DFMT_A8:		        cf = ColorFormat_A8;	   break;
        case D3DFMT_R5G6B5:	        cf = ColorFormat_RGB565;   break;
        case D3DFMT_V8U8:	        cf = ColorFormat_V8U8;     break;
        // case D3DFMT_R16F:	        cf = ColorFormat_R16F;     break;     
        // case D3DFMT_G16R16F:	    cf = ColorFormat_GR16F;    break;     
        // case D3DFMT_A16B16G16R16F:  cf = ColorFormat_ABGR16F;  break;     
        // case D3DFMT_R32F:		    cf = ColorFormat_R32F;     break;     
        // case D3DFMT_G32R32F:	    cf = ColorFormat_GR32F;    break;     
        // case D3DFMT_A32B32G32R32F:  cf = ColorFormat_ABGR32F;  break;     
        case D3DFMT_UNKNOWN:	    cf = ColorFormat_Unknown;  break;
    }
    return cf;
} // ConvertColorFormat

inline D3DPOOL ConvertPoolType( PoolType poolType	)
{
    D3DPOOL pool = D3DPOOL_MANAGED;
    switch (poolType)
    {
        case PoolType_Video:      pool = D3DPOOL_DEFAULT;   break;
        case PoolType_System:     pool = D3DPOOL_SYSTEMMEM;	break;
        case PoolType_Managed:    pool = D3DPOOL_MANAGED;   break;
    };
    return pool;
} // ConvertPoolType

inline PoolType ConvertPoolType( D3DPOOL poolType )
{
    PoolType pool = PoolType_Unknown;
    switch (poolType)
    {
        case D3DPOOL_DEFAULT:    pool = PoolType_Video;     break;
        case D3DPOOL_SYSTEMMEM:  pool = PoolType_System;    break;
        case D3DPOOL_MANAGED:    pool = PoolType_Managed;   break;
    };
    return pool;
} // ConvertPoolType


inline D3DPRIMITIVETYPE ConvertPrimitiveType( PrimitiveType priType )
{
    D3DPRIMITIVETYPE d3dPri = D3DPT_POINTLIST;
    switch (priType)
    {
    case PrimitiveType_TriangleList:	d3dPri = D3DPT_TRIANGLELIST;	break;
    case PrimitiveType_TriangleStrip:	d3dPri = D3DPT_TRIANGLESTRIP;	break;
    case PrimitiveType_TriangleFan:     d3dPri = D3DPT_TRIANGLEFAN;		break;
    case PrimitiveType_LineStrip:		d3dPri = D3DPT_LINESTRIP;		break;
    case PrimitiveType_LineList:		d3dPri = D3DPT_LINELIST;		break;
    case PrimitiveType_PointList:		d3dPri = D3DPT_POINTLIST;		break;
    case PrimitiveType_QuadList:        d3dPri = D3DPT_TRIANGLELIST;	break;
    }
    return d3dPri;
}  // ConvertPrimitiveType

struct VertexDeclaration;
DWORD CreateFVF( const VertexDeclaration& vdecl );

#endif // __DXUTILS_H__
