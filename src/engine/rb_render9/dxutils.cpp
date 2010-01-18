//****************************************************************************/
//  File:  DXUtils.cpp
//  Desc:  
//****************************************************************************/
#include "precompile.h"
#include "DXUtils.h"

D3DCOLORVALUE ConvertColor( uint32_t col )
{
    D3DCOLORVALUE res;
    res.a = float( (col & 0xFF000000)>>24 ) / 255.0f;
    res.r = float( (col & 0x00FF0000)>>16 ) / 255.0f;
    res.g = float( (col & 0x0000FF00)>>8  ) / 255.0f;
    res.b = float( (col & 0x000000FF)     ) / 255.0f;
    return res;
} // ConvertColor

uint32_t ConvertColor( D3DCOLORVALUE col )
{
    uint32_t res;
    uint32_t a = uint32_t( col.a*255.0f )<<24;
    uint32_t r = uint32_t( col.r*255.0f )<<16;
    uint32_t g = uint32_t( col.g*255.0f )<<8;
    uint32_t b = uint32_t( col.b*255.0f );
    res = a | r | g | b;
    return res;
} // ConvertColor

const char* GetDXError( HRESULT hresult )
{
    switch (hresult)
    {
    case D3DERR_CONFLICTINGRENDERSTATE: 
        return "The currently set render states cannot be used together."; 
    case D3DERR_CONFLICTINGTEXTUREFILTER: 
        return "The current texture filters cannot be used together.";
    case D3DERR_CONFLICTINGTEXTUREPALETTE: 
        return "The current textures cannot be used simultaneously.";
    case D3DERR_DEVICELOST: 
        return "The device is lost and cannot be restored at the current time."
            "Rendering is not possible."; 
    case D3DERR_DEVICENOTRESET: 
        return "The device cannot be reset."; 
    case D3DERR_DRIVERINTERNALERROR: 
        return "Internal driver error."; 
    case D3DERR_INVALIDCALL: 
        return "The method call is invalid. " 
            "Method's parameter may have an invalid value."; 
    case D3DERR_INVALIDDEVICE: 
        return "The requested device type is not valid."; 
    case D3DERR_MOREDATA: 
        return "There is more data available than the specified buffer size can hold."; 
    case D3DERR_NOTAVAILABLE: 
        return "This device does not support the queried technique."; 
    case D3DERR_NOTFOUND: 
        return "The requested item was not found.";
    case D3DERR_OUTOFVIDEOMEMORY: 
        return "Direct3D does not have enough display memory to perform the operation."; 
    case D3DERR_TOOMANYOPERATIONS: 
        return "The application is requesting more texture-filtering operations "
            "than the device supports."; 
    case D3DERR_UNSUPPORTEDALPHAARG: 
        return "The device does not support a specified texture-blending argument "
            "for the alpha channel.";
    case D3DERR_UNSUPPORTEDALPHAOPERATION: 
        return "The device does not support a specified texture-blending operation " 
            "for the alpha channel."; 
    case D3DERR_UNSUPPORTEDCOLORARG: 
        return "The device does not support a specified texture-blending argument "
            "for color values."; 
    case D3DERR_UNSUPPORTEDCOLOROPERATION: 
        return "The device does not support a specified texture-blending operation "
            "for color values."; 
    case D3DERR_UNSUPPORTEDFACTORVALUE: 
        return "The device does not support the specified texture factor value."; 
    case D3DERR_UNSUPPORTEDTEXTUREFILTER: 
        return "The device does not support the specified texture filter."; 
    case D3DERR_WRONGTEXTUREFORMAT: 
        return "The pixel format of the texture surface is not valid."; 
    case E_FAIL: 
        return "An undetermined error occurred inside the Direct3D subsystem."; 
    case E_INVALIDARG: 
        return "An invalid parameter was passed to the returning function."; 
    case E_OUTOFMEMORY: 
        return "Direct3D could not allocate sufficient memory to complete the call.";
    default: return "";
    }
    return "";
} // GetDXError

