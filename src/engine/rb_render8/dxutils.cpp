//****************************************************************************/
//  File:  DXUtils.cpp
//  Desc:  
//****************************************************************************/
#include "stdafx.h"
#include "DXUtils.h"

D3DCOLORVALUE ConvertColor( DWORD col )
{
    D3DCOLORVALUE res;
    res.a = float( (col & 0xFF000000)>>24 ) / 255.0f;
    res.r = float( (col & 0x00FF0000)>>16 ) / 255.0f;
    res.g = float( (col & 0x0000FF00)>>8  ) / 255.0f;
    res.b = float( (col & 0x000000FF)     ) / 255.0f;
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

DWORD CreateFVF( const VertexDeclaration& vdecl )
{
    DWORD fvf = 0;
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
} // CreateVDecl
