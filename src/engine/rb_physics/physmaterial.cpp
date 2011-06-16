/***********************************************************************************/
//  File:   PhysMaterial.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "PhysMaterial.h"
#include "PhysicsServer.h"

/***********************************************************************************/
/*  PhysMaterial implementation
/***********************************************************************************/
decl_class(PhysMaterial)
PhysMaterial::PhysMaterial()
{
    m_Mu            = 0.2f; 
    m_Mu2           = 0.2f;
    m_bFDir1        = false; 
    m_Bounce        = 0.3f; 
    m_BounceVel     = 0.5f;
    m_SoftERP       = 0.96f; 
    m_SoftCFM       = 0.05f; 
    m_Motion1       = 0.0f; 
    m_Motion2       = 0.0f;
    m_Slip1         = 0.1f;
    m_Slip2         = 0.1f; 
    m_bApprox1      = true;
    m_bApprox2      = true; 

    m_Density       = 2.0f;
} // PhysMaterial::PhysMaterial