//****************************************************************************/
//  File:   PSBoxPlacer.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSBoxPlacer_H__
#define __PSBoxPlacer_H__

//****************************************************************************/
//  Class:  PSBoxPlacer
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSBoxPlacer : public PSOperator
{
    bool                m_bPlanar;      
    bool                m_bSurface;    
    Vec3                m_Ext;

public: 
    PSBoxPlacer();
    virtual void  Update ( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps );

    expose( PSBoxPlacer )
    {
        parent(PSOperator);
        field( "Planar",        m_bPlanar       );
        field( "Surface",       m_bSurface      );
        field( "Ext",           m_Ext           );
    }
}; // class PSBoxPlacer

#endif // __PSBoxPlacer_H__


