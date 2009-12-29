//****************************************************************************/
//  File:   PSLinePlacer.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSLinePlacer_H__
#define __PSLinePlacer_H__

//****************************************************************************/
//  Class:  PSLinePlacer
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSLinePlacer : public PSOperator
{
    Vec3        m_Start;
    Vec3        m_End;

public: 
    PSLinePlacer();

    void Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps );

    expose( PSLinePlacer )
    {
        parent(PSOperator);
        field( "Start", m_Start );
        field( "End",   m_End );
    }
}; // class PSLinePlacer

#endif // __PSLinePlacer_H__


