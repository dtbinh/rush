//****************************************************************************/
//  File:   PSShotMask.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSHOTMASK_H__
#define __PSSHOTMASK_H__

#include "PSOperator.h"

//****************************************************************************/
//  Class:  PSShotMask
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSShotMask : public PSOperator
{
public: 
    PSShotMask();

    expose( PSShotMask )
    {
        parent(PSOperator);
    }
}; // class PSShotMask

#endif // __PSSHOTMASK_H__


