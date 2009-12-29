//****************************************************************************/
//  File:   PSShotBox.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSHOTBOX_H__
#define __PSSHOTBOX_H__

#include "PSOperator.h"

//****************************************************************************/
//  Class:  PSShotBox
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSShotBox : public PSOperator
{
public: 
    PSShotBox();

    expose( PSShotBox )
    {
        parent(PSOperator);
    }
}; // class PSShotBox

#endif // __PSSHOTBOX_H__


