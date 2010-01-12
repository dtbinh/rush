//****************************************************************************/
//  File:   PSSizeRamp.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSIZERAMP_H__
#define __PSSIZERAMP_H__

//****************************************************************************/
//  Class:  PSSizeRamp
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSSizeRamp : public PSOperator
{
    Ramp            m_Ramp;
    bool            m_bRelative;
    int             m_NRepeats;
    PSRampUsage     m_Usage;
    PSAxis          m_Axis;

public: 
            PSSizeRamp();
    void    Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );

    expose( PSSizeRamp )
    {
        parent(PSOperator);
        field( "Ramp",      m_Ramp      );
        field( "Relative",  m_bRelative );
        field( "Repeats",   m_NRepeats  );
        field( "Usage",     m_Usage     );
        field( "Axis",      m_Axis      );
    }
}; // class PSSizeRamp

#endif // __PSSizeRamp_H__


