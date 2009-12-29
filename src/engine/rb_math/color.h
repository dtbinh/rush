//****************************************************************************/
//  File:  Color.h
//  Desc:
//****************************************************************************/
#ifndef __COLOR_H__
#define __COLOR_H__


#include "scalar.h"

const float c_ClrEpsilon = 1.0f/512.f;
//****************************************************************************/
//  Class:  ColorF
//  Desc:
//****************************************************************************/
class ColorF
{
public:
    float a, r, g, b;

    ColorF() {}
    ColorF( float ca, float cr, float cg, float cb ) : r(cr), g(cg), b(cb), a(ca) {}

    ColorF( uint32_t c )
    {
         a = float( (c&0xFF000000) >> 24 )/255.0f;
         r = float( (c&0x00FF0000) >> 16 )/255.0f;
         g = float( (c&0x0000FF00) >> 8  )/255.0f;
         b = float( (c&0x000000FF)       )/255.0f;
    }

    ColorF( uint8_t ba, uint8_t br, uint8_t bg, uint8_t bb )
    {
         a = float( ba )/255.0f;
         r = float( br )/255.0f;
         g = float( bg )/255.0f;
         b = float( bb )/255.0f;
    }

    operator uint32_t() const
    {
        float fa = a;
        float fr = r;
        float fg = g;
        float fb = b;

        fa = clamp( fa, 0.0f, 1.0f );
	    fr = clamp( fr, 0.0f, 1.0f );
	    fg = clamp( fg, 0.0f, 1.0f );
	    fb = clamp( fb, 0.0f, 1.0f );

	    uint8_t ba, br, bg, bb;
	    ba = (uint8_t)(fa*255.0f);
	    br = (uint8_t)(fr*255.0f);
	    bg = (uint8_t)(fg*255.0f);
	    bb = (uint8_t)(fb*255.0f);
        return  (
				((ba & 0xFF) << 24) |
				((br & 0xFF) << 16) |
				((bg & 0xFF) << 8)  |
				(bb & 0xFF)
				);
    } // operator uint32_t

    friend uint32_t lerp( const ColorF& c1, const ColorF& c2, float t )
    {
        ColorF c;
        c.lerp( c1, c2, t );
        return c;
    } // lerp

    void lerp( const ColorF& c1, const ColorF& c2, float t )
    {
        a = (c2.a - c1.a)*t + c1.a;
	    b = (c2.b - c1.b)*t + c1.b;
	    g = (c2.g - c1.g)*t + c1.g;
	    r = (c2.r - c1.r)*t + c1.r;
    } // lerp

    void toGreyScale()
    {
        float grey = 0.2125f*r + 0.7154f*g + 0.0721f*b;
        b = grey;
        g = grey;
        r = grey;
    } // toGreyScale

    void adjustSaturation( float s )
    {
        float grey = 0.2125f*r + 0.7154f*g + 0.0721f*b;
        r = grey + s*(r - grey);
        g = grey + s*(g - grey);
        b = grey + s*(b - grey);
    } // adjustSaturation

    void adjustContrast( float c )
    {
        r = 0.5f + c*(r - 0.5f);
        g = 0.5f + c*(g - 0.5f);
        b = 0.5f + c*(b - 0.5f);
    } // adjustContrast

    void operator *= ( const ColorF& clr )
    {
        r *= clr.r;
        g *= clr.g;
        b *= clr.b;
        a *= clr.a;
    }

    void operator *= ( float val )
    {
        r *= val;
        g *= val;
        b *= val;
        a *= val;
    }

    void operator += ( const ColorF& clr )
    {
        r += clr.r;
        g += clr.g;
        b += clr.b;
        a += clr.a;
    }

    friend ColorF operator +( const ColorF& a, const ColorF& b )
    {
        return ColorF( a.a + b.a, a.r + b.r, a.g + b.g, a.b + b.b );
    }

    friend ColorF operator *( const ColorF& a, const ColorF& b )
    {
        return ColorF( a.a*b.a, a.r*b.r, a.g *b.g, a.b*b.b );
    }

    friend ColorF operator *( const ColorF& clr, float val )
    {
        return ColorF( clr.a*val, clr.r*val, clr.g*val, clr.b*val );
    }

    friend ColorF operator *( float val, const ColorF& clr )
    {
        return ColorF( clr.a*val, clr.r*val, clr.g*val, clr.b*val );
    }

    bool operator ==( const ColorF& clr ) const
    {
        return isEqual( clr );
    }

    bool isEqual( const ColorF& clr, float epsilon = c_ClrEpsilon ) const
    {
        return (_fabs( a - clr.a ) < epsilon &&
                _fabs( r - clr.r ) < epsilon &&
                _fabs( g - clr.g ) < epsilon &&
                _fabs( b - clr.b ) < epsilon);
    }

    bool operator !=( const ColorF& clr ) const
    {
        return !operator ==( clr );
    }

    static ColorF       black;
    static ColorF       white;

    static ColorF       red;
    static ColorF       green;
    static ColorF       blue;

    static ColorF       yellow;
    static ColorF       cyan;
    static ColorF       magenta;
}; // ColorF

//****************************************************************************/
//	Enum:   ColorFormat
//  Desc:   Decribes pixel color configuration
//****************************************************************************/
enum ColorFormat
{
    ColorFormat_ARGB4444    = 0,
    ColorFormat_ARGB8888    = 1,
    ColorFormat_RGB565      = 2,
    ColorFormat_RGB888      = 3,
    ColorFormat_A8          = 4,
    ColorFormat_XRGB1555    = 5,
    ColorFormat_XRGB8888    = 6,
    ColorFormat_V8U8        = 7,
    ColorFormat_R16F        = 8,
    ColorFormat_GR16F       = 9,
    ColorFormat_ABGR16F     = 10,
    ColorFormat_R32F        = 11,
    ColorFormat_GR32F       = 12,
    ColorFormat_ABGR32F     = 13,
    ColorFormat_ARGB32F     = 14,
    ColorFormat_Last        = 15,

    ColorFormat_Unknown     = 255,

}; // enum ColorFormat

inline int GetBytesPerPixel( ColorFormat cf )
{
    switch (cf)
    {
    case ColorFormat_ARGB4444:    return 2;
    case ColorFormat_ARGB8888:    return 4;
    case ColorFormat_RGB565:      return 2;
    case ColorFormat_RGB888:      return 3;
    case ColorFormat_A8:          return 1;
    case ColorFormat_XRGB1555:    return 2;
    case ColorFormat_XRGB8888:    return 4;
    case ColorFormat_V8U8:        return 2;
    case ColorFormat_R16F:        return 2;
    case ColorFormat_GR16F:       return 4;
    case ColorFormat_ABGR16F:     return 8;
    case ColorFormat_R32F:        return 4;
    case ColorFormat_GR32F:       return 8;
    case ColorFormat_ARGB32F:     return 32;
    }
    return 0;
} // GetBytesPerPixel

#endif // __COLOR_H__