uint32_t CreateFVF( const VertexDeclaration& vdecl )
{
    uint32_t fvf = 0;
    for (int i = 0; i < vdecl.m_NElements; i++)
    {
        switch (vdecl.m_Element[i].m_Usage)
        {
        case VertexComponent_Position		: fvf |= D3DFVF_XYZ;    break; 
        case VertexComponent_PositionRHW	: fvf |= D3DFVF_XYZRHW; break; 

        case VertexComponent_Blend0		    : fvf |= D3DFVF_XYZB1; break; 
        case VertexComponent_Blend1		    : fvf |= D3DFVF_XYZB2; fvf &= ~D3DFVF_XYZB1; break; 
        case VertexComponent_Blend2		    : fvf |= D3DFVF_XYZB3; fvf &= ~D3DFVF_XYZB2; break; 
        case VertexComponent_Blend3		    : fvf |= D3DFVF_XYZB4; fvf &= ~D3DFVF_XYZB3; break; 
        case VertexComponent_BlendIdx	    : fvf |= D3DFVF_LASTBETA_UBYTE4; break; 

        case VertexComponent_Normal		    : fvf |= D3DFVF_NORMAL; break;
        case VertexComponent_Diffuse		: fvf |= D3DFVF_DIFFUSE; break;
        case VertexComponent_Specular		: fvf |= D3DFVF_SPECULAR; break;

        case VertexComponent_TexCoor0		: fvf |= D3DFVF_TEX1; break; 
        case VertexComponent_TexCoor1		: fvf |= D3DFVF_TEX2; fvf &= ~D3DFVF_TEX1; break; 
        case VertexComponent_TexCoor2		: fvf |= D3DFVF_TEX3; fvf &= ~D3DFVF_TEX2; break; 
        case VertexComponent_TexCoor3		: fvf |= D3DFVF_TEX4; fvf &= ~D3DFVF_TEX3; break; 
        case VertexComponent_TexCoor4		: fvf |= D3DFVF_TEX5; fvf &= ~D3DFVF_TEX4; break; 
        case VertexComponent_TexCoor5		: fvf |= D3DFVF_TEX6; fvf &= ~D3DFVF_TEX5; break; 
        case VertexComponent_TexCoor6		: fvf |= D3DFVF_TEX7; fvf &= ~D3DFVF_TEX6; break; 
        case VertexComponent_TexCoor7		: fvf |= D3DFVF_TEX8; fvf &= ~D3DFVF_TEX7; break; 
        }
    }
    return fvf;
} // CreateFVF

VertexDeclaration CreateVDecl( uint32_t fvf )
{
    VertexDeclaration res;
    if ((fvf & D3DFVF_XYZ) == D3DFVF_XYZ)                res << VertexComponent_Position;
    if ((fvf & D3DFVF_XYZRHW) == D3DFVF_XYZRHW)          res << VertexComponent_PositionRHW;
    if ((fvf & D3DFVF_XYZB1) == D3DFVF_XYZB1)            res << VertexComponent_Blend0;
    if ((fvf & D3DFVF_XYZB2) == D3DFVF_XYZB2)            res << VertexComponent_Blend1;
    if ((fvf & D3DFVF_XYZB3) == D3DFVF_XYZB3)            res << VertexComponent_Blend2;
    if ((fvf & D3DFVF_XYZB4) == D3DFVF_XYZB4)            res << VertexComponent_Blend3;
    if ((fvf & D3DFVF_LASTBETA_UBYTE4) == D3DFVF_LASTBETA_UBYTE4)   res << VertexComponent_BlendIdx;
    if ((fvf & D3DFVF_NORMAL) == D3DFVF_NORMAL)          res << VertexComponent_Normal;
    if ((fvf & D3DFVF_DIFFUSE) == D3DFVF_DIFFUSE)        res << VertexComponent_Diffuse;
    if ((fvf & D3DFVF_SPECULAR) == D3DFVF_SPECULAR)      res << VertexComponent_Specular;
    if ((fvf & D3DFVF_TEX1) == D3DFVF_TEX1)              res << VertexComponent_TexCoor0;
    if ((fvf & D3DFVF_TEX2) == D3DFVF_TEX2)              res << VertexComponent_TexCoor1;
    if ((fvf & D3DFVF_TEX3) == D3DFVF_TEX3)              res << VertexComponent_TexCoor2;
    if ((fvf & D3DFVF_TEX4) == D3DFVF_TEX4)              res << VertexComponent_TexCoor3;
    if ((fvf & D3DFVF_TEX5) == D3DFVF_TEX5)              res << VertexComponent_TexCoor5;
    if ((fvf & D3DFVF_TEX6) == D3DFVF_TEX6)              res << VertexComponent_TexCoor6;
    if ((fvf & D3DFVF_TEX7) == D3DFVF_TEX7)              res << VertexComponent_TexCoor7;

    return res;
} // CreateVDecl

inline D3DDECLTYPE ConvertVCompType( VCompType type )
{
    D3DDECLTYPE res = D3DDECLTYPE_UNUSED;
    switch (type)
    {
        case VCompType_Float1: res = D3DDECLTYPE_FLOAT1;    break;
        case VCompType_Float2: res = D3DDECLTYPE_FLOAT2;    break;
        case VCompType_Float3: res = D3DDECLTYPE_FLOAT3;    break;
        case VCompType_Float4: res = D3DDECLTYPE_FLOAT4;    break;
        case VCompType_Color:  res = D3DDECLTYPE_D3DCOLOR;  break;
        case VCompType_Short2: res = D3DDECLTYPE_SHORT2;    break;
        case VCompType_Short4: res = D3DDECLTYPE_SHORT4;    break;
        case VCompType_UByte4: res = D3DDECLTYPE_UBYTE4;    break;
    };
    return res;
} // ConvertVCompType

