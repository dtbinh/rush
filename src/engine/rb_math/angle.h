
//  File:  Angle.h
//  Desc:  Incapsulates rotation angle 
//****************************************************************************/
#ifndef __ANGLE_H__
#define __ANGLE_H__
#pragma once

//****************************************************************************/
//  Class:  Angle
//  Desc:   Incapsulates rotation angle 
//****************************************************************************/
class Angle
{
    float       m_Angle;
public:
                Angle   () : m_Angle( 0.0f ) {}
                Angle   ( float ang ) : m_Angle( ang ) { Wrap(); }
    operator    float   () const { return m_Angle; }
    bool        inRange ( float angMin, float angMax ) const 
    { 
        assert( false );
        return false; 
    }

private:
    void        Wrap()
    {
        if (m_Angle >= 360.0f)
        {
            m_Angle = fmodf( m_Angle, 360.0f );
        }
        else if (m_Angle < 0.0f)
        {
            m_Angle = 360.0f + fmodf( m_Angle, 360.0f );
        }
    }
}; // class Angle

#endif // __ANGLE_H__

