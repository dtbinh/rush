//****************************************************************************/
//  File:   PSOperator.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSOPERATOR_H__
#define __PSOPERATOR_H__

class Particle;
class ParticleServer;
class EmitterInstance;
//****************************************************************************/
//  Class:  PSOperator
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSOperator : public JObject
{
public: 
    PSOperator();

    virtual void Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps ) {}

    expose( PSOperator )
    {
        parent(JObject);
    }
}; // class PSOperator

#endif // __PSOPERATOR_H__


