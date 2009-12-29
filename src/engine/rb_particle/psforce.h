//****************************************************************************/
//  File:   PSForce.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSForce_H__
#define __PSForce_H__

//****************************************************************************/
//  Class:  PSForce
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSForce : public PSOperator
{
    Vec3        m_Force;
public: 
    PSForce();
    virtual void Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps );
    

    expose( PSForce )
    {
        parent(PSOperator);
        field( "Force", m_Force );
    }
}; // class PSForce

#endif // __PSForce_H__


