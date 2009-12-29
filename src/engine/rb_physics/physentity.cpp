/***********************************************************************************/
//  File:   PhysEntity.cpp
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "PhysEntity.h"
#include "PhysicsServer.h"

/***********************************************************************************/
/*  PhysEntity implementation
/***********************************************************************************/
decl_class(PhysEntity);

void PhysEntity::SetModelInstID( int modelInstID ) 
{ 
    m_ModelInstID = modelInstID; 
} // PhysEntity::SetModelInstID
