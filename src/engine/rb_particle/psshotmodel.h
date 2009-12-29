//****************************************************************************/
//  File:   PSShotModel.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSHOTMODEL_H__
#define __PSSHOTMODEL_H__

#include "PSOperator.h"

//****************************************************************************/
//  Class:  PSShotModel
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSShotModel : public PSOperator
{
public: 
    PSShotModel();

    expose( PSShotModel )
    {
        parent(PSOperator);
    }
}; // class PSShotModel

#endif // __PSSHOTMODEL_H__


