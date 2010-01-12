//****************************************************************************/
//  File:   PSAttract.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSATTRACT_H__
#define __PSATTRACT_H__

//****************************************************************************/
//  Class:  PSAttract
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSAttract : public PSOperator
{
public: 
    PSAttract();

    expose( PSAttract )
    {
        parent(PSOperator);
    }
}; // class PSAttract

#endif // __PSAttract_H__


