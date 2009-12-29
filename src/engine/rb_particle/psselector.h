//****************************************************************************/
//  File:   PSSelector.h
//  Date:   18.10.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSELECTOR_H__
#define __PSSELECTOR_H__

class ParticleServer;
//****************************************************************************/
//  Class:  PSSelector
//  Desc:   Performs selective firing of child emitters
//****************************************************************************/
class PSSelector : public PSEmitter
{
    JFloatList         m_Chances;
public: 
    PSSelector();

    expose( PSSelector )
    {
        parent(PSEmitter)
        field( "Chances", m_Chances );
    }
private:
    virtual PSEmitter* AssignEmitter( EmitterInstance* pInst, Particle* pHostParticle );
}; // class PSSelector

#endif // __PSSELECTOR_H__


