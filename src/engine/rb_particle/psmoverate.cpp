//****************************************************************************/
//  File:   PSMoveRate.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "stdafx.h"
#include "Particle.h"
#include "EmitterInstance.h"
#include "ParticleServer.h"
#include "PSOperator.h"
#include "PSMoveRate.h"

//****************************************************************************/
/*  PSMoveRate implementation
//****************************************************************************/
decl_class(PSMoveRate);
PSMoveRate::PSMoveRate()
{
    m_MinRate  = 0.0f;
    m_MaxRate  = 1.0f;
    m_MinSpeed = 0.0f;
    m_MaxSpeed = 100.0f;
} // PSMoveRate::PSMoveRate

void PSMoveRate::Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps )
{
    if ((mode&ProcessMode_State) == 0) return;
    float emSpeed     = (pInst->GetPos() - pInst->m_PrevPos).norm()/dt;
    float speedFactor = (emSpeed - m_MinSpeed)/(m_MaxSpeed - m_MinSpeed);
    float modifier    = m_MinRate + (m_MaxRate - m_MinRate)*speedFactor;
    modifier = clamp( modifier, 0.0f, 1.0f );
    pInst->m_RateMultiplier = modifier;
} // PSMoveRate::Update
