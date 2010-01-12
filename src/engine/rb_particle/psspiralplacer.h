//****************************************************************************/
//  File:   PSSpiralPlacer.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSPIRALPLACER_H__
#define __PSSPIRALPLACER_H__

//****************************************************************************/
//  Class:  PSSpiralPlacer
//  Desc:    
//****************************************************************************/
class PSSpiralPlacer : public PSOperator
{
    float               m_Radius;      //  radius of the spiral
    float               m_Period;      //  period of the spiral
    bool                m_bRelToDir;   //  whether to bind to the moving direction

public: 
    PSSpiralPlacer();
    virtual void  Update ( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );

    expose( PSSpiralPlacer )
    {
        parent(PSOperator);
        field( "Radius",       m_Radius      );
        field( "Period",       m_Period      );
        field( "RelToDir",     m_bRelToDir   );
    }
}; // class PSSpiralPlacer

#endif // __PSSPIRALPLACER_H__


