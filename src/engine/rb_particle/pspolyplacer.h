//****************************************************************************/
//  File:   PSPolyPlacer.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSPOLYPLACER_H__
#define __PSPOLYPLACER_H__

//****************************************************************************/
//  Class:  PSPolyPlacer
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSPolyPlacer : public PSOperator
{
public:  
    PSPolyPlacer();

    expose( PSPolyPlacer )
    {
        parent(PSOperator);
        field( "Contour", m_Contour );
        field( "Surface",       m_bSurface      );
    }

    void Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );

protected:
    PolyLine2                   m_Contour;
    bool                        m_bSurface;
}; // class PSPolyPlacer

#endif // __PSPolyPlacer_H__


