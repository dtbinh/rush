//****************************************************************************/
//  File:   PSAlphaRamp.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSALPHARAMP_H__
#define __PSALPHARAMP_H__

//****************************************************************************/
//  Class:  PSAlphaRamp
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSAlphaRamp : public PSOperator
{
    Ramp            m_Ramp;
    bool            m_bRelative;
    int             m_NRepeats;
    PSRampUsage     m_Usage;

public: 
            PSAlphaRamp();
    void    Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps );

    expose( PSAlphaRamp )
    {
        parent(PSOperator);
        field( "Ramp",      m_Ramp      );
        field( "Relative",  m_bRelative );
        field( "Repeats",   m_NRepeats  );
        field( "Usage",     m_Usage     );
    }
}; // class PSAlphaRamp

#endif // __PSALPHARAMP_H__


