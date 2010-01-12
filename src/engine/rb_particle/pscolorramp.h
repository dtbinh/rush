//****************************************************************************/
//  File:   PSColorRamp.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSCOLORRAMP_H__
#define __PSCOLORRAMP_H__

//****************************************************************************/
//  Class:  PSColorRamp
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSColorRamp : public PSOperator
{
    ColorRamp       m_Ramp;
    bool            m_bRelative;
    int             m_NRepeats;
    PSRampUsage     m_Usage;

public: 
            PSColorRamp();
    void    Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );

    expose( PSColorRamp )
    {
        parent(PSOperator);
        field( "Ramp",      m_Ramp      );
        field( "Relative",  m_bRelative );
        field( "Repeats",   m_NRepeats  );
        field( "Usage",     m_Usage     );
    }
}; // class PSColorRamp

#endif // __PSColorRamp_H__


