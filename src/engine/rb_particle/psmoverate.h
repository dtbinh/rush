//****************************************************************************/
//  File:   PSMoveRate.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSMOVERATE_H__
#define __PSMOVERATE_H__

//****************************************************************************/
//  Class:  PSMoveRate
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSMoveRate : public PSOperator
{
    float       m_MinRate;   
    float       m_MaxRate; 
    float       m_MinSpeed;
    float       m_MaxSpeed;

public: 
                PSMoveRate();
    void        Update    ( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );

    expose( PSMoveRate )
    {
        parent(PSOperator);
        field( "MinRate",  m_MinRate  );
        field( "MaxRate",  m_MaxRate  );
        field( "MinSpeed", m_MinSpeed );
        field( "MaxSpeed", m_MaxSpeed );
    }
}; // class PSMoveRate

#endif // __PSMOVERATE_H__


