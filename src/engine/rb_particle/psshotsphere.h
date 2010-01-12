//****************************************************************************/
//  File:   PSShotSphere.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSHOTSPHERE_H__
#define __PSSHOTSPHERE_H__

#include "psoperator.h"

//****************************************************************************/
//  Class:  PSShotSphere
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSShotSphere : public PSOperator
{
public: 
    PSShotSphere();

    expose( PSShotSphere )
    {
        parent(PSOperator);
    }
}; // class PSShotSphere

#endif // __PSSHOTSPHERE_H__


