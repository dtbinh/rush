//****************************************************************************/
//  File:   EmitterInstance.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __EmitterInstance_H__
#define __EmitterInstance_H__

class PSEmitter;
//****************************************************************************/
//  Class:  EmitterInstance
//  Desc:    
//****************************************************************************/
struct EmitterInstance  
{
    PSEmitter*          m_pEmitter;         //  emitter description
    Particle*           m_pParticle;        //  first particle in the list of emitted particles
    Particle*           m_pHostParticle;    //  parent particle, this emitter is attached to
    double              m_CurTime;          //  curent life of the emitter
    int                 m_NEmitted;         //  number of particles emitted so far
    int                 m_NumToEmit;        //  number of particles to emit at this update delta

    Vec3                m_Pos;              //  emitter's position
    Vec3                m_PrevPos;          //  previous emitter's position

    float               m_Alpha;            //  emitter's transparency multiplier
    float               m_Scale;            //  emitter's scale factor
    float               m_RateMultiplier;   //  coefficient to modify emitter rate

    EmitterInstance*    m_pChild;
    EmitterInstance*    m_pParent; 
    EmitterInstance*    m_pNext;
    EmitterInstance*    m_pPrev;

    Vec3                GetPos() const
    {
        Vec3 pos = m_Pos;
        if (m_pParent) 
        {
            pos += m_pParent->GetPos();
        }
        return pos;
    }


    EmitterInstance() {}

    void AddChild( EmitterInstance* pChild )
    {
        pChild->m_pNext = NULL;
        if (m_pChild) 
        {
            pChild->m_pNext = m_pChild;
            m_pChild->m_pPrev = pChild;
        }
        m_pChild = pChild;
        pChild->m_pPrev   = NULL;
        pChild->m_pParent = this;
    }

    void Init()
    {
        m_pParticle      = NULL;
        m_pHostParticle  = NULL;
        m_CurTime        = 0.0;
        m_pParent        = NULL; 
        m_NEmitted       = 0;
        m_NumToEmit      = 0;
        m_RateMultiplier = 1.0f;

        m_Pos            = Vec3::null; 
        m_PrevPos        = Vec3::null; 
        m_Alpha          = 1.0f;
        m_Scale          = 1.0f;

        m_pChild         = NULL;
        m_pParent        = NULL;
        m_pNext          = NULL;
        m_pPrev          = NULL;
        m_pEmitter       = NULL;
        m_pHostParticle  = NULL;
    }
}; // struct EmitterInstance

#endif // __EmitterInstance_H__


