/***********************************************************************************/
//  File:   ColorRamp.h
//  Date:   12.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLORRAMP_H__
#define __COLORRAMP_H__

#include "Ramp.h"
#include "Color.h"

/***********************************************************************************/
//  Class:  ColorRamp
//  Desc:   
/***********************************************************************************/
class ColorRamp
{
    Ramp        m_R, m_G, m_B;
public:
    
    bool operator ==( const ColorRamp& ramp ) const
    {
        return (m_R == ramp.m_R && m_G == ramp.m_G && m_B == ramp.m_B);
    }

    int         GetNKeys() const { return m_R.GetNKeys(); }
    void        Clear   ()
    {
        m_R.Clear();
        m_G.Clear();
        m_B.Clear();
    }
    void        AddKey  ( float keyTime, DWORD keyVal ) 
    { 
        ColorF clr( keyVal );
        m_R.AddKey( keyTime, clr.r );
        m_G.AddKey( keyTime, clr.g );
        m_B.AddKey( keyTime, clr.b );
    }

    void        DeleteKey ( int idx )
    {
        m_R.DeleteKey( idx );
        m_G.DeleteKey( idx );
        m_B.DeleteKey( idx );
    }

    float       GetTime ( int keyIdx ) const { return m_R.GetTime( keyIdx ); }
    DWORD       GetValue( int keyIdx ) const 
    { 
        ColorF clr( 1.0f, m_R.GetValue( keyIdx ), m_G.GetValue( keyIdx ), m_B.GetValue( keyIdx ) );
        return clr; 
    }
    DWORD       GetValue( float cTime ) const
    {
        ColorF clr( 1.0f, m_R.GetValue( cTime ), m_G.GetValue( cTime ), m_B.GetValue( cTime ) );
        return clr; 
    }
    void        SetKey  ( int idx, float keyTime, DWORD keyVal )
    {
        ColorF clr( keyVal );
        m_R.SetKey( idx, keyTime, clr.r );
        m_G.SetKey( idx, keyTime, clr.g );
        m_B.SetKey( idx, keyTime, clr.b );
    }

}; // class ColorRamp

#endif //__COLORRAMP_H__