//****************************************************************************/
//  File:   PSPath2.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSPath2_H__
#define __PSPath2_H__

//****************************************************************************/
//  Class:  PSPath2
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSPath2 : public PSOperator
{
    PolyLine2           m_Path;
    bool                m_bClosed;
    float               m_Magnitude;

public: 
    PSPath2();
    void    Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps );

    expose( PSPath2 )
    {
        parent(PSOperator);
        field( "Path",      m_Path      );
        field( "Closed",    m_bClosed   );
        field( "Magnitude", m_Magnitude );
    }
}; // class PSPath2

#endif // __PSPath2_H__


