//****************************************************************************/
//  File:   PSVelRamp.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSVelRamp_H__
#define __PSVelRamp_H__

//****************************************************************************/
//  Class:  PSVelRamp
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSVelRamp : public PSOperator
{
    Ramp            m_Ramp;
    int             m_NRepeats;
public: 
            PSVelRamp();
    void    Update   ( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );

    expose( PSVelRamp )
    {
        parent(PSOperator);
        field( "Ramp",      m_Ramp      );
        field( "Repeats",   m_NRepeats  );
    }
}; // class PSVelRamp

#endif // __PSVelRamp_H__