inline D3DDECLUSAGE ConvertVertexComponent( VertexComponent usage )
{
    D3DDECLUSAGE res = D3DDECLUSAGE_TEXCOORD;
    switch (usage)
    {
        case VertexComponent_Position: 
        case VertexComponent_PositionRHW: 
            res = D3DDECLUSAGE_POSITION;  break;
        case VertexComponent_Normal:      res = D3DDECLUSAGE_NORMAL;    break;
        case VertexComponent_Diffuse:     res = D3DDECLUSAGE_COLOR;     break;
        case VertexComponent_Specular:    res = D3DDECLUSAGE_COLOR;     break;
        case VertexComponent_Binormal:    res = D3DDECLUSAGE_BINORMAL;  break;
        case VertexComponent_Tangent:     res = D3DDECLUSAGE_TANGENT;   break;
        case VertexComponent_Blend0:
        case VertexComponent_Blend1:
        case VertexComponent_Blend2:
        case VertexComponent_Blend3:
            res = D3DDECLUSAGE_BLENDWEIGHT; break;
        case VertexComponent_BlendIdx:
        case VertexComponent_BlendIdx0:
        case VertexComponent_BlendIdx1:
        case VertexComponent_BlendIdx2:
        case VertexComponent_BlendIdx3:
            res = D3DDECLUSAGE_BLENDINDICES; break;
        case VertexComponent_TexCoor0:
        case VertexComponent_TexCoor1:
        case VertexComponent_TexCoor2:
        case VertexComponent_TexCoor3:
        case VertexComponent_TexCoor4:
        case VertexComponent_TexCoor5:
        case VertexComponent_TexCoor6:
        case VertexComponent_TexCoor7:
            res = D3DDECLUSAGE_TEXCOORD; break;
    };
    return res;
} // ConvertVertexComponent

inline int GetVCompIndex( VertexComponent usage )
{
    int res = 0;
    switch (usage)
    {
        case VertexComponent_Specular:    res = 1; break;
        case VertexComponent_TexCoor0:    res = 0; break;
        case VertexComponent_TexCoor1:    res = 1; break;
        case VertexComponent_TexCoor2:    res = 2; break;
        case VertexComponent_TexCoor3:    res = 3; break;
        case VertexComponent_TexCoor4:    res = 4; break;
        case VertexComponent_TexCoor5:    res = 5; break;
        case VertexComponent_TexCoor6:    res = 6; break;
        case VertexComponent_TexCoor7:    res = 7; break;
        case VertexComponent_Blend:       res = 2; break;
        case VertexComponent_Blend0:      res = 2; break;
        case VertexComponent_Blend1:      res = 3; break;
        case VertexComponent_Blend2:      res = 4; break;
        case VertexComponent_Blend3:      res = 5; break;
        case VertexComponent_BlendIdx:    res = 2; break;
        case VertexComponent_BlendIdx0:   res = 2; break;
        case VertexComponent_BlendIdx1:   res = 3; break;
        case VertexComponent_BlendIdx2:   res = 4; break;
        case VertexComponent_BlendIdx3:   res = 5; break;
    };
    return res;
} // GetVCompIndex

IDirect3DVertexDeclaration9* CreateVDecl( IDirect3DDevice9* pDevice, const VertexDeclaration& vdecl )
{
    if (!pDevice) return NULL;
    IDirect3DVertexDeclaration9* pDecl = NULL;
    D3DVERTEXELEMENT9 elem[MAXD3DDECLLENGTH + 1];
    int cElem = 0;
    for (int i = 0; i < vdecl.m_NElements; i++)
    {
        elem[cElem].Stream     = vdecl.m_Element[i].m_Stream;
        elem[cElem].Offset     = vdecl.m_Element[i].m_Offset;
        elem[cElem].Type       = ConvertVCompType( vdecl.m_Element[i].m_Type );
        elem[cElem].Method     = D3DDECLMETHOD_DEFAULT; 
        elem[cElem].Usage      = ConvertVertexComponent( vdecl.m_Element[i].m_Usage );
        elem[cElem].UsageIndex = GetVCompIndex( vdecl.m_Element[i].m_Usage );
        cElem++;
    }
    //  end element
    elem[cElem].Stream     = 0xFF; 
    elem[cElem].Offset     = 0;
    elem[cElem].Type       = D3DDECLTYPE_UNUSED;
    elem[cElem].Method     = 0;
    elem[cElem].Usage      = 0;
    elem[cElem].UsageIndex = 0;
    pDevice->CreateVertexDeclaration( elem, &pDecl );
    return pDecl;
} // CreateVDecl