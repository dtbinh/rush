//****************************************************************************/
//  File:   PSEmitter.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSEMITTER_H__
#define __PSEMITTER_H__

//****************************************************************************/
//	Enum:  PSEmitMode
//  Desc:  Particle emittance mode for emitter  
//****************************************************************************/
enum PSEmitMode
{
    EmitMode_Static = 0,
    EmitMode_Normal = 1,
    EmitMode_Burst  = 2
}; // enum PSEmitMode

enum_beg(PSEmitMode)
    enum_val( "Static",  EmitMode_Static ),
    enum_val( "Normal",  EmitMode_Normal ),
    enum_val( "Burst",   EmitMode_Burst  )
enum_end(PSEmitMode)

//****************************************************************************/
//	Enum:  PSCreateMode
//  Desc:  Child emitter creation mode
//****************************************************************************/
enum PSCreateMode
{
    CreateMode_OnBirth = 0,
    CreateMode_OnDeath = 1,
    CreateMode_OnHit   = 2,
}; // enum PSCreateMode

enum_beg(PSCreateMode)
    enum_val( "OnBirth",  CreateMode_OnBirth ),
    enum_val( "OnDeath",  CreateMode_OnDeath ),
    enum_val( "OnHit"  ,  CreateMode_OnHit   )
enum_end(PSCreateMode)

//****************************************************************************/
//	Enum:  PSRampUsage
//  Desc:  Defines the way some particular ramp operator is applied
//****************************************************************************/
enum PSRampUsage
{
    RampUsage_Particle = 0,     //  ramp horizontal axis is particle lifetime
    RampUsage_Emitter  = 1,     //  ramp horizontal axis is emitter lifetime (used for particle initialization)
    RampUsage_Init     = 2,     //  ramp horizontal axis is random value [0..1] (used for particle initialization)
}; // enum PSRampUsage

enum_beg(PSRampUsage)
    enum_val( "Particle",  RampUsage_Particle ),
    enum_val( "Emitter",   RampUsage_Emitter  ),
    enum_val( "Init",      RampUsage_Init     )
enum_end(PSRampUsage)

//****************************************************************************/
//	Enum:  PSAxis
//  Desc:  Defines the influence axis of some operator
//****************************************************************************/
enum PSAxis
{
    Axis_X      = 1,
    Axis_Y      = 2,
    Axis_XY     = 3,
    Axis_Z      = 4,
    Axis_YZ     = 5,
    Axis_XZ     = 6,
    Axis_XYZ    = 7
}; // enum PSAxis

enum_beg(PSAxis)
    enum_val( "X",   Axis_X   ),
    enum_val( "Y",   Axis_Y   ),
    enum_val( "XY",  Axis_XY  ),
    enum_val( "Z",   Axis_Z   ),
    enum_val( "YZ",  Axis_YZ  ),
    enum_val( "XZ",  Axis_XZ  ),
    enum_val( "XYZ", Axis_XYZ )
enum_end(PSAxis)

const float c_VeryBigLifeTime = 100000.0f;
class ParticleServer;
//****************************************************************************/
//  Class:  PSEmitter
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSEmitter : public PSOperator
{
    Vec3                m_Pos;          //  position of the emit centre, relative to parent frame
    
    DWORD               m_Color;        //  default particles color
    Vec3                m_Size;         //  default particles size

    float               m_LifeTime;     //  average paticles time-to-live
    float               m_LifeTimeD;    //  time-to-live deviation

    float               m_Rate;         //  emittance rate (particles per period)

    float               m_Period;       //  emittance period, sec (for burst mode)
    PSEmitMode          m_EmitMode;     //  mode of particle emittance
    PSCreateMode        m_CreateMode;   //  mode of child emitter creation

    bool                m_bParentSpace;  //  whether bind particles to this emitter's local space

    float               m_StartTime;    //  emitter starting time, relative to parent time frame
    float               m_Duration;     //  emitter duration
    bool                m_bLooped;      

public: 
                        PSEmitter       ();
    virtual void        Update          ( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps );
    bool                Process         ( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps );

    void                InitParticle    ( EmitterInstance* pInst, Particle* pParticle ) const;
    bool                IsParentSpace   () const { return m_bParentSpace; }
    float               GetDuration     () const { return m_Duration; } 

    expose( PSEmitter )
    {
        parent(PSOperator)
        field( "Position",           m_Pos           );
        field( "Color",         m_Color         );
        field( "Size",          m_Size          );
        field( "LifeTime",      m_LifeTime      );
        field( "LifeTimeD",     m_LifeTimeD     );
        field( "Rate",          m_Rate          );
        field( "Period",        m_Period        );
        field( "CreateMode",    m_CreateMode    );
        field( "EmitMode",      m_EmitMode      );
        field( "StartTime",     m_StartTime     );
        field( "Duration",      m_Duration      );
        field( "Looped",        m_bLooped       );
        field( "ParentSpace",   m_bParentSpace   );

    }

private:
    virtual PSEmitter* AssignEmitter( EmitterInstance* pInst, Particle* pHostParticle )
    {
        return this;
    }
}; // class PSEmitter

#endif // __PSEmitter_H__


