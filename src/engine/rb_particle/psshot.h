//****************************************************************************/
//  File:   PSShot.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSHOT_H__
#define __PSSHOT_H__

//****************************************************************************/
//  Class:  PSShot
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSShot : public PSOperator
{
    Vec3                m_Dir;          //  emittance direction
    float               m_ConeAngle;    //  emittance cone angle
    float               m_ConeAngleD;   //  emittance cone angle deviation

    float               m_Velocity;     //  starting velocity
    float               m_VelocityD;    //  starting velocity deviation

    Vec3                m_AngVel;       //  starting angular velocity
    Vec3                m_AngVelD;      //  starting angular velocity deviation

    bool                m_bPlanar;      
    bool                m_bSurface;     

public: 
    PSShot();
    virtual void        Update          ( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );
    

    expose( PSShot )
    {
        parent(PSOperator);
        field( "Dir",           m_Dir           );
        field( "ConeAngle",     m_ConeAngle     );
        field( "ConeAngleD",    m_ConeAngleD    );
        field( "Velocity",      m_Velocity      );
        field( "VelocityD",     m_VelocityD     );
        field( "AngVel",        m_AngVel        );
        field( "AngVelD",       m_AngVelD       );
        field( "Planar",        m_bPlanar       );
        field( "Surface",       m_bSurface      );
    }

private:
    Vec3                GetDirection    () const;
}; // class PSShot

#endif // __PSSHOT_H__


