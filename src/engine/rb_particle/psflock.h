//****************************************************************************/
//  File:   PSFlock.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSFlock_H__
#define __PSFlock_H__

//****************************************************************************/
//  Class:  PSFlock
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSFlock : public PSOperator
{
public: 
    PSFlock();

    expose( PSFlock )
    {
        parent(PSOperator);
    }
}; // class PSFlock

#endif // __PSFlock_H__


