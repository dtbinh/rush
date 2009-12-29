//****************************************************************************/
//  File:   PSShotCyl.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSHOTCYL_H__
#define __PSSHOTCYL_H__

#include "PSOperator.h"

//****************************************************************************/
//  Class:  PSShotCyl
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSShotCyl : public PSOperator
{
public: 
    PSShotCyl();

    expose( PSShotCyl )
    {
        parent(PSOperator);
    }
}; // class PSShotCyl

#endif // __PSSHOTCYL_H__


