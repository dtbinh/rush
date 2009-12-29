/***********************************************************************************/
//  File:   PhysMaterial.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __PHYSMATERIAL_H__
#define __PHYSMATERIAL_H__

#include "ode/ode.h"
#include "PhysEntity.h"

/***********************************************************************************/
//  Class:  PhysMaterial
//  Desc:   
/***********************************************************************************/
class PhysMaterial : public PhysEntity
{
    float				m_Mu; 
    float				m_Mu2; 
    float				m_Bounce; 
    float				m_BounceVel; 
    float				m_SoftERP; 
    float				m_SoftCFM; 
    float				m_Motion1; 
    float				m_Motion2;
    float				m_Slip1;
    float				m_Slip2; 

    bool				m_bFDir1; 
    bool				m_bApprox1; 
    bool				m_bApprox2; 

    float               m_Density;

    friend class        PhysicsServer;

public:
                        PhysMaterial();  

    float               GetDensity() const { return m_Density; }

    expose( PhysMaterial )
    {
        parent( PhysEntity );
        field( "Mu",        m_Mu         );
        field( "Mu2",       m_Mu2        );
        field( "Bounce",    m_Bounce     );
        field( "BounceVel", m_BounceVel  );
        field( "SoftERP",   m_SoftERP    );
        field( "SoftCFM",   m_SoftCFM    );
        field( "Motion1",   m_Motion1    );
        field( "Motion2",   m_Motion2    );
        field( "Slip1",     m_Slip1      );
        field( "Slip2",     m_Slip2      );
        field( "FDir1",     m_bFDir1     );
        field( "Approx1",   m_bApprox1   );
        field( "Approx2",   m_bApprox2   );
        field( "Density",   m_Density    );
    }
}; // class PhysMaterial

#endif //__PHYSMATERIAL_H__