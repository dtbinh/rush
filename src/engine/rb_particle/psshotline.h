//****************************************************************************/
//  File:   PSShotLine.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSHOTLINE_H__
#define __PSSHOTLINE_H__

#include "psoperator.h"

//****************************************************************************/
//  Class:  PSShotLine
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSShotLine : public PSOperator
{
public: 
    PSShotLine();

    expose( PSShotLine )
    {
        parent(PSOperator);
    }
}; // class PSShotLine

#endif // __PSSHOTLINE_H__


